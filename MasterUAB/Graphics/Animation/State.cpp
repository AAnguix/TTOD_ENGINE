#include "State.h"
#include "Animation\AnimatorController.h"
#include <luabind/luabind.hpp>
#include "Engine.h"
#include "ScriptManager\ScriptManager.h"
#include "RenderableObjects\RenderableObject.h"

CState::CState(CAnimatorController* AnimatorController, const std::string &Name, const std::string &Animation, const float &Speed, const std::string &OnEnter, const std::string &OnUpdate, const std::string &OnExit)
:m_AnimatorController(AnimatorController)
,m_Name(Name)
,m_Animation(Animation)
,m_Speed(Speed)
,m_OnEnter(OnEnter)
,m_OnUpdate(OnUpdate)
,m_OnExit(OnExit)
{
	
}

CState::~CState()
{
	std::map<const std::string, STransition*>::iterator itMap;

	for (itMap = m_Transitions.begin(); itMap != m_Transitions.end(); ++itMap)
	{
		delete itMap->second;
	}
	
	m_Transitions.clear();
}

void CState::OnEnter()
{
	try
	{
		if (!m_OnEnter.empty())
			luabind::call_function<void>(CEngine::GetSingleton().GetScriptManager()->GetLuaState(),
			m_OnEnter.c_str(), m_AnimatorController->GetOwner());
	}
	catch (const luabind::error &e)
	{
		//luabind::object error_msg(luabind::from_stack(e.state(), -1));
		//CEngine::GetSingleton().GetLogManager()->Log(error_msg);
	}
}

void CState::OnExit()
{
	try
	{
		if (!m_OnExit.empty())
			luabind::call_function<void>(CEngine::GetSingleton().GetScriptManager()->GetLuaState(),
			m_OnExit.c_str(), m_AnimatorController->GetOwner());
	}
	catch (const luabind::error &e)
	{
		//luabind::object error_msg(luabind::from_stack(e.state(), -1));
		//CEngine::GetSingleton().GetLogManager()->Log(error_msg);
	}
}

CState::STransition::STransition(CState* NewState, const bool &HasExitTime, const float &ExitTime)
:m_NewState(NewState)
,m_HasExitTime(HasExitTime)
,m_ExitTime(ExitTime)
{
}

CState::STransition::~STransition()
{
	for (size_t i = 0; i < m_Parameters.size(); ++i)
	{
		if (m_Parameters[i] != NULL)
		{
			delete m_Parameters[i];
			m_Parameters[i] = nullptr;
		}
	}
}

void CState::AddTransition(const std::string &Name, CState* NewState, const bool &HasExitTime, const float &ExitTime)
{
	STransition* l_Transition = new STransition(NewState, HasExitTime, ExitTime);
	m_Transitions.insert(std::pair<const std::string, STransition*>(Name,l_Transition));
}

void CState::OnUpdate(float ElapsedTime)
{
	try
	{
		if (!m_OnExit.empty())
			luabind::call_function<void>(CEngine::GetSingleton().GetScriptManager()->GetLuaState(),
			m_OnExit.c_str(), m_AnimatorController->GetOwner());
	}
	catch (const luabind::error &e)
	{
		//luabind::object error_msg(luabind::from_stack(e.state(), -1));
		//CEngine::GetSingleton().GetLogManager()->Log(error_msg);
	}

	std::map<const std::string, STransition*>::iterator itMap;

	for (itMap = m_Transitions.begin(); itMap != m_Transitions.end(); ++itMap)
	{
		if (TransitionMeetsConditions(itMap->second))
		{
			m_AnimatorController->ChangeCurrentState(this);
		}
	}
}

bool CState::TransitionMeetsConditions(CState::STransition* Transition)
{
	bool m_MeetsConditions = true;

	for (size_t i = 0; i < Transition->m_Parameters.size(); ++i)
	{
		if (!Transition->m_Parameters[i]->Launch())
		{
			m_MeetsConditions = false;
		}	
	}

	return m_MeetsConditions;
}