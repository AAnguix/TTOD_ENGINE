#include "Components\ComponentManager.h"

CComponentManager::CComponentManager()
{

}

CComponentManager::~CComponentManager()
{

}

void CComponentManager::Update(float ElapsedTime)
{
	for(size_t i=0;i<m_ResourcesVector.size();++i)
	{
		m_ResourcesVector[i]->Update(ElapsedTime);
	}
}

void CComponentManager::Render(CRenderManager &RenderManager)
{
	for(size_t i=0;i<m_ResourcesVector.size();++i)
	{
		m_ResourcesVector[i]->Render(RenderManager);
	}
}

void CComponentManager::RenderDebug(CRenderManager &RenderManager)
{
	for(size_t i=0;i<m_ResourcesVector.size();++i)
	{
		m_ResourcesVector[i]->RenderDebug(RenderManager);
	}
}

bool CComponentManager::AddComponent(CComponent *Component)
{
	if(!AddResource(Component->GetName(),Component))
	{
		return false;
	} 
	
	return true;
}

