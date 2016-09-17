#include "Animation\AnimatedInstanceModel.h"

#include "cal3d\coremodel.h"
#include "cal3d\hardwaremodel.h"
#include "cal3d\model.h"
#include "cal3d\mixer.h"
#include "cal3d\animation.h"
#include "cal3d\animation_action.h"
#include "cal3d\animation_cycle.h"
#include "cal3d\skeleton.h"
#include "cal3d\bone.h"

#include "XML\XMLTreeNode.h"
#include "Vertex\RenderableVertexs.h"
#include "Vertex\VertexTypes.h"
#include "Textures\TextureManager.h"
#include "Engine\Engine.h"
#include "RenderableObjects\RenderableObjectTechnique.h"
#include "Animation\AnimatedModelManager.h"
#include "Animation\tick.h"
#include "Log\Log.h"


CAnimatedInstanceModel::CAnimatedInstanceModel(CGameObject* Owner, const std::string &Name, const std::string &ModelName, const Vect3f &Position, float Yaw, float Pitch, float Roll)
:CRenderableObject(Owner, Name, Position, Yaw, Pitch, Roll)
,m_CalModel(nullptr), m_AnimatedCoreModel(nullptr), m_CalHardwareModel(nullptr), m_Materials(NULL), m_RenderableVertexs(nullptr), m_TemporalMaterials(NULL)
,m_NumVertices(0), m_NumFaces(0), m_lastTick(0)
,m_fpsDuration(0.0f), m_fpsFrames(0), m_fps(0)
,m_bPaused(false), m_blendTime(0.3f)
,m_TemporalRenderbleObjectTechnique(nullptr)
{
	CAnimatedCoreModel* l_AnimatedCoreModel = CEngine::GetSingleton().GetAnimatedModelManager()->GetResource(ModelName);
	if (l_AnimatedCoreModel == nullptr)
	{
		#ifdef _DEBUG
			CEngine::GetSingleton().GetLogManager()->Log("The animated core model " + ModelName + " doesn't exists");
		#endif
		assert(false);
	}
	Initialize(l_AnimatedCoreModel);
}

CAnimatedInstanceModel::CAnimatedInstanceModel(CXMLTreeNode &TreeNode)
:CRenderableObject(TreeNode)
,m_CalModel(nullptr), m_AnimatedCoreModel(nullptr), m_CalHardwareModel(nullptr), m_Materials(NULL), m_RenderableVertexs(nullptr), m_TemporalMaterials(NULL)
,m_NumVertices(0), m_NumFaces(0), m_lastTick(0)
,m_fpsDuration(0.0f), m_fpsFrames(0), m_fps(0)
,m_bPaused(false), m_blendTime(0.3f)
,m_TemporalRenderbleObjectTechnique(nullptr)
{
	std::string l_ModelName = TreeNode.GetPszProperty("model_name");
	CAnimatedCoreModel* l_AnimatedCoreModel = CEngine::GetSingleton().GetAnimatedModelManager()->GetResource(l_ModelName);
	if (l_AnimatedCoreModel == nullptr)
	{
		#ifdef _DEBUG
			CEngine::GetSingleton().GetLogManager()->Log("The animated core model " + l_ModelName + " doesn't exists");
		#endif
		assert(false);
	}
	Initialize(l_AnimatedCoreModel);
}

CAnimatedInstanceModel::~CAnimatedInstanceModel()
{
	Destroy();
}

/*
Creates a copy of the core materials. 
Instance will be rendered with these materials.
*/
std::vector<CMaterial*> CAnimatedInstanceModel::CreateCopyMaterialsFromCore()
{
	for (size_t i = 0; i < m_Materials.size(); ++i)
	{
		CMaterial* l_CopyMaterial = new CMaterial(*m_Materials[i]);
		m_TemporalMaterials.push_back(l_CopyMaterial);
	}
	m_Materials.clear();
	return m_TemporalMaterials;
}

void CAnimatedInstanceModel::Destroy()
{
	m_Materials.clear();

	for (size_t i = 0; i < m_TemporalMaterials.size(); ++i)
	{
		if (m_TemporalMaterials[i])
		{
			delete m_TemporalMaterials[i];
			m_TemporalMaterials[i] = NULL;
		}
	}
	m_TemporalMaterials.clear();
	CHECKED_DELETE(m_CalModel);
	CHECKED_DELETE(m_CalHardwareModel);
	CHECKED_DELETE(m_RenderableVertexs);
}

bool s_Update = true;

void CAnimatedInstanceModel::Initialize(CAnimatedCoreModel *AnimatedCoreModel)
{
	m_AnimatedCoreModel=AnimatedCoreModel;
	LoadMaterials();

	int materialId;
	for(materialId = 0; materialId < m_AnimatedCoreModel->GetCoreModel()->getCoreMaterialCount(); materialId++)
	{
		m_AnimatedCoreModel->GetCoreModel()->createCoreMaterialThread(materialId);
		m_AnimatedCoreModel->GetCoreModel()->setCoreMaterialId(materialId, 0, materialId);
	}

	m_CalModel = new CalModel(m_AnimatedCoreModel->GetCoreModel());

	//onInit
	
  //int materialId;
  for(materialId = 0; materialId < m_AnimatedCoreModel->GetCoreModel()->getCoreMaterialCount(); materialId++)
  {
    CalCoreMaterial *pCoreMaterial;
    pCoreMaterial = m_AnimatedCoreModel->GetCoreModel()->getCoreMaterial(materialId);

    int mapId;
    for(mapId = 0; mapId < pCoreMaterial->getMapCount(); mapId++)
    {
      std::string strFilename;
      strFilename = pCoreMaterial->getMapFilename(mapId);

      // load the texture from the file
      //LPDIRECT3DTEXTURE9 texture;
      //texture = loadTexture(strFilename);

      // store the d3d texture id in the user data of the map
	  pCoreMaterial->setMapUserData(mapId, (Cal::UserData)CEngine::GetSingleton().GetTextureManager()->GetResource(strFilename));
    }
  }

	//Atach all meshes to the model
	int meshId;
	for(meshId = 0; meshId < m_AnimatedCoreModel->GetCoreModel()->getCoreMeshCount(); ++meshId)
	{
		m_CalModel->attachMesh(meshId);
	}

	 m_CalModel->setMaterialSet(0);

	m_currentAnimationId = 0;
	m_leftAnimationTime = m_AnimatedCoreModel->GetCoreModel()->getCoreAnimation(m_currentAnimationId)->getDuration() - m_blendTime;

	if (m_AnimatedCoreModel->GetCoreModel()->getCoreAnimationCount() > 1)
	{
		//m_CalModel->getMixer()->blendCycle(m_currentAnimationId, 0.1f, 0.0f);
	}

	m_CalModel->disableInternalData();

	m_lastTick = Tick::getTick();

	m_CalHardwareModel = new CalHardwareModel(m_AnimatedCoreModel->GetCoreModel());

	LoadVertexBuffer();
}

void CAnimatedInstanceModel::SetTemporalRenderableObjectTechnique(CRenderableObjectTechnique* RenderableObjectTechnique)
{
	m_TemporalRenderbleObjectTechnique = RenderableObjectTechnique;
}

void CAnimatedInstanceModel::Render(CRenderManager *RenderManager)
{	
	if (m_Enabled) //&& (RenderManager->GetFrustum().BoxVisible(l_Max, l_Min))
	{
		CEffectManager::m_SceneEffectParameters.m_World = GetTransform();
		int l_HardwareMeshCount = m_CalHardwareModel->getHardwareMeshCount();
		CMaterial* l_Material;

		for (int l_HardwareMeshId = 0; l_HardwareMeshId < l_HardwareMeshCount; ++l_HardwareMeshId)
		{
			if (m_TemporalMaterials.size() > 0)
			{
				l_Material = m_TemporalMaterials[l_HardwareMeshId];
			}
			else
			{
				l_Material = m_Materials[l_HardwareMeshId];
			}

			l_Material->Apply();
			m_CalHardwareModel->selectHardwareMesh(l_HardwareMeshId);
			Mat44f l_Transformations[MAXBONES];

			for (int l_BoneId = 0; l_BoneId < m_CalHardwareModel->getBoneCount(); ++l_BoneId)
			{
				Quatf l_Quaternion = (const Quatf &)m_CalHardwareModel->getRotationBoneSpace(l_BoneId, m_CalModel->getSkeleton());
				l_Transformations[l_BoneId].SetIdentity();
				l_Transformations[l_BoneId].SetRotByQuat(l_Quaternion);
				CalVector translationBoneSpace = m_CalHardwareModel->getTranslationBoneSpace(l_BoneId, m_CalModel->getSkeleton());
				l_Transformations[l_BoneId].SetPos(Vect3f(translationBoneSpace.x, translationBoneSpace.y, translationBoneSpace.z));
			}

			memcpy(&CEffectManager::m_AnimatedModelEffectParameters.m_Bones, l_Transformations, MAXBONES*sizeof(float) * 4 * 4);
			if (!m_TemporalRenderbleObjectTechnique)
			{
				l_Material->GetRenderableObjectTechnique()->GetEffectTechnique()->SetConstantBuffer(2, &CEffectManager::m_AnimatedModelEffectParameters.m_Bones);
				m_RenderableVertexs->RenderIndexed(RenderManager, l_Material->GetRenderableObjectTechnique()->GetEffectTechnique(), &CEffectManager::m_SceneEffectParameters, m_CalHardwareModel->getFaceCount() * 3, m_CalHardwareModel->getStartIndex(), m_CalHardwareModel->getBaseVertexIndex());
			}
			else
			{
				m_TemporalRenderbleObjectTechnique->GetEffectTechnique()->SetConstantBuffer(2, &CEffectManager::m_AnimatedModelEffectParameters.m_Bones);
				m_RenderableVertexs->RenderIndexed(RenderManager, m_TemporalRenderbleObjectTechnique->GetEffectTechnique(), &CEffectManager::m_SceneEffectParameters, m_CalHardwareModel->getFaceCount() * 3, m_CalHardwareModel->getStartIndex(), m_CalHardwareModel->getBaseVertexIndex());
			}
		}
	}
}

Mat44f CAnimatedInstanceModel::GetBoneTransformationMatrix(const int BoneID) const
{
	Mat44f l_BoneTransformation; 
	CalBone* l_Bone = m_CalModel->getSkeleton()->getBone(BoneID);
	CalQuaternion l_Rotation = l_Bone->getRotationAbsolute();
	CalVector l_Translation = l_Bone->getTranslationAbsolute();
		
	//OK
	l_BoneTransformation.SetIdentity();
	l_BoneTransformation.SetRotByQuat((const Quatf &)l_Rotation);
	l_BoneTransformation.SetPos(Vect3f(l_Translation.x, l_Translation.y, l_Translation.z));
	
	//Vect3f eulers = ((Quatf &)l_Rotation).GetRadians();
	
	//Quatf q1(eulers.y, eulers.x, eulers.z);
	//l_BoneTransformation.SetRotByQuat(q1); //x,y,z x,z,y y,x,z y,z,x z,x,y
	
	//l_BoneTransformation.SetRotByAnglesYXZ(eulers.x, eulers.y, eulers.z);
	//l_BoneTransformation.SetPos(Vect3f(l_Translation.x, l_Translation.y, l_Translation.z));

	return l_BoneTransformation;
}

void CAnimatedInstanceModel::Update(float ElapsedTime)
{
	if (s_Update)
		m_CalModel->update(ElapsedTime);
}

void CAnimatedInstanceModel::ExecuteAction(int Id, float DelayIn, float DelayOut, float WeightTarget, float Speed, bool AutoLock)
{
	m_CalModel->getMixer()->executeAction(Id, DelayIn, DelayOut,WeightTarget,AutoLock);
	std::list<CalAnimationAction*> l_AnimationActionList = m_CalModel->getMixer()->getAnimationActionList();
	if (l_AnimationActionList.size()>0)
		l_AnimationActionList.front()->setTimeFactor(Speed);
}

void CAnimatedInstanceModel::BlendCycle(int Id, float Weight, float DelayIn, float Speed)
{
	m_CalModel->getMixer()->setTimeFactor(Speed);
	m_CalModel->getMixer()->blendCycle(Id, Weight, DelayIn);
}

void CAnimatedInstanceModel::ClearCycle(int Id, float DelayOut)
{
	m_CalModel->getMixer()->clearCycle(Id,DelayOut);
}

bool CAnimatedInstanceModel::IsCycleAnimationActive(int Id) const
{
	CalCoreAnimation* l_CoreAnimation = m_CalModel->getCoreModel()->getCoreAnimation(Id);
	if (l_CoreAnimation == 0) { return false; }

	std::list<CalAnimationCycle*> l_AnimationCycleList = m_CalModel->getMixer()->getAnimationCycle();
	if (l_AnimationCycleList.size() == 0)
		return false;

	for (std::list<CalAnimationCycle*>::iterator it = l_AnimationCycleList.begin(); it != l_AnimationCycleList.end(); ++it)
	{
		std::string l_Name = l_CoreAnimation->getName();
		std::string l_NameIt = (*it)->getCoreAnimation()->getName();
		if (l_Name == l_NameIt)
			return true;
	}

	return false;
}

bool CAnimatedInstanceModel::IsActionAnimationActive(int Id) const
{
	CalCoreAnimation* l_CoreAnimation = m_CalModel->getCoreModel()->getCoreAnimation(Id);
	if (l_CoreAnimation == 0) { return false; }

	std::list<CalAnimationAction*> l_AnimationActionList = m_CalModel->getMixer()->getAnimationActionList();
	if (l_AnimationActionList.size() == 0)
		return false;

	for (std::list<CalAnimationAction*>::iterator it = l_AnimationActionList.begin(); it != l_AnimationActionList.end(); ++it)
	{
		std::string l_Name = l_CoreAnimation->getName();
		std::string l_NameIt = (*it)->getCoreAnimation()->getName();
		if (l_Name == l_NameIt)
			return true;
	}

	return false;
}

bool CAnimatedInstanceModel::LoadVertexBuffer()
{
	m_NumVertices = 0;
	m_NumFaces = 0;
	unsigned int l_VertexType = 0;
	if (m_AnimatedCoreModel->GetVertexTypes().size() > 0)
		l_VertexType = m_AnimatedCoreModel->GetVertexTypes()[0];

	CalCoreModel *l_CalCoreModel = m_AnimatedCoreModel->GetCoreModel();
	for (int i = 0; i<l_CalCoreModel->getCoreMeshCount(); ++i)
	{
		CalCoreMesh *l_CalCoreMesh = l_CalCoreModel->getCoreMesh(i);
		for (int j = 0; j<l_CalCoreMesh->getCoreSubmeshCount(); ++j)
		{
			CalCoreSubmesh *l_CalCoreSubmesh = l_CalCoreMesh->getCoreSubmesh(j);
			m_NumVertices += l_CalCoreSubmesh->getVertexCount();
			m_NumFaces += l_CalCoreSubmesh->getFaceCount();
		}
	}

	if (l_VertexType == MV_POSITION_WEIGHT_INDICES_NORMAL_BINORMAL_TANGENT_TEXTURE_VERTEX::GetVertexType())
	{
		MV_POSITION_WEIGHT_INDICES_NORMAL_BINORMAL_TANGENT_TEXTURE_VERTEX*l_Vertexs = (MV_POSITION_WEIGHT_INDICES_NORMAL_BINORMAL_TANGENT_TEXTURE_VERTEX*)malloc(m_NumFaces * 3 * sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_BINORMAL_TANGENT_TEXTURE_VERTEX));
		CalIndex *l_Faces = (CalIndex *)malloc(m_NumFaces * 3 * sizeof(CalIndex));
		m_CalHardwareModel->setVertexBuffer((char*)l_Vertexs, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_BINORMAL_TANGENT_TEXTURE_VERTEX));
		m_CalHardwareModel->setWeightBuffer(((char*)l_Vertexs) + 12, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_BINORMAL_TANGENT_TEXTURE_VERTEX));
		m_CalHardwareModel->setMatrixIndexBuffer(((char*)l_Vertexs) + 28, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_BINORMAL_TANGENT_TEXTURE_VERTEX));
		m_CalHardwareModel->setNormalBuffer(((char*)l_Vertexs) + 44, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_BINORMAL_TANGENT_TEXTURE_VERTEX));
		
		m_CalHardwareModel->setTangentSpaceBuffer(1, ((char*)l_Vertexs) + 56, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_BINORMAL_TANGENT_TEXTURE_VERTEX));
		
		m_CalHardwareModel->setTextureCoordNum(1);
		m_CalHardwareModel->setTextureCoordBuffer(0, ((char*)l_Vertexs) + 88, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_BINORMAL_TANGENT_TEXTURE_VERTEX));
		m_CalHardwareModel->setIndexBuffer(l_Faces);
		m_CalHardwareModel->load(0, 0, MAXBONES);
		m_NumFaces = m_CalHardwareModel->getTotalFaceCount();
		m_NumVertices = m_CalHardwareModel->getTotalVertexCount();

		CRenderableVertexs::CalcTangentsAndBinormals(l_Vertexs, (unsigned short*)l_Faces, m_NumVertices, m_NumFaces, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_BINORMAL_TANGENT_TEXTURE_VERTEX), 0, 44, 56, 72, 88);
	
		if (sizeof(CalIndex) == 2)
			m_RenderableVertexs = new CTriangleListRenderableIndexed16Vertexs<MV_POSITION_WEIGHT_INDICES_NORMAL_BINORMAL_TANGENT_TEXTURE_VERTEX>(l_Vertexs, m_NumVertices, l_Faces, m_NumFaces * 3, false);
		else
			m_RenderableVertexs = new CTriangleListRenderableIndexed32Vertexs<MV_POSITION_WEIGHT_INDICES_NORMAL_BINORMAL_TANGENT_TEXTURE_VERTEX>(l_Vertexs, m_NumVertices, l_Faces, m_NumFaces * 3, false);
		
		free(l_Vertexs);
		free(l_Faces);
	}
	else if (l_VertexType == MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX::GetVertexType())
	{
		MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX*l_Vertexs = (MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX*)malloc(m_NumFaces * 3 * sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));
		CalIndex *l_Faces = (CalIndex *)malloc(m_NumFaces * 3 * sizeof(CalIndex));
		m_CalHardwareModel->setVertexBuffer((char*)l_Vertexs, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));
		m_CalHardwareModel->setWeightBuffer(((char*)l_Vertexs) + 12, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));
		m_CalHardwareModel->setMatrixIndexBuffer(((char*)l_Vertexs) + 28, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));
		m_CalHardwareModel->setNormalBuffer(((char*)l_Vertexs) + 44, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));
		m_CalHardwareModel->setTextureCoordNum(1);
		m_CalHardwareModel->setTextureCoordBuffer(0, ((char*)l_Vertexs) + 56, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));
		m_CalHardwareModel->setIndexBuffer(l_Faces);
		m_CalHardwareModel->load(0, 0, MAXBONES);
		m_NumFaces = m_CalHardwareModel->getTotalFaceCount();
		m_NumVertices = m_CalHardwareModel->getTotalVertexCount();

		if (sizeof(CalIndex) == 2)
			m_RenderableVertexs = new CTriangleListRenderableIndexed16Vertexs<MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX>(l_Vertexs, m_NumVertices, l_Faces, m_NumFaces * 3, false);
		else
			m_RenderableVertexs = new CTriangleListRenderableIndexed32Vertexs<MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX>(l_Vertexs, m_NumVertices, l_Faces, m_NumFaces * 3, false);
	
		free(l_Vertexs);
		free(l_Faces);
	}

	return true;
}

void CAnimatedInstanceModel::LoadMaterials()
{
	m_Materials = m_AnimatedCoreModel->GetMaterials();
}