#ifndef _WEAPON_H
#define _WEAPON_H

//#include "ISoundManager.h"
#include <string>

class CWeapon
{

public:
	enum EWeaponType{ Sword, Axe, Knife, Bow, PhysiqueStrength };

private:
	EWeaponType m_Type;
	float m_Damage;
	std::string m_Sound;

public:
	CWeapon(const float &Damage, const EWeaponType Type) :m_Damage(Damage), m_Type(Type){};
	float GetDamage(){ return m_Damage; };
	void SetDamage(const float &Damage){ m_Damage = Damage; };
	EWeaponType GetType(){ return m_Type; };
};

#endif
