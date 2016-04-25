#ifndef _ANIMATORCONTROLLERINTEGERPARAMETER_H
#define _ANIMATORCONTROLLERINTEGERPARAMETER_H

#include <string>
#include "Animation\AnimatorControllerParameter.h"

class CAnimatorControllerIntegerParameter : CAnimatorControllerParameter
{
public:
	enum EIntegerCondition
	{
		GREATER = 0,
		LESS,
		EQUALS,
		NOT_EQUAL
	};

private:
	int m_CurrentValue;
	int m_LaunchValue;
	EIntegerCondition m_Condition;

public:
	CAnimatorControllerIntegerParameter(const std::string &Name, const int &LaunchValue, EIntegerCondition Condition);
	virtual ~CAnimatorControllerIntegerParameter();
	void UpdateParameter(const int &CurrentValue);
	bool Launch();
};

#endif