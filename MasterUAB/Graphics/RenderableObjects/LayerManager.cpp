#include "LayerManager.h"
#include "RenderableObjects\MeshInstance.h"
#include "Animation\AnimatedModelManager.h"
#include "Animation\AnimatedInstanceModel.h"
#include "XML\XMLTreeNode.h"
#include "Cinematics\Cinematic.h"
#include "Particles\ParticleSystemInstance.h"
#include "Engine\Engine.h"
#include "GameObject\GameObjectManager.h"

CRenderableObjectsManager* CLayerManager::GetLayer(CXMLTreeNode &Node)
{
	return GetResource(Node.GetPszProperty("layer",""));
}

CRenderableObjectsManager* CLayerManager::AddLayer(CXMLTreeNode &TreeNode)
{
	CRenderableObjectsManager *l_ROManager = new CRenderableObjectsManager();
	
	std::string l_LayerName=TreeNode.GetPszProperty("name","");
	if(!AddResource(l_LayerName, l_ROManager))
	{
		if (l_ROManager != NULL) delete(l_ROManager); l_ROManager = NULL;
	}
	else 
	{
		l_ROManager->SetName(l_LayerName);
		bool l_Default=TreeNode.GetBoolProperty("default",false);
		if(l_Default)		
			m_DefaultLayer=l_ROManager;
	}

	return l_ROManager;
}

CLayerManager::CLayerManager()
:m_DefaultLayer(nullptr)
{

}

CLayerManager::~CLayerManager()
{

}

void CLayerManager::AddMeshComponent(const std::string &Layer, std::string &CoreMeshName, const std::string &InstanceMeshName, CGameObject* Owner, const Vect3f &Position, float Yaw, float Pitch, float Roll)
{
	CMeshInstance* l_RObject = new CMeshInstance(Owner, InstanceMeshName, CoreMeshName, Position, Yaw, Pitch, Roll);
	if (!AddElementToLayer(Layer, l_RObject))
	{
		assert(false);
		delete(l_RObject); l_RObject = NULL;
	}
	else
	{
		Owner->SetRenderableObject(l_RObject);
	}
}

void CLayerManager::AddAnimatedComponent(const std::string &Layer, const std::string &CoreModelName, const std::string &InstanceModelName, CGameObject* Owner, const Vect3f &Position, float Yaw, float Pitch, float Roll)
{
	CAnimatedInstanceModel* l_RObject = new CAnimatedInstanceModel(Owner, InstanceModelName, CoreModelName, Position, Yaw, Pitch, Roll);
	if (!AddElementToLayer(Layer, l_RObject))
	{
		assert(false);
		delete(l_RObject); l_RObject = NULL;
	}
	else
	{
		Owner->SetRenderableObject(l_RObject);
	}
}

/*Creates a GameObject and attachs it a ParticleSystemInstance*/
void CLayerManager::AddParticleSystemInstance(const std::string &Layer, const std::string &ParticleSystemTypeName, const std::string &ParticleSystemInstanceName, const Vect3f &Position, float Yaw, float Pitch, float Roll)
{
	CGameObject* l_GameObject = CEngine::GetSingleton().GetGameObjectManager()->AddGameObject(ParticleSystemInstanceName);
	m_ResourcesMap[Layer].m_Value->AddParticleSystemInstance(ParticleSystemTypeName, ParticleSystemInstanceName, l_GameObject, Position, Yaw, Pitch, Roll);
	
	/*if (!AddElementToLayer(Layer, l_ParticleSystemInstance))
	{
		assert(false);
		delete(l_ParticleSystemInstance); l_ParticleSystemInstance = NULL;
	}*/
	/*else
	{
		l_GameObject->SetRenderableObject(l_ParticleSystemInstance);
	}*/
}

void CLayerManager::Load(const std::string &Filename)
{
	m_Filename=Filename;
	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(Filename.c_str()))
	{
		CXMLTreeNode l_RenderableObjects = l_XML["renderable_objects"];

		if (l_RenderableObjects.Exists())
		{
			CGameObjectManager* l_GOManager = CEngine::GetSingleton().GetGameObjectManager();

			for (int i = 0; i < l_RenderableObjects.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_RenderableObjects(i);

				if (l_Element.GetName() == std::string("layer"))
				{
					AddLayer(l_Element);
				}
				else if (l_Element.GetName() == std::string("game_object"))
				{
					CGameObject* l_GameObject = l_GOManager->AddGameObject(l_Element);
				}
				else if (l_Element.GetName() == std::string("mesh_instance"))
				{
					CGameObject* l_GameObject = l_GOManager->AddGameObject(l_Element);
					if (l_GameObject!=NULL)
					{
						CMeshInstance *l_MeshInstance = new CMeshInstance(l_Element);
						CRenderableObjectsManager* l_Layer = GetLayer(l_Element);

						if (!l_Layer) l_Layer = m_DefaultLayer; 

						if (!l_Layer->AddResource(l_MeshInstance->GetName(), l_MeshInstance))
						{
							if (l_MeshInstance != NULL) delete(l_MeshInstance); l_MeshInstance = NULL;
						}
						else
						{
							l_GameObject->SetRenderableObject(l_MeshInstance);
							l_MeshInstance->GeneratePhysxActor(l_Element);
						}
					}	
				}
				else if (l_Element.GetName() == std::string("animated_model"))
				{
					CGameObject* l_GameObject = l_GOManager->AddGameObject(l_Element);
					if (l_GameObject != NULL)
					{
						CAnimatedInstanceModel* l_AnimatedInstance = new CAnimatedInstanceModel(l_Element);
						CRenderableObjectsManager* l_Layer = GetLayer(l_Element);

						if (!l_Layer) l_Layer = m_DefaultLayer;

						if (!l_Layer->AddResource(l_AnimatedInstance->GetName(), l_AnimatedInstance))
						{
							if (l_AnimatedInstance != NULL) delete(l_AnimatedInstance); l_AnimatedInstance = NULL;
						}
						else 
						{
							l_GameObject->SetRenderableObject(l_AnimatedInstance);

							if (l_Element.GetBoolProperty("player"))
								CEngine::GetSingleton().GetGameObjectManager()->SetPlayer(l_GameObject);
						}
					}
				}
				else if (l_Element.GetName() == std::string("cinematic"))
				{

					CGameObject* l_GameObject = l_GOManager->AddGameObject(l_Element);
					if (l_GameObject != NULL)
					{
						CRenderableObjectsManager* l_Layer = GetLayer(l_Element);
						if (!l_Layer) l_Layer = m_DefaultLayer;

						CCinematic *l_Cinematic = new CCinematic(l_Element);
						l_Cinematic->LoadXML(l_Element.GetPszProperty("file"));

						if (!l_Layer->AddResource(l_Cinematic->GetName(), l_Cinematic))
						{
							if (l_Cinematic != NULL) delete(l_Cinematic); l_Cinematic = NULL;
						}
						else
						{
							l_GameObject->SetRenderableObject(l_Cinematic);
						}
					}
				}
				else if (l_Element.GetName() == std::string("particle_emiter"))
				{
					CGameObject* l_GameObject = l_GOManager->AddGameObject(l_Element);
					if (l_GameObject != NULL)
					{
						CParticleSystemInstance* l_ParticleSystemInstance = new CParticleSystemInstance(l_Element);

						CRenderableObjectsManager* l_Layer = GetLayer(l_Element);

						if (!l_Layer)
						{
							l_Layer = m_DefaultLayer;
						}

						if (!l_Layer->AddResource(l_ParticleSystemInstance->GetName(), l_ParticleSystemInstance))
						{
							if (l_ParticleSystemInstance != NULL) delete(l_ParticleSystemInstance); l_ParticleSystemInstance = NULL;
						}
						else
						{
							l_GameObject->SetRenderableObject(l_ParticleSystemInstance);
						}
					}
				}
			}
		}
	}
}

void CLayerManager::Reload()
{
	m_DefaultLayer=NULL;

	Destroy();
	Load(m_Filename);
}

void CLayerManager::Update(float ElapsedTime)
{
	for(size_t i=0;i<m_ResourcesVector.size();++i)
	{
		m_ResourcesVector[i]->Update(ElapsedTime);
	}
}

void CLayerManager::Render(CRenderManager &RenderManager)
{
	for(size_t i=0;i<m_ResourcesVector.size();++i)
	{
		m_ResourcesVector[i]->Render(&RenderManager);
	}
}

void CLayerManager::Render(CRenderManager &RenderManager, const std::string &LayerName)
{
	m_ResourcesMap[LayerName].m_Value->Render(&RenderManager);
}

bool CLayerManager::AddElementToLayer(const std::string &Layer, CRenderableObject* RenderableObject)
{
	CRenderableObjectsManager* l_Layer = GetResource(Layer);

	if (!l_Layer)
	{
		l_Layer = m_DefaultLayer;
	}

	if (!l_Layer->AddResource(RenderableObject->GetName(), RenderableObject))
	{
		if (RenderableObject != NULL) delete(RenderableObject); RenderableObject = NULL;
		return false;
	}

	return true;
}