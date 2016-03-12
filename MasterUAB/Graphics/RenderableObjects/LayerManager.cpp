#include "LayerManager.h"
#include "RenderableObjects\MeshInstance.h"
#include "Animation\AnimatedInstanceModel.h"
#include "Cinematics\Cinematic.h"
#include "Particles\ParticleSystemInstance.h"

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
		CHECKED_DELETE(l_ROManager);
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
{

}

CLayerManager::~CLayerManager()
{
	
}
/*
void CLayerManager::Destroy()
{
	
}*/

void CLayerManager::Load(const std::string &Filename)
{
	m_Filename=Filename;
	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(Filename.c_str()))
	{
		CXMLTreeNode l_RenderableObjects = l_XML["renderable_objects"];

		if (l_RenderableObjects.Exists())
		{
			for (int i = 0; i < l_RenderableObjects.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_RenderableObjects(i);

				if (l_Element.GetName() == std::string("layer"))
				{
					AddLayer(l_Element);
				}

				if (l_Element.GetName() == std::string("mesh_instance"))
				{
					CMeshInstance *l_MeshInstance = new CMeshInstance(l_Element);

					CRenderableObjectsManager* l_Layer=GetLayer(l_Element);

					if(!l_Layer)
					{
						l_Layer=m_DefaultLayer;
					}

					if(!l_Layer->AddResource(l_MeshInstance->GetName(), l_MeshInstance))
					{
						CHECKED_DELETE(l_MeshInstance);
					}
					
				}
				else if (l_Element.GetName() == std::string("animated_model"))
				{
					CAnimatedInstanceModel* l_AnimatedInstance = new CAnimatedInstanceModel(l_Element);
					
					CRenderableObjectsManager* l_Layer=GetLayer(l_Element);

					if(!l_Layer)
					{
						l_Layer=m_DefaultLayer;
					}

					if(!l_Layer->AddResource(l_AnimatedInstance->GetName(), l_AnimatedInstance))
					{
						CHECKED_DELETE(l_AnimatedInstance);
					}
					else if (l_Element.GetBoolProperty("player"))
					{
						m_Player = l_AnimatedInstance;
					}	
				}
				else if (l_Element.GetName() == std::string("cinematic"))
				{
					CRenderableObjectsManager* l_Layer=GetLayer(l_Element);

					if(!l_Layer)
					{
						l_Layer=m_DefaultLayer;
					}

					CCinematic *l_Cinematic = new CCinematic(l_Element);
					l_Cinematic->LoadXML(l_Element.GetPszProperty("file"));

					if(!l_Layer->AddResource(l_Cinematic->GetName(),l_Cinematic))
					{
						CHECKED_DELETE(l_Cinematic);
					}
				}
				else if (l_Element.GetName() == std::string("particle_emiter"))
				{
					CParticleSystemInstance* l_ParticleSystemInstance = new CParticleSystemInstance(l_Element);

					CRenderableObjectsManager* l_Layer = GetLayer(l_Element);

					if (!l_Layer)
					{
						l_Layer = m_DefaultLayer;
					}

					if (!l_Layer->AddResource(l_ParticleSystemInstance->GetName(), l_ParticleSystemInstance))
					{
						CHECKED_DELETE(l_ParticleSystemInstance);
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

CAnimatedInstanceModel* CLayerManager::GetPlayer() const
{
	return m_Player;
}