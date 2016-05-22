#include "Components\AnimatorController\AnimatorControllerIntegerParameter.h"


CAnimatorControllerIntegerParameter::CAnimatorControllerIntegerParameter(const std::string &Name, int Value)
:CAnimatorControllerParameter(Name)
,m_Value(Value)
{

}

CAnimatorControllerIntegerParameter::~CAnimatorControllerIntegerParameter()
{

}
	
//bool CAnimatorControllerIntegerParameter::Launch()
//{
//	if (m_Condition == GREATER)
//	{
//		if (m_CurrentValue > m_LaunchValue)
//			return true;
//	}
//	else if (m_Condition == EQUALS)
//	{
//		if (m_CurrentValue == m_LaunchValue)
//			return true;
//	}
//	else if (m_Condition == LESS)
//	{
//		if (m_CurrentValue < m_LaunchValue)
//			return true;
//	}
//	else if (m_Condition == NOT_EQUAL)
//	{
//		if (m_CurrentValue != m_LaunchValue)
//			return true;
//	}
//	
//	return false;
//	
//}

void CAnimatorControllerIntegerParameter::UpdateParameter(int Value)
{
	m_Value = Value;
}


