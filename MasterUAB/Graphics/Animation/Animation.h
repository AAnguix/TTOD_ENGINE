#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <string>

struct EAnimation
{
	std::string m_Name;
	bool m_Loop;
	unsigned int m_ID;
	EAnimation(const std::string &Name, const bool &Loop, const unsigned int &ID):m_Name(Name), m_Loop(Loop), m_ID(ID){};
};

#endif