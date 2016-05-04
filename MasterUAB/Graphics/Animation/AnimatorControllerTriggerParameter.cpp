#include "Animation\AnimatorControllerTriggerParameter.h"


CAnimatorControllerTriggerParameter::CAnimatorControllerTriggerParameter(const std::string &Name, const bool &Triggered)
:CAnimatorControllerParameter(Name)
, m_TriggerEnter(Triggered)
{

}

CAnimatorControllerTriggerParameter::~CAnimatorControllerTriggerParameter()
{

}



