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

/*
Generates a state that has one animation.
*/
CState::CState(CAnimatorController* AnimatorController, const std::string &Name, const EAnimation &Animation, float Speed, const std::string &OnEnter, const std::string &OnUpdate, const std::string &OnExit)
:m_AnimatorController(AnimatorController)
,m_Name(Name)
,m_Speed(Speed)
,m_OnEnter(OnEnter)
,m_OnUpdate(OnUpdate)
,m_OnExit(OnExit)
,m_CurrentAnimationIndex(0)
,m_RestartAnimationsTime(0.0f)
,m_LastTimeSinceStart(CEngine::GetSingleton().GetRealTimeSinceStartup())
{
	m_Animations.push_back(Animation);
}

/*
Generates a state that has an array animations.
*/
CState::CState(CAnimatorController* AnimatorController, const std::string &Name, const std::vector<EAnimation> &Animations, float RestartAnimationsTime, float Speed, const std::string &OnEnter, const std::string &OnUpdate, const std::string &OnExit)
:m_AnimatorController(AnimatorController)
,m_Name(Name)
,m_Speed(Speed)
,m_OnEnter(OnEnter)
,m_OnUpdate(OnUpdate)
,m_OnExit(OnExit)
,m_CurrentAnimationIndex(0)
,m_RestartAnimationsTime(RestartAnimationsTime)
,m_LastTimeSinceStart(CEngine::GetSingleton().GetRealTimeSinceStartup())
{
	m_Animations = Animations;
}

CState::~CState()
{
	std::map<const std::string, CTransition*>::iterator itMap;

	for (itMap = m_Transitions.begin(); itMap != m_Transitions.end(); ++itMap)
	{
		delete itMap->second;
	}
	
	m_Transitions.clear();
	m_Animations.clear();
}

void CState::OnEnter(CTransition* Transition)
{
	float l_DelayIn = Transition != nullptr ? Transition->GetDelayIn() : 0.0f;
	
	unsigned int l_AnimationsCount = m_Animations.size();

	if (l_AnimationsCount > 1)
	{
		float l_TimeSinceStart = CEngine::GetSingleton().GetRealTimeSinceStartup();

		if (l_TimeSinceStart - m_LastTimeSinceStart > m_RestartAnimationsTime)
		{
			m_CurrentAnimationIndex = 0;
		}
		else
		{
			++m_CurrentAnimationIndex;
			if (m_CurrentAnimationIndex >= l_AnimationsCount)
				m_CurrentAnimationIndex = 0;
		}
		m_LastTimeSinceStart = l_TimeSinceStart;
	}

	if (m_Animations[m_CurrentAnimationIndex].m_Loop)
	{
		((CAnimatedInstanceModel*)(m_AnimatorController->GetOwner()->GetRenderableObject()))->BlendCycle(m_Animations[m_CurrentAnimationIndex].m_ID, m_Animations[m_CurrentAnimationIndex].m_Weight, l_DelayIn);
	}
	else
	{
		float l_DelayOut = Transition != nullptr ? Transition->GetDelayOut() : 0.0f;
		((CAnimatedInstanceModel*)(m_AnimatorController->GetOwner()->GetRenderableObject()))->ExecuteAction(m_Animations[m_CurrentAnimationIndex].m_ID, l_DelayIn, l_DelayOut, m_Animations[m_CurrentAnimationIndex].m_Weight, false);
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
	if (m_Animations[m_CurrentAnimationIndex].m_Loop)
	{
		float l_DelayOut = Transition != nullptr ? Transition->GetDelayOut() : 0.0f;
		((CAnimatedInstanceModel*)(m_AnimatorController->GetOwner()->GetRenderableObject()))->ClearCycle(m_Animations[m_CurrentAnimationIndex].m_ID, l_DelayOut);
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
		if (!m_Animations[m_CurrentAnimationIndex].m_Loop)
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
				bool l_Changued = CheckStateChange(itMap->second->GetHasExitTime(), l_Timer, itMap->second->GetNewState(), itMap->second);
				if (l_Changued || CEngine::GetSingleton().LoadingLevel())
				{
					break;
				}
			}
		}
	}
}

bool CState::CheckStateChange(bool HasExitTime, float Timer, CState* NewState, CTransition* Transition)
{
	if (HasExitTime)
	{
		float l_AnimDuration = m_Animations[m_CurrentAnimationIndex].m_Duration - 0.075f;
		if (Timer >= l_AnimDuration)
		{
			m_AnimatorController->ChangeCurrentState(NewState, Transition);
			return true;
		}
		return false;
	}
	else
	{
		m_AnimatorController->ChangeCurrentState(NewState, Transition);
		return true;
	}
}
