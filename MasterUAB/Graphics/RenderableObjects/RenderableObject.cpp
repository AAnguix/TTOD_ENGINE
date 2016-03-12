#include "RenderableObject.h"
#include "Components\ComponentManager.h"
#include "Components\Component.h"

CRenderableObject::CRenderableObject() : CNamed("")
{
	m_ComponentManager= new CComponentManager();
}

CRenderableObject::~CRenderableObject()
{
	CHECKED_DELETE(m_ComponentManager);
}

void CRenderableObject::Update(float ElapsedTime)
{
	if(m_ComponentManager!=NULL)
	{
		m_ComponentManager->Update(ElapsedTime);
	}
}

bool CRenderableObject::AddComponent(CComponent *Component)
{
	return m_ComponentManager->AddComponent(Component);
}