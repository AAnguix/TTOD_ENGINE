#include "Components\AnimatorController\AnimatorControllerBoolParameter.h"


CAnimatorControllerBoolParameter::CAnimatorControllerBoolParameter(const std::string &Name, bool Value)
:CAnimatorControllerParameter(Name)
,m_Value(Value)
{

}
CAnimatorControllerBoolParameter::~CAnimatorControllerBoolParameter()
{

}
//bool CAnimatorControllerBoolParameter::Launch()
//{
//	/*if (m_CurrentValue == m_LaunchValue)
//			return true;	
//
//	return false;*/
//}

void CAnimatorControllerBoolParameter::UpdateParameter(bool Value)
{
	m_Value = Value;
}


