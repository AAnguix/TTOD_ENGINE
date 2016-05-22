#include "Components\AnimatorController\AnimatorControllerFloatParameter.h"


CAnimatorControllerFloatParameter::CAnimatorControllerFloatParameter(const std::string &Name, float Value)
:CAnimatorControllerParameter(Name)
,m_Value(Value)
{

}
CAnimatorControllerFloatParameter::~CAnimatorControllerFloatParameter()
{

}

void CAnimatorControllerFloatParameter::UpdateParameter(float Value)
{
	m_Value = Value;
}


