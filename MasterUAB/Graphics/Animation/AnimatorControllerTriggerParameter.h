#ifndef _ANIMATORCONTROLLERTRIGGERPARAMETER_H
#define _ANIMATORCONTROLLERTRIGGERPARAMETER_H

#include <string>
#include "Animation\AnimatorControllerParameter.h"

class CAnimatorControllerTriggerParameter : CAnimatorControllerParameter
{

private:
	bool m_TriggerEnter;

public:
	CAnimatorControllerTriggerParameter(const std::string &Name);
	virtual ~CAnimatorControllerTriggerParameter();
	void TriggerEnter();
	bool Launch();
};

#endif