#ifndef _ANIMATORCONTROLLERINTEGERPARAMETER_H
#define _ANIMATORCONTROLLERINTEGERPARAMETER_H

#include <string>
#include "Animation\AnimatorControllerParameter.h"

class CAnimatorControllerIntegerParameter : CAnimatorControllerParameter
{
public:
	int GetValue() const{ return m_Value; };

private:
	int m_Value;
	/*int m_LaunchValue;
	EIntegerCondition m_Condition;*/

public:
	CAnimatorControllerIntegerParameter(const std::string &Name, const int &Value);
	virtual ~CAnimatorControllerIntegerParameter();
	void UpdateParameter(const int &Value);
};

#endif