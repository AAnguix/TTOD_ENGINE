#ifndef _ANIMATORCONTROLLERPARAMETER_H
#define _ANIMATORCONTROLLERPARAMETER_H

#include <string>

class CAnimatorControllerParameter
{
protected:
	std::string m_Name;

public:
	CAnimatorControllerParameter(const std::string &Name) :m_Name(Name){};
	virtual ~CAnimatorControllerParameter(){};
	virtual bool Launch() = 0;
};

#endif