#include "State.h"
#include "Components\AnimatorController\AnimatorController.h"
#include <luabind/luabind.hpp>
#include "Engine.h"
#include "ScriptManager\ScriptManager.h"
#include "Animation\AnimatedInstanceModel.h"
//#include "RenderableObjects\RenderableObject.h"
#include "Transition.h"
#include "Log.h"

CState::CState(CAnimatorController* AnimatorController, const std::string &Name, const EAnimation &Animation, float Speed, const std::string &OnEnter, const std::string &OnUpdate, const std::string &OnExit)
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
	std::map<const std::string, CTransition*>::iterator itMap;

	for (itMap = m_Transitions.begin(); itMap != m_Transitions.end(); ++itMap)
	{
		delete itMap->second;
	}
	
	m_Transitions.clear();
}

void CState::OnEnter(CTransition* Transition)
{
	float l_DelayIn = Transition != nullptr ? Transition->GetDelayIn() : 0.0f;

	if (m_Animation.m_Loop)
	{
		((CAnimatedInstanceModel*)(m_AnimatorController->GetOwner()))->BlendCycle(m_Animation.m_ID, m_Animation.m_Weight, l_DelayIn);
	}
	else
	{
		float l_DelayOut = Transition != nullptr ? Transition->GetDelayOut() : 0.0f;
		((CAnimatedInstanceModel*)(m_AnimatorController->GetOwner()))->ExecuteAction(m_Animation.m_ID, l_DelayIn, l_DelayOut, m_Animation.m_Weight, false);
	}

	try
	{
		if (!m_OnEnter.empty())
		{
			CLUAComponent* l_LuaComponent = m_AnimatorController->GetOwner()->GetFirstLuaComponent();
			assert(l_LuaComponent!=nullptr);
			luabind::call_function<void>(CEngine::GetSingleton().GetScriptManager()->GetLuaState(), m_OnEnter.c_str(), l_LuaComponent);
		}
	}
	catch (const luabind::error &e)
	{
		CEngine::GetSingleton().GetLogManager()->Log("Error in function: " + m_OnEnter +". "+ std::string(e.what()));
	}
}

void CState::OnExit(CTransition* Transition)
{
	if (m_Animation.m_Loop)
	{
		float l_DelayOut = Transition != nullptr ? Transition->GetDelayOut() : 0.0f;
		((CAnimatedInstanceModel*)(m_AnimatorController->GetOwner()))->ClearCycle(m_Animation.m_ID, l_DelayOut);
	}

	try
	{
		if (!m_OnExit.empty())
		{
			CLUAComponent* l_LuaComponent = m_AnimatorController->GetOwner()->GetFirstLuaComponent();
			assert(l_LuaComponent != nullptr);
			luabind::call_function<void>(CEngine::GetSingleton().GetScriptManager()->GetLuaState(), m_OnExit.c_str(), l_LuaComponent); //(CAnimatedInstanceModel*)m_AnimatorController->GetOwner()
		}
	}
	catch (const luabind::error &e)
	{
		//luabind::object error_msg(luabind::from_stack(e.state(), -1));
		//CEngine::GetSingleton().GetLogManager()->Log(error_msg);
		CEngine::GetSingleton().GetLogManager()->Log("Error in function: " + m_OnExit + ". " + std::string(e.what()));
	}
}

CTransition* CState::AddTransition(const std::string &Name, CState* NewState, bool HasExitTime, float ExitTime, float DelayIn, float DelayOut)
{
	CTransition* l_Transition = new CTransition(NewState, HasExitTime, ExitTime, DelayIn, DelayOut);
	m_Transitions.insert(std::pair<const std::string, CTransition*>(Name, l_Transition));
	return l_Transition;
}

void CState::OnUpdate(float ElapsedTime)
{
	try
	{
		if (!m_OnUpdate.empty())
		{
			CLUAComponent* l_LuaComponent = m_AnimatorController->GetOwner()->GetFirstLuaComponent();
			assert(l_LuaComponent != nullptr);
			luabind::call_function<void>(CEngine::GetSingleton().GetScriptManager()->GetLuaState(), m_OnUpdate.c_str(), l_LuaComponent, ElapsedTime);
		}
	}
	catch (const luabind::error &e)
	{
		//luabind::object error_msg(luabind::from_stack(e.state(), -1));
		
		CEngine::GetSingleton().GetLogManager()->Log("Error in function: " + m_OnUpdate + ". " + std::string(e.what()));
	}

	std::map<const std::string, CTransition*>::iterator itMap;

	if (m_Transitions.empty())
	{
		if (!m_Animation.m_Loop)
		{
			m_AnimatorController->ChangeCurrentState(m_AnimatorController->GetPreviousState(), nullptr);
		}
	}
	else
	{
		for (itMap = m_Transitions.begin(); itMap != m_Transitions.end(); ++itMap)
		{
			if ((itMap->second->MeetsConditions()))
			{
				m_AnimatorController->ChangeCurrentState(itMap->second->GetNewState(), itMap->second);
			}
		}
	}
}

