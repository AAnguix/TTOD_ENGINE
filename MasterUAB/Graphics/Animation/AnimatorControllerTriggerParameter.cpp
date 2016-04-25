#include "Animation\AnimatorControllerTriggerParameter.h"


CAnimatorControllerTriggerParameter::CAnimatorControllerTriggerParameter(const std::string &Name)
:CAnimatorControllerParameter(Name)
,m_TriggerEnter(false)
{

}
CAnimatorControllerTriggerParameter::~CAnimatorControllerTriggerParameter()
{

}
bool CAnimatorControllerTriggerParameter::Launch()
{
	if (m_TriggerEnter)
	{
		m_TriggerEnter = false;
		return true;
	}

	return false;
}

void CAnimatorControllerTriggerParameter::TriggerEnter()
{
	m_TriggerEnter = true;
}


