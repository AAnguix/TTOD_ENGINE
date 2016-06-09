#include "LuaComponent.h"
#include "Utils\EmptyPointerClass.h"

CLUAComponent::CLUAComponent(const std::string Name) 
:m_Name(Name)
,m_Timer(0.0f)
{}
const std::string &CLUAComponent::GetName()
{
	return m_Name;
}

CEmptyPointerClass* CLUAComponent::GetThisLuaAddress() const { return (CEmptyPointerClass *)((void*)this); };
CEmptyPointerClass* CLUAComponent::GetEnabledLuaAddress() const { return (CEmptyPointerClass *)&m_Enabled; }