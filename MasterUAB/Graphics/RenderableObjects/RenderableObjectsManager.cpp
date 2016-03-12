#include "RenderableObjects\RenderableObjectsManager.h"
#include "Utils\Utils.h"
#include "Engine.h"
#include "RenderableObjects\MeshInstance.h"
#include "Animation\AnimatedInstanceModel.h"
#include "Animation\AnimatedModelManager.h"
#include "Effects\EffectManager.h"
#include "Cinematics\Cinematic.h"

CRenderableObjectsManager::CRenderableObjectsManager() : CNamed("")
{
}

CRenderableObjectsManager::~CRenderableObjectsManager()
{

}

void CRenderableObjectsManager::Reload()
{
	Destroy();
	Load(m_Filename);
}

void CRenderableObjectsManager::Update(float ElapsedTime)
{
	for (size_t i = 0; i<m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->Update(ElapsedTime);
	}
}

void CRenderableObjectsManager::Render(CRenderManager *RM)
{
	for (size_t i = 0; i<m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->Render(RM);
	}
}

CRenderableObject * CRenderableObjectsManager::AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position)
{
	CMeshInstance *l_MeshInstance;
	l_MeshInstance = new CMeshInstance(InstanceName,CoreMeshName);
	l_MeshInstance->SetPosition(Position);

	if(!AddResource(l_MeshInstance->GetName(),l_MeshInstance))
	{
		CHECKED_DELETE(l_MeshInstance);
	}
	
	return l_MeshInstance;
}

CRenderableObject * CRenderableObjectsManager::AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position)
{
	CAnimatedInstanceModel *l_AnimatedInstanceModel;
	l_AnimatedInstanceModel = new CAnimatedInstanceModel(InstanceModelName);
	l_AnimatedInstanceModel->SetPosition(Position);

	if(!AddResource(l_AnimatedInstanceModel->GetName(),l_AnimatedInstanceModel))
	{
		CHECKED_DELETE(l_AnimatedInstanceModel);
	}
	else
	{
		l_AnimatedInstanceModel->Initialize(CEngine::GetSingleton().GetAnimatedModelManager()->GetResource(CoreModelName));
	}
	
	return l_AnimatedInstanceModel;
}

/*void CRenderableObjectsManager::AddResource(const std::string &Name, CRenderableObject *RenderableObject)
{

}*/

/*void CRenderableObjectsManager::CleanUp()
{

}*/

void CRenderableObjectsManager::Load(const std::string &FileName)
{
	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode l_RenderableObjects = l_XML["renderable_objects"];

		if (l_RenderableObjects.Exists())
		{
			for (int i = 0; i < l_RenderableObjects.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_RenderableObjects(i);

				if (l_Element.GetName() == std::string("mesh_instance"))
				{
					CMeshInstance *l_MeshInstance = new CMeshInstance(l_Element.GetPszProperty("name",""),l_Element.GetPszProperty("core_name",""));
					l_MeshInstance->SetPosition(l_Element.GetVect3fProperty("pos",v3fZERO));
					l_MeshInstance->SetYaw(l_Element.GetFloatProperty("yaw",0.0f));
					l_MeshInstance->SetPitch(l_Element.GetFloatProperty("pitch",0.0f));
					l_MeshInstance->SetRoll(l_Element.GetFloatProperty("roll",0.0f));

					if(!AddResource(l_MeshInstance->GetName(), l_MeshInstance))
					{
						CHECKED_DELETE(l_MeshInstance);
					}
				}
				else if (l_Element.GetName() == std::string("animated_model"))
				{
					AddAnimatedInstanceModel(l_Element.GetPszProperty("model_name"),l_Element.GetPszProperty("name"),l_Element.GetVect3fProperty("pos",v3fZERO));
					GetResource(l_Element.GetPszProperty("name"))->SetYaw(l_Element.GetFloatProperty("yaw",0.0f));
					GetResource(l_Element.GetPszProperty("name"))->SetPitch(l_Element.GetFloatProperty("pitch",0.0f));
					GetResource(l_Element.GetPszProperty("name"))->SetRoll(l_Element.GetFloatProperty("roll",0.0f));
					//(CAnimatedInstanceModel)(GetResource(l_Element.GetPszProperty("name")))
				}
				else if (l_Element.GetName() == std::string("cinematic"))
				{
					CCinematic *l_Cinematic = new CCinematic(l_Element);
					l_Cinematic->LoadXML(l_Element.GetPszProperty("file"));

					if(!AddResource(l_Cinematic->GetName(),l_Cinematic))
					{
						CHECKED_DELETE(l_Cinematic);
					}
				}
			}
		}
	}
}