#include "RenderableObjects\RenderableObjectsManager.h"
#include "Particles\ParticleSystemInstance.h"
#include "GameObject\GameObject.h"

CRenderableObjectsManager::CRenderableObjectsManager() : CNamed("")
,m_ObjectsInsideFrustrum(0)
{
}

CRenderableObjectsManager::~CRenderableObjectsManager()
{

}

//void CRenderableObjectsManager::Reload()
//{
//	Destroy();
//	Load(m_Filename);
//}

bool CRenderableObjectsManager::RemoveRenderableObject(const std::string& RenderableObjectName)
{
	for (size_t i = 0; i < m_ResourcesVector.size(); ++i)
	{
		if (m_ResourcesVector[i]->GetName()==RenderableObjectName)
		{
			RemoveResource(RenderableObjectName);
			return true;
		}
	}
	return false;
}

/*
Remove RenderableObject from map and vector.
Caution with this method. It doesn't deletes the RenderableObject pointer.
Used to move a RenderableObject from one layer to another.
*/
void CRenderableObjectsManager::RemoveRenderableObjectFromContainers(const std::string& RenderableObjectName)
{
	CMapResourceValue l_ResourceValue = m_ResourcesMap[RenderableObjectName];
	size_t l_Index = l_ResourceValue.m_Id;

	m_ResourcesMap.erase(RenderableObjectName);
	m_ResourcesVector.erase(m_ResourcesVector.begin() + l_Index);
	for (TMapResources::iterator l_It = m_ResourcesMap.begin(); l_It != m_ResourcesMap.end(); l_It++)
	{
		if (l_It->second.m_Id>l_Index)
		{
			l_It->second.m_Id--;
		}
	}
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
	m_ObjectsInsideFrustrum = 0;
	for (size_t i = 0; i<m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->Render(RM);
	}
}

void CRenderableObjectsManager::IncrementObjects()
{
	m_ObjectsInsideFrustrum+=1;
};

void CRenderableObjectsManager::AddParticleSystemInstance(const std::string &ParticleSystemTypeName, const std::string &ParticleSystemInstanceName, CGameObject* GameObject, const Vect3f &Position, float Yaw, float Pitch, float Roll)
{
	CParticleSystemInstance* l_ParticleSystemInstance = new CParticleSystemInstance(ParticleSystemInstanceName, ParticleSystemTypeName, GameObject, Position, Yaw, Pitch, Roll);

	if (!AddResource(l_ParticleSystemInstance->GetName(), l_ParticleSystemInstance))
	{
		if (l_ParticleSystemInstance != NULL) delete(l_ParticleSystemInstance); l_ParticleSystemInstance = NULL;
	}
	else
	{
		l_ParticleSystemInstance->Disable();
		//GameObject->SetRenderableObject(l_ParticleSystemInstance);
	}
}