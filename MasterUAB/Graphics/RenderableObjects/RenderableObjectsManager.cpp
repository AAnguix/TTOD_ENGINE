#include "RenderableObjects\RenderableObjectsManager.h"

CRenderableObjectsManager::CRenderableObjectsManager() : CNamed("")
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
	for (size_t i = 0; i<m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->Render(RM);
	}
}