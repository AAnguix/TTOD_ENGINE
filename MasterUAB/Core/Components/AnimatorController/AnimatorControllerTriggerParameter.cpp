#include "Components\AnimatorController\AnimatorControllerTriggerParameter.h"


CAnimatorControllerTriggerParameter::CAnimatorControllerTriggerParameter(const std::string &Name, bool Triggered)
:CAnimatorControllerParameter(Name)
, m_TriggerEnter(Triggered)
{

}

CAnimatorControllerTriggerParameter::~CAnimatorControllerTriggerParameter()
{

}



