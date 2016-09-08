#include "Components\Script\Script.h"
#include "Components\LuaComponent.h"
#include "Log\Log.h"
#include "Engine\Engine.h"
#include <cassert>

CScript::CScript(const std::string& Name, CGameObject* Owner, CLUAComponent* Component)
:CComponent(Name, Owner)
,m_LuaComponent(Component)
{
}

CScript::~CScript()
{
	//m_LuaComponent = nullptr;
}

void CScript::Update(float ElapsedTime)
{
	m_LuaComponent->Update(ElapsedTime);
}