#include "Components\ScriptedComponent.h"
#include "Engine.h"
#include "ScriptManager\ScriptManager.h"
#include <luabind/luabind.hpp>
#include "Components\ComponentManager.h"
#include "Engine.h"
#include "Log.h"
#include "RenderableObjects\RenderableObject.h"

CScriptedComponent::CScriptedComponent(const std::string &Name, CRenderableObject *Owner, const std::string &FnOnCreate, const std::string &FnOnDestroy, const std::string &FnOnUpdate, const std::string &FnOnRender, const std::string &FnOnRenderDebug)
	: CComponent(Name,Owner)
{
	m_FnOnCreate=FnOnCreate;
	m_FnOnDestroy=FnOnDestroy;
	m_FnOnUpdate=FnOnUpdate;
	m_FnOnRender=FnOnRender;
	m_FnOnRenderDebug=FnOnRenderDebug;

	try
	{
		if(!m_FnOnCreate.empty())
			luabind::call_function<void>(CEngine::GetSingleton().GetScriptManager()->GetLuaState(),
			m_FnOnCreate.c_str(), m_Owner);
	}
	catch(const luabind::error &e)
	{
		//luabind::object error_msg(luabind::from_stack(e.state(), -1));
		//CEngine::GetSingleton().GetLogManager()->Log(error_msg);
	}
}

CScriptedComponent::~CScriptedComponent()
{
	try
	{
		if(!m_FnOnDestroy.empty())
			luabind::call_function<void>(CEngine::GetSingleton().GetScriptManager()->GetLuaState(),
			m_FnOnDestroy.c_str(), m_Owner);
	}
	catch(const luabind::error &e)
	{
	}
}

void CScriptedComponent::Update(float ElapsedTime)
{
	try
	{
		if(!m_FnOnUpdate.empty())
			luabind::call_function<void>(CEngine::GetSingleton().GetScriptManager()->GetLuaState(),
			m_FnOnUpdate.c_str(), m_Owner, ElapsedTime);
	}
	catch(const luabind::error &e)
	{
		//ShowLuabindError(e);
	}
}
	
void CScriptedComponent::Render(CRenderManager &RenderManager)
{
	try
	{
		if(!m_FnOnRender.empty())
			luabind::call_function<void>(CEngine::GetSingleton().GetScriptManager()->GetLuaState(),
			m_FnOnRender.c_str(), m_Owner, RenderManager);
	}
	catch(const luabind::error &e)
	{
	}

}

void CScriptedComponent::RenderDebug(CRenderManager &RenderManager)
{

}

CScriptedComponent* CScriptedComponent::CreateScriptedComponent(const std::string &Name, CRenderableObject *Owner, const std::string &FnOnCreate, const std::string &FnOnDestroy, const std::string &FnOnUpdate, const std::string &FnOnRender, const std::string &FnOnRenderDebug)
{
	CScriptedComponent* l_SComponent = new CScriptedComponent(Name,Owner,FnOnCreate,FnOnDestroy,FnOnUpdate,FnOnRender,FnOnRenderDebug);

	if(!Owner->AddComponent(l_SComponent))
	{
		CHECKED_DELETE(l_SComponent);
	}

	return l_SComponent;
}