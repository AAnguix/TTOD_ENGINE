#ifndef _ANIMATORCONTROLLERBOOLPARAMETER_H
#define _ANIMATORCONTROLLERBOOLPARAMETER_H

#include <string>
#include "Animation\AnimatorControllerParameter.h"

class CAnimatorControllerBoolParameter : CAnimatorControllerParameter
{

private:
	bool m_CurrentValue;
	bool m_LaunchValue;

public:
	CAnimatorControllerBoolParameter(const std::string &Name, const bool &LaunchValue);
	virtual ~CAnimatorControllerBoolParameter();
	void UpdateParameter(const bool &CurrentValue);
	bool Launch();
};

#endif