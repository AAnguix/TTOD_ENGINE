#ifndef _LUACOMPONENT_H
#define _LUACOMPONENT_H

#include <string>

class CLUAComponent
{
private:
	std::string m_Type;
public:
	virtual ~CLUAComponent() {}
	CLUAComponent(const std::string Type){ m_Type = Type; };
	virtual std::string GetType(){ return m_Type; };
};

#endif