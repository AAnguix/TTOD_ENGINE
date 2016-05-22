#include "Transition.h"
#include "State.h"
#include "AnimatorController.h"

CTransition::CTransition(CState* NewState, bool HasExitTime, float ExitTime, float DelayIn, float DelayOut)
:m_NewState(NewState)
,m_HasExitTime(HasExitTime)
,m_ExitTime(ExitTime)
,m_DelayIn(DelayIn)
,m_DelayOut(DelayOut)
{
}

CTransition::~CTransition()
{
	for (size_t i = 0; i < m_IntegerConditions.size(); ++i)
	{
		if (m_IntegerConditions[i] != NULL)
		{
			delete m_IntegerConditions[i];
			m_IntegerConditions[i] = nullptr;
		}
	}
	for (size_t i = 0; i < m_FloatConditions.size(); ++i)
	{
		if (m_FloatConditions[i] != NULL)
		{
			delete m_FloatConditions[i];
			m_FloatConditions[i] = nullptr;
		}
	}
	for (size_t i = 0; i < m_BoolConditions.size(); ++i)
	{
		if (m_BoolConditions[i] != NULL)
		{
			delete m_BoolConditions[i];
			m_BoolConditions[i] = nullptr;
		}
	}
	for (size_t i = 0; i < m_TriggerConditions.size(); ++i)
	{
		if (m_TriggerConditions[i] != NULL)
		{
			delete m_TriggerConditions[i];
			m_TriggerConditions[i] = nullptr;
		}
	}
}

CTransition::EFloatCondition::EFloatCondition(const std::string &ParameterName, float TriggerValue, EFloatConditionType ConditionType)
:m_ParameterName(ParameterName), m_TriggerValue(TriggerValue), m_Type(ConditionType)
{
}

CTransition::EIntegerCondition::EIntegerCondition(const std::string &ParameterName, int TriggerValue, EIntegerConditionType ConditionType)
:m_ParameterName(ParameterName), m_TriggerValue(TriggerValue), m_Type(ConditionType)
{
}

CTransition::EBoolCondition::EBoolCondition(const std::string &ParameterName, bool TriggerValue)
:m_ParameterName(ParameterName), m_TriggerValue(TriggerValue)
{
}

CTransition::ETriggerCondition::ETriggerCondition(const std::string &ParameterName)
:m_ParameterName(ParameterName)
{
}

void CTransition::AddFloatCondition(const std::string &ParameterName, float TriggerValue, EFloatCondition::EFloatConditionType TriggerConditionType)
{
	CTransition::EFloatCondition* l_FloatCondition = new CTransition::EFloatCondition(ParameterName, TriggerValue, TriggerConditionType);
	m_FloatConditions.push_back(l_FloatCondition);
}
void CTransition::AddIntegerCondition(const std::string &ParameterName, int TriggerValue, EIntegerCondition::EIntegerConditionType TriggerConditionType)
{
	EIntegerCondition* l_IntegerCondition = new EIntegerCondition(ParameterName, TriggerValue, TriggerConditionType);
	m_IntegerConditions.push_back(l_IntegerCondition);
}
void CTransition::AddBoolCondition(const std::string &ParameterName, bool TriggerValue)
{
	EBoolCondition* l_BoolCondition = new EBoolCondition(ParameterName, TriggerValue);
	m_BoolConditions.push_back(l_BoolCondition);
}
void CTransition::AddTriggerCondition(const std::string &ParameterName)
{
	ETriggerCondition* l_TriggerCondition = new ETriggerCondition(ParameterName);
	m_TriggerConditions.push_back(l_TriggerCondition);
}

bool CTransition::MeetsConditions()
{
	if (MeetsIntegerConditions() && MeetsFloatConditions() && MeetsBoolConditions() && MeetsTriggerConditions())
	{
		ClearTriggers();
		return true;
	}
	return false;
}

bool CTransition::MeetsIntegerConditions()
{
	for (unsigned int i = 0; i < m_IntegerConditions.size(); ++i)
	{
		CAnimatorControllerIntegerParameter* l_Parameter = ((CAnimatorControllerIntegerParameter*)m_NewState->GetOwnAnimatorController()->GetParameter(m_IntegerConditions[i]->m_ParameterName));

		if (m_IntegerConditions[i]->m_Type == EIntegerCondition::GREATER && m_IntegerConditions[i]->m_TriggerValue > l_Parameter->GetValue())
		{
			return false;
		}
		else if (m_IntegerConditions[i]->m_Type == EIntegerCondition::LESS && m_IntegerConditions[i]->m_TriggerValue < l_Parameter->GetValue())
		{
			return false;
		}
		else if (m_IntegerConditions[i]->m_Type == EIntegerCondition::EQUALS && m_IntegerConditions[i]->m_TriggerValue != l_Parameter->GetValue())
		{
			return false;
		}
		else if (m_IntegerConditions[i]->m_Type == EIntegerCondition::NOT_EQUAL && m_IntegerConditions[i]->m_TriggerValue == l_Parameter->GetValue())
		{
			return false;
		}
	}

	return true;
}
bool CTransition::MeetsFloatConditions()
{
	for (unsigned int i = 0; i < m_FloatConditions.size(); ++i)
	{
		CAnimatorControllerFloatParameter* l_Parameter = ((CAnimatorControllerFloatParameter*)m_NewState->GetOwnAnimatorController()->GetParameter(m_FloatConditions[i]->m_ParameterName));

		if (m_FloatConditions[i]->m_Type == EFloatCondition::GREATER && m_FloatConditions[i]->m_TriggerValue > l_Parameter->GetValue())
		{
			return false;
		}
		else if (m_FloatConditions[i]->m_Type == EFloatCondition::LESS && m_FloatConditions[i]->m_TriggerValue < l_Parameter->GetValue())
		{
			return false;
		}
	}

	return true;
}
bool CTransition::MeetsBoolConditions()
{
	for (unsigned int i = 0; i < m_BoolConditions.size(); ++i)
	{
		CAnimatorControllerBoolParameter* l_Parameter = ((CAnimatorControllerBoolParameter*)m_NewState->GetOwnAnimatorController()->GetParameter(m_BoolConditions[i]->m_ParameterName));

		if (m_BoolConditions[i]->m_TriggerValue != l_Parameter->GetValue())
		{
			return false;
		}
	}

	return true;
}
bool CTransition::MeetsTriggerConditions()
{
	for (unsigned int i = 0; i < m_TriggerConditions.size(); ++i)
	{
		CAnimatorControllerTriggerParameter* l_Parameter = ((CAnimatorControllerTriggerParameter*)m_NewState->GetOwnAnimatorController()->GetParameter(m_TriggerConditions[i]->m_ParameterName));

		if (!l_Parameter->Triggered())
		{
			return false;
		}
	}

	return true;
}

void CTransition::ClearTriggers()
{
	for (unsigned int i = 0; i < m_TriggerConditions.size(); ++i)
	{
		CAnimatorControllerTriggerParameter* l_Parameter = ((CAnimatorControllerTriggerParameter*)m_NewState->GetOwnAnimatorController()->GetParameter(m_TriggerConditions[i]->m_ParameterName));

		l_Parameter->RaiseTrigger();
	}
}