#include "LuaComponent.h"
#include "Utils\EmptyPointerClass.h"
#include "Log\Log.h"
#include "Engine\Engine.h"

CLUAComponent::CLUAComponent(const std::string Name) 
:m_Name(Name)
,m_Timer(0.0f)
,m_Enabled(true)
{}

const std::string &CLUAComponent::GetName()
{
	return m_Name;
}

void CLUAComponent::Update(float ElapsedTime)
{
};

CEmptyPointerClass* CLUAComponent::GetThisLuaAddress() const { return (CEmptyPointerClass *)((void*)this); };
CEmptyPointerClass* CLUAComponent::GetEnabledLuaAddress() const { return (CEmptyPointerClass *)&m_Enabled; }