#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <string>

struct EAnimation
{
	std::string m_Name;
	bool m_Loop;
	unsigned int m_ID;
	float m_Weight;
	EAnimation(const std::string &Name, const bool &Loop, const unsigned int &ID, const float &Weight):m_Name(Name), m_Loop(Loop), m_ID(ID), m_Weight(Weight){};
};

#endif