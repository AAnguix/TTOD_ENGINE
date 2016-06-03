#include "Components\Script.h"
#include "RenderableObjects\RenderableObject.h"
#include "Components\LuaComponent.h"

CScript::CScript(const std::string& Name, CRenderableObject* Owner, CLUAComponent* Component)
:CComponent(Name, Owner)
,m_Component(Component)
{
}

CScript::~CScript()
{

}

void CScript::Update(float ElapsedTime)
{
	m_Component->Update(ElapsedTime);
}