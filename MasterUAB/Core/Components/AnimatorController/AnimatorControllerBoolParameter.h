#ifndef _ANIMATORCONTROLLERBOOLPARAMETER_H
#define _ANIMATORCONTROLLERBOOLPARAMETER_H

#include <string>
#include "Components\AnimatorController\AnimatorControllerParameter.h"

class CAnimatorControllerBoolParameter : CAnimatorControllerParameter
{
public:
	bool GetValue() const{ return m_Value; };

private:
	bool m_Value;

public:
	CAnimatorControllerBoolParameter(const std::string &Name, bool Value);
	virtual ~CAnimatorControllerBoolParameter();
	void UpdateParameter(bool Value);
};

#endif