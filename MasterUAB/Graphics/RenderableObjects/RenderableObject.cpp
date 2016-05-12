#include "RenderableObject.h"
#include "Components\ComponentManager.h"
#include "Components\Component.h"
#include <string>
#include "Components\AnimatorController\AnimatorController.h"

CRenderableObject::CRenderableObject(const CXMLTreeNode &XMLTreeNode) 
:CNamed(XMLTreeNode)
,C3DElement(XMLTreeNode)
{
	m_ComponentManager= new CComponentManager();
}

CRenderableObject::CRenderableObject(const std::string &Name, const Vect3f &Position, float Yaw, float Pitch, float Roll)
:CNamed(Name)
,C3DElement(Position,Yaw,Pitch,Roll)
{

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

CAnimatorController* CRenderableObject::GetAnimatorController() const
{
	return (CAnimatorController*)m_ComponentManager->GetResource("AnimatorController");
}

bool CRenderableObject::AddLuaComponent(CLUAComponent* LUAComponent)
{
	m_LuaComponents.push_back(LUAComponent);
	return true;
}

CLUAComponent* CRenderableObject::GetFirstLuaComponent() const
{
	CLUAComponent* l_FirstComponent = m_LuaComponents.size() > 0 ? m_LuaComponents[0]:nullptr;
	return l_FirstComponent;
}
	