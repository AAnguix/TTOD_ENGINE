#ifndef _ARMOR_H
#define _ARMOR_H

//#include "ISoundManager.h"
#include <string>

class CArmor 
{

public:
	enum EArmorType{Basic,Heavy,Light,Heroic,DragonSkin};

private:
	EArmorType m_Type;
	float m_Value;
	std::string m_Sound;

public:
	CArmor(const float &Value, const EArmorType Type):m_Value(Value),m_Type(Type){};
	float GetValue(){ return m_Value; };
	void SetValue(const float &Value){ m_Value = Value;};
	EArmorType GetType(){ return m_Type; };
};

#endif
