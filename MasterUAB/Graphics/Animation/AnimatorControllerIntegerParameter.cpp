#include "Animation\AnimatorControllerIntegerParameter.h"


CAnimatorControllerIntegerParameter::CAnimatorControllerIntegerParameter(const std::string &Name, const int &LaunchValue, EIntegerCondition Condition)
:CAnimatorControllerParameter(Name)
,m_LaunchValue(LaunchValue)
,m_Condition(Condition)
,m_CurrentValue(0)
{

}

CAnimatorControllerIntegerParameter::~CAnimatorControllerIntegerParameter()
{

}
	
bool CAnimatorControllerIntegerParameter::Launch()
{
	if (m_Condition == GREATER)
	{
		if (m_CurrentValue > m_LaunchValue)
			return true;
	}
	else if (m_Condition == EQUALS)
	{
		if (m_CurrentValue == m_LaunchValue)
			return true;
	}
	else if (m_Condition == LESS)
	{
		if (m_CurrentValue < m_LaunchValue)
			return true;
	}
	else if (m_Condition == NOT_EQUAL)
	{
		if (m_CurrentValue != m_LaunchValue)
			return true;
	}
	
	return false;
	
}

void CAnimatorControllerIntegerParameter::UpdateParameter(const int &CurrentValue)
{
	m_CurrentValue = CurrentValue;
}


