#ifndef _DAMAGEREDUCTION_H
#define _DAMAGEREDUCTION_H

#include "Weapon.h"
#include "Armor.h"
#include <map>

class CDamageReduction
{

private:
	std::map<std::pair<CWeapon::EWeaponType, CArmor::EArmorType>, float> m_DamageReductionPct;

public:
	bool AddValue(std::pair<CWeapon::EWeaponType, CArmor::EArmorType> WeaponArmor, float Percentage);
	float GetDamageReductionPct(std::pair<CWeapon::EWeaponType, CArmor::EArmorType> WeaponArmor);

};

#endif
