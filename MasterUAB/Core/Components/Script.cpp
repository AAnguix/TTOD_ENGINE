#include "Components\Script.h"
#include "Engine.h"
#include "ScriptManager\ScriptManager.h"
#include <luabind/luabind.hpp>
#include "Components\ComponentManager.h"
#include "Engine.h"
#include "Log.h"
#include "RenderableObjects\RenderableObject.h"

CScript::CScript(const std::string &Name, CRenderableObject *Owner, const std::string &FnOnCreate, const std::string &FnOnDestroy, const std::string &FnOnUpdate, const std::string &FnOnRender, const std::string &FnOnRenderDebug)
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
		CEngine::GetSingleton().GetLogManager()->Log("Error in function: " + m_FnOnCreate + ". " + std::string(e.what()));
		//luabind::object error_msg(luabind::from_stack(e.state(), -1));
		//CEngine::GetSingleton().GetLogManager()->Log(error_msg);
	}
}

CScript::~CScript()
{
	try
	{
		if(!m_FnOnDestroy.empty())
			luabind::call_function<void>(CEngine::GetSingleton().GetScriptManager()->GetLuaState(),
			m_FnOnDestroy.c_str(), m_Owner);
	}
	catch(const luabind::error &e)
	{
		CEngine::GetSingleton().GetLogManager()->Log("Error in function: " + m_FnOnDestroy + ". " + std::string(e.what()));
	}
}

void CScript::Update(float ElapsedTime)
{
	try
	{
		if(!m_FnOnUpdate.empty())
			luabind::call_function<void>(CEngine::GetSingleton().GetScriptManager()->GetLuaState(),
			m_FnOnUpdate.c_str(), m_Owner, ElapsedTime);
	}
	catch(const luabind::error &e)
	{
		CEngine::GetSingleton().GetLogManager()->Log("Error in function: " + m_FnOnUpdate + ". " + std::string(e.what()));
		//ShowLuabindError(e);
	}
}
	
void CScript::Render(CRenderManager &RenderManager)
{
	try
	{
		if(!m_FnOnRender.empty())
			luabind::call_function<void>(CEngine::GetSingleton().GetScriptManager()->GetLuaState(),
			m_FnOnRender.c_str(), m_Owner, RenderManager);
	}
	catch(const luabind::error &e)
	{
		CEngine::GetSingleton().GetLogManager()->Log("Error in function: " + m_FnOnRender + ". " + std::string(e.what()));
	}

}

void CScript::RenderDebug(CRenderManager &RenderManager)
{

}

CScript* CScript::AddScript(const std::string &Name, CRenderableObject *Owner, const std::string &FnOnCreate, const std::string &FnOnDestroy, const std::string &FnOnUpdate, const std::string &FnOnRender, const std::string &FnOnRenderDebug)
{
	CScript* l_Script = new CScript(Name, Owner, FnOnCreate, FnOnDestroy, FnOnUpdate, FnOnRender, FnOnRenderDebug);

	if(!Owner->AddComponent(l_Script))
	{
		CHECKED_DELETE(l_Script);
	}

	return l_Script;
}