#include "State.h"
#include "Components\AnimatorController\AnimatorController.h"
#include "GameObject\GameObject.h"
#include <luabind/luabind.hpp>
#include "Engine\Engine.h"
#include "LuabindManager\LuabindManager.h"
#include "Animation\AnimatedInstanceModel.h"
#include "Components\LuaComponent.h"
#include "Components\Script\Script.h"
#include "Transition.h"
#include "Log\Log.h"


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
		((CAnimatedInstanceModel*)(m_AnimatorController->GetOwner()->GetRenderableObject()))->BlendCycle(m_Animation.m_ID, m_Animation.m_Weight, l_DelayIn);
	}
	else
	{
		float l_DelayOut = Transition != nullptr ? Transition->GetDelayOut() : 0.0f;
		((CAnimatedInstanceModel*)(m_AnimatorController->GetOwner()->GetRenderableObject()))->ExecuteAction(m_Animation.m_ID, l_DelayIn, l_DelayOut, m_Animation.m_Weight, false);
	}

	try
	{
		if (!m_OnEnter.empty())
		{
			CScript* l_Script = m_AnimatorController->GetOwner()->GetScript();
			if (l_Script != nullptr)
			{
				CLUAComponent* l_LuaComponent = l_Script->GetLuaComponent();
				assert(l_LuaComponent != nullptr);
				l_LuaComponent->ResetTimer();
				luabind::call_function<void>(CEngine::GetSingleton().GetLuabindManager()->GetLuaState(), m_OnEnter.c_str(), l_LuaComponent);
			}
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
		((CAnimatedInstanceModel*)(m_AnimatorController->GetOwner()->GetRenderableObject()))->ClearCycle(m_Animation.m_ID, l_DelayOut);
	}

	try
	{
		if (!m_OnExit.empty())
		{
			CScript* l_Script = m_AnimatorController->GetOwner()->GetScript();
			if (l_Script != nullptr)
			{
				CLUAComponent* l_LuaComponent = l_Script->GetLuaComponent();
				assert(l_LuaComponent != nullptr);
				luabind::call_function<void>(CEngine::GetSingleton().GetLuabindManager()->GetLuaState(), m_OnExit.c_str(), l_LuaComponent);
			}
		}
	}
	catch (const luabind::error &e)
	{
		//luabind::object error_msg(luabind::from_stack(e.state(), -1));
		//CEngine::GetSingleton().GetLogManager()->Log(error_msg);
		CEngine::GetSingleton().GetLogManager()->Log("Error in function: " + m_OnExit + ". " + std::string(e.what()));
	}
}

CTransition* CState::AddTransition(const std::string &Name, CState* NewState, bool HasExitTime, float DelayIn, float DelayOut)
{
	#ifdef _DEBUG
	if (NewState->GetAnimation().m_Loop)
	{
		LOG("Trying to create a transition(" + Name + ") with out delay in a loop animation. ");
			assert(false);
	}
	#endif

	CTransition* l_Transition = new CTransition(NewState, HasExitTime, DelayIn, DelayOut);
	m_Transitions.insert(std::pair<const std::string, CTransition*>(Name, l_Transition));
	return l_Transition;
}

CTransition* CState::AddTransition(const std::string &Name, CState* NewState, bool HasExitTime, float DelayIn)
{
	#ifdef _DEBUG
	if (!NewState->GetAnimation().m_Loop)
	{
		LOG("Trying to create a transition(" + Name + ") without out delay in a non-loop animation. ");
		assert(false);
	}
	#endif
	
	CTransition* l_Transition = new CTransition(NewState, HasExitTime, DelayIn, 0.0f);
	m_Transitions.insert(std::pair<const std::string, CTransition*>(Name, l_Transition));
	return l_Transition;
}

void CState::OnUpdate(float ElapsedTime)
{
	float l_Timer = 0.0f;
	try
	{
		if (!m_OnUpdate.empty())
		{
			CScript* l_Script = m_AnimatorController->GetOwner()->GetScript();
			if (l_Script != nullptr)
			{
				CLUAComponent* l_LuaComponent = l_Script->GetLuaComponent();
				assert(l_LuaComponent != nullptr);
				l_LuaComponent->AddTime(ElapsedTime);
				l_Timer = l_LuaComponent->GetTimer();
				luabind::call_function<void>(CEngine::GetSingleton().GetLuabindManager()->GetLuaState(), m_OnUpdate.c_str(), l_LuaComponent, ElapsedTime);
			}
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
			CheckStateChange(false, l_Timer, m_AnimatorController->GetPreviousState(), nullptr);
		}
	}
	else
	{
		for (itMap = m_Transitions.begin(); itMap != m_Transitions.end(); ++itMap)
		{
			if (itMap->second->MeetsConditions())
			{
				CheckStateChange(itMap->second->GetHasExitTime(), l_Timer, itMap->second->GetNewState(), itMap->second);
			}
		}
	}
}

void CState::CheckStateChange(bool HasExitTime, float Timer, CState* NewState, CTransition* Transition)
{
	if (HasExitTime)
	{
		float l_AnimDuration = m_Animation.m_Duration - 0.075f;
		if (Timer >= l_AnimDuration)
		{
			m_AnimatorController->ChangeCurrentState(NewState, Transition);
		}
	}
	else
	{
		m_AnimatorController->ChangeCurrentState(NewState, Transition);
	}
}
