#include "Components\Script.h"
#include "RenderableObjects\RenderableObject.h"
#include "Components\LuaComponent.h"
#include <cassert>

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
	if (m_Component == nullptr)
	{
		assert(false);
	}
	else
	{
		m_Component->Update(ElapsedTime);
	}
}