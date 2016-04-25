#include "Animation\AnimatorControllerBoolParameter.h"


CAnimatorControllerBoolParameter::CAnimatorControllerBoolParameter(const std::string &Name, const bool &LaunchValue)
:CAnimatorControllerParameter(Name)
,m_LaunchValue(LaunchValue)
,m_CurrentValue(0)
{

}
CAnimatorControllerBoolParameter::~CAnimatorControllerBoolParameter()
{

}
bool CAnimatorControllerBoolParameter::Launch()
{
	if (m_CurrentValue == m_LaunchValue)
			return true;	

	return false;
}

void CAnimatorControllerBoolParameter::UpdateParameter(const bool &CurrentValue)
{
	m_CurrentValue = CurrentValue;
}


