#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <string>

struct EAnimation
{
	std::string m_Name;
	bool m_Loop;
	unsigned int m_ID;
	float m_Weight;
	float m_Duration;
	EAnimation():m_Name(""), m_Loop(false), m_ID(0), m_Weight(0.0f), m_Duration(0.0f){};
	EAnimation(const std::string &Name, bool Loop, unsigned int ID, float Weight, float Duration):m_Name(Name), m_Loop(Loop), m_ID(ID), m_Weight(Weight), m_Duration(Duration){};
};

#endif