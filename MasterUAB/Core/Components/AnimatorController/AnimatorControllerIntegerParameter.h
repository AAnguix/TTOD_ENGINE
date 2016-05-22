#ifndef _ANIMATORCONTROLLERINTEGERPARAMETER_H
#define _ANIMATORCONTROLLERINTEGERPARAMETER_H

#include <string>
#include "Components\AnimatorController\AnimatorControllerParameter.h"

class CAnimatorControllerIntegerParameter : CAnimatorControllerParameter
{
public:
	int GetValue() const{ return m_Value; };

private:
	int m_Value;
	/*int m_LaunchValue;
	EIntegerCondition m_Condition;*/

public:
	CAnimatorControllerIntegerParameter(const std::string &Name, int Value);
	virtual ~CAnimatorControllerIntegerParameter();
	void UpdateParameter(int Value);
};

#endif