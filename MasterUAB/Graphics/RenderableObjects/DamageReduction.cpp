#include "DamageReduction.h"
#include <assert.h>

bool CDamageReduction::AddValue(std::pair<CWeapon::EWeaponType, CArmor::EArmorType> WeaponArmor, float Percentage)
{
	std::map<std::pair<CWeapon::EWeaponType, CArmor::EArmorType>, float>::iterator itMap;

	itMap = m_DamageReductionPct.find(WeaponArmor);

	if (itMap != m_DamageReductionPct.end())
	{
		return false;
	}
	else
	{
		m_DamageReductionPct.insert(std::pair<std::pair<CWeapon::EWeaponType, CArmor::EArmorType>,float>(WeaponArmor, Percentage));
		return true;
	}
}

float CDamageReduction::GetDamageReductionPct(std::pair<CWeapon::EWeaponType, CArmor::EArmorType> WeaponArmor)
{
	std::map<std::pair<CWeapon::EWeaponType, CArmor::EArmorType>, float>::iterator itMap;

	itMap = m_DamageReductionPct.find(WeaponArmor);
	if (itMap == m_DamageReductionPct.end())
	{
		assert(false);
		return 0.0f;
	}
	else
		return itMap->second;
}