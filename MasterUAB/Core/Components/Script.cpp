#include "Components\Script.h"
#include "Components\LuaComponent.h"
#include <cassert>

CScript::CScript(const std::string& Name, CGameObject* Owner, CLUAComponent* Component)
:CComponent(Name, Owner)
,m_LuaComponent(Component)
{
}

CScript::~CScript()
{
	m_LuaComponent = nullptr;
}

void CScript::Update(float ElapsedTime)
{
	if (m_LuaComponent == nullptr)
	{
		assert(false);
	}
	else
	{
		m_LuaComponent->Update(ElapsedTime);
	}
}