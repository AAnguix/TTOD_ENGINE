#ifndef _LUACOMPONENT_H
#define _LUACOMPONENT_H

#include <string>

class CLUAComponent
{
private:
	std::string m_Type;
	float m_Timer;
public:
	virtual ~CLUAComponent() {}
	CLUAComponent(const std::string Type):m_Type(Type),m_Timer(0.0f){};
	virtual void AddTime(float Time){ m_Timer += Time; };
	virtual float GetTimer() { return m_Timer; };
	virtual void ResetTimer() { m_Timer = 0.0f; };
	virtual std::string GetType(){ return m_Type; };
};

#endif