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