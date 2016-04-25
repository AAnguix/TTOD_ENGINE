#ifndef _ANIMATORCONTROLLERFLOATPARAMETER_H
#define _ANIMATORCONTROLLERFLOATPARAMETER_H

#include <string>
#include "Animation\AnimatorControllerParameter.h"

class CAnimatorControllerFloatParameter : CAnimatorControllerParameter
{
public:
	enum EFloatCondition
	{
		GREATER = 0,
		LESS
	};

private:
	float m_CurrentValue;
	float m_LaunchValue;
	EFloatCondition m_Condition;

public:
	CAnimatorControllerFloatParameter(const std::string &Name, const float &LaunchValue, EFloatCondition Condition);
	virtual ~CAnimatorControllerFloatParameter();
	void UpdateParameter(const float &CurrentValue);
	bool Launch();
};

#endif