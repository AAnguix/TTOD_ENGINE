#include "Animation\AnimatedInstanceModel.h"
#include "XML\XMLTreeNode.h"
#include "Render\RenderManager.h"
#include "Animation\AnimatedCoreModel.h"
#include "Utils\Utils.h"
#include "Materials\Material.h"
#include "Vertex\RenderableVertexs.h"
#include "Effects\EffectManager.h"

#include "cal3d\coresubmesh.h"
#include "cal3d\mixer.h"
#include "cal3d\animation.h"

#include "Vertex\VertexTypes.h"
#include "Engine.h"

#include "Animation\AnimatedModelManager.h"
#include "Animation\tick.h"
#include "Textures\TextureManager.h"

#include "RenderableObjects\RenderableObjectTechnique.h"


#include "Components\ComponentManager.h"

CAnimatedInstanceModel::CAnimatedInstanceModel(const std::string &Name, const std::string &ModelName, const Vect3f &Position, float Yaw, float Pitch, float Roll)
:CRenderableObject(Name, Position, Yaw, Pitch, Roll)
,m_CalModel(NULL), m_AnimatedCoreModel(NULL), m_CalHardwareModel(NULL), m_Materials(NULL), m_RenderableVertexs(NULL)
,m_NumVertices(0), m_NumFaces(0), m_lastTick(0)
,m_fpsDuration(0.0f), m_fpsFrames(0), m_fps(0)
,m_bPaused(false), m_blendTime(0.3f)
{
	Initialize(CEngine::GetSingleton().GetAnimatedModelManager()->GetResource(ModelName));
}

CAnimatedInstanceModel::CAnimatedInstanceModel(CXMLTreeNode &TreeNode)
:CRenderableObject(TreeNode)
,m_CalModel(NULL), m_AnimatedCoreModel(NULL), m_CalHardwareModel(NULL), m_Materials(NULL), m_RenderableVertexs(NULL)
,m_NumVertices(0), m_NumFaces(0), m_lastTick(0)
,m_fpsDuration(0.0f), m_fpsFrames(0), m_fps(0)
,m_bPaused(false), m_blendTime(0.3f)
{
	Initialize(CEngine::GetSingleton().GetAnimatedModelManager()->GetResource(TreeNode.GetPszProperty("model_name")));
}

CAnimatorController* CAnimatedInstanceModel::GetAnimatorController() const
{
	return (CAnimatorController*)m_ComponentManager->GetResource("AnimatorController");
}

CAnimatedInstanceModel::~CAnimatedInstanceModel()
{
	Destroy();
}

void CAnimatedInstanceModel::Destroy()
{
	m_Materials.clear();
	CHECKED_DELETE(m_CalModel);
	CHECKED_DELETE(m_CalHardwareModel);
	CHECKED_DELETE(m_RenderableVertexs);
}

void CAnimatedInstanceModel::Initialize(CAnimatedCoreModel *AnimatedCoreModel)
{
	//onCreate
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

void CAnimatedInstanceModel::Render(CRenderManager *RenderManager)
{
	//m_Position=CEngine::GetSingleton().GetPhysXManager()->GetActorPosition(m_Name); /*TODO VER SI FUNCIONA*/
	m_ComponentManager->Render(*RenderManager);

	CEffectManager::m_SceneEffectParameters.m_World=GetTransform(); 

	for(int l_HardwareMeshId=0; l_HardwareMeshId<m_CalHardwareModel->getHardwareMeshCount(); ++l_HardwareMeshId)
	{
		m_Materials[l_HardwareMeshId]->Apply();
		m_CalHardwareModel->selectHardwareMesh(l_HardwareMeshId);
		Mat44f l_Transformations[MAXBONES];

		for(int l_BoneId=0; l_BoneId<m_CalHardwareModel->getBoneCount();++l_BoneId)
		{
			Quatf l_Quaternion=(const Quatf &)m_CalHardwareModel->getRotationBoneSpace(l_BoneId, m_CalModel->getSkeleton());
			l_Transformations[l_BoneId].SetIdentity();
			l_Transformations[l_BoneId].SetRotByQuat(l_Quaternion);
			CalVector translationBoneSpace=m_CalHardwareModel->getTranslationBoneSpace(l_BoneId, m_CalModel->getSkeleton());
			l_Transformations[l_BoneId].SetPos(Vect3f(
			translationBoneSpace.x, translationBoneSpace.y, translationBoneSpace.z));
		}

		memcpy(&CEffectManager::m_AnimatedModelEffectParameters
		.m_Bones, l_Transformations, MAXBONES*sizeof(float)*4*4);
		m_Materials[l_HardwareMeshId]->GetRenderableObjectTechnique()->GetEffectTechnique()->SetConstantBuffer(2,
		&CEffectManager::m_AnimatedModelEffectParameters.m_Bones);
		m_RenderableVertexs->RenderIndexed(RenderManager,
		m_Materials[l_HardwareMeshId]->GetRenderableObjectTechnique()->GetEffectTechnique(), &CEffectManager::m_SceneEffectParameters,m_CalHardwareModel->getFaceCount()*3, m_CalHardwareModel->getStartIndex(),m_CalHardwareModel->getBaseVertexIndex());
	}
}

void CAnimatedInstanceModel::Update(float ElapsedTime)
{
	if(m_ComponentManager!=NULL)
	{
		m_ComponentManager->Update(ElapsedTime);
	}

	m_CalModel->update(ElapsedTime);
}

void CAnimatedInstanceModel::ExecuteAction(int Id, float DelayIn, float DelayOut, float WeightTarget, bool AutoLock)
{
	m_CalModel->getMixer()->executeAction(Id, DelayIn, DelayOut,WeightTarget,AutoLock);
}

void CAnimatedInstanceModel::BlendCycle(int Id, float Weight, float DelayIn)
{
	 m_CalModel->getMixer()->blendCycle(Id, Weight, DelayIn);
}

void CAnimatedInstanceModel::ClearCycle(int Id, float DelayOut)
{
	m_CalModel->getMixer()->clearCycle(Id,DelayOut);
}

bool CAnimatedInstanceModel::IsCycleAnimationActive(int Id) const
{
	if(m_CalModel->getMixer()->getAnimationVector().at(Id)->getType()==CalAnimation::TYPE_CYCLE && 
		m_CalModel->getMixer()->getAnimationVector().at(Id)->getState()==CalAnimation::STATE_IN)
		return true;

	return false;
}

bool CAnimatedInstanceModel::IsActionAnimationActive(int Id) const
{
	if(m_CalModel->getMixer()->getAnimationVector().at(Id)->getType()==CalAnimation::TYPE_ACTION && 
		m_CalModel->getMixer()->getAnimationVector().at(Id)->getState()==CalAnimation::STATE_IN)
		return true;

	return false;
}

bool CAnimatedInstanceModel::LoadVertexBuffer()
{
	m_NumVertices = 0;
	m_NumFaces = 0;

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

	return true;
}

void CAnimatedInstanceModel::LoadMaterials()
{
	m_Materials = m_AnimatedCoreModel->GetMaterials();
	/*int materialId;
	for(materialId = 0; materialId < m_AnimatedCoreModel->GetCoreModel()->getCoreMaterialCount(); ++materialId)
	{
	CalCoreMaterial *pCoreMaterial;
	pCoreMaterial =  m_AnimatedCoreModel->GetCoreModel()->getCoreMaterial(materialId);

	int mapId;
	for(mapId = 0; mapId < pCoreMaterial->getMapCount(); ++mapId)
	{
	//std::string strFilename;
	//strFilename = pCoreMaterial->getMapFilename(mapId);

	CMaterial* l_Material = CEngine::GetSingleton().GetMaterialManager()->GetResource(pCoreMaterial->getName());
	if(l_Material!=NULL)
	{
	m_Materials.push_back(l_Material);
	}else
	{
	CHECKED_DELETE(l_Material)
	}
	//CTexture *l_Texture = CEngine::GetSingleton().GetTextureManager()->GetTexture(strFilename);
	//m_Textures.push_back(l_Texture);
	//CEngine::GetSingleton().GetMaterialManager()->AddResource("",new CMaterial());

	//LPDIRECT3DTEXTURE9 texture;
	//texture = loadTexture(strFilename);
	//pCoreMaterial->setMapUserData(mapId, (Cal::UserData)texture);
	}
	}

	m_CalModel->setMaterialSet(0);*/
}