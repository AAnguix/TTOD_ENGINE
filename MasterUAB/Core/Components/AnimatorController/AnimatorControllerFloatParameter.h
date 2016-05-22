#ifndef _ANIMATORCONTROLLERFLOATPARAMETER_H
#define _ANIMATORCONTROLLERFLOATPARAMETER_H

#include <string>
#include "Components\AnimatorController\AnimatorControllerParameter.h"

class CAnimatorControllerFloatParameter : CAnimatorControllerParameter
{
public:
	float GetValue() const{ return m_Value; };
private:
	float m_Value;

public:
	CAnimatorControllerFloatParameter(const std::string &Name, float Value);
	virtual ~CAnimatorControllerFloatParameter();
	void UpdateParameter(float Value);
};

#endif