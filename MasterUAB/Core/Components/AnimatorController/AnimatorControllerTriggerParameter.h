#ifndef _ANIMATORCONTROLLERTRIGGERPARAMETER_H
#define _ANIMATORCONTROLLERTRIGGERPARAMETER_H

#include <string>
#include "Components\AnimatorController\AnimatorControllerParameter.h"

class CAnimatorControllerTriggerParameter : CAnimatorControllerParameter
{

private:
	bool m_TriggerEnter;

public:
	CAnimatorControllerTriggerParameter(const std::string &Name, bool Triggered);
	virtual ~CAnimatorControllerTriggerParameter();
	
	void TriggerEnter(){ m_TriggerEnter = true; };
	void RaiseTrigger(){ m_TriggerEnter = false; };

	bool Triggered() const { return m_TriggerEnter; };
	
};

#endif