#include "Animation\AnimatorControllerFloatParameter.h"


CAnimatorControllerFloatParameter::CAnimatorControllerFloatParameter(const std::string &Name, const float &Value)
:CAnimatorControllerParameter(Name)
,m_Value(Value)
{

}
CAnimatorControllerFloatParameter::~CAnimatorControllerFloatParameter()
{

}

void CAnimatorControllerFloatParameter::UpdateParameter(const float &Value)
{
	m_Value = Value;
}


