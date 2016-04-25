#include "Animation\AnimatorControllerFloatParameter.h"


CAnimatorControllerFloatParameter::CAnimatorControllerFloatParameter(const std::string &Name, const float &LaunchValue, EFloatCondition Condition)
:CAnimatorControllerParameter(Name)
,m_LaunchValue(LaunchValue)
,m_Condition(Condition)
,m_CurrentValue(0.0f)
{

}
CAnimatorControllerFloatParameter::~CAnimatorControllerFloatParameter()
{

}
bool CAnimatorControllerFloatParameter::Launch()
{
	if (m_Condition == GREATER)
	{
		if (m_CurrentValue > m_LaunchValue)
			return true;
	}
	else if (m_Condition == LESS)
	{
		if (m_CurrentValue < m_LaunchValue)
			return true;
	}

	return false;

}
void CAnimatorControllerFloatParameter::UpdateParameter(const float &CurrentValue)
{
	m_CurrentValue = CurrentValue;
}


