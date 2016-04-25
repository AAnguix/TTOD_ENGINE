#include "RangedEnemy.h"
#include "Engine.h"
#include "PhysXManager.h"
#include "Weapon.h"
#include "Armor.h"

CRangedEnemy::CRangedEnemy(float Health, float Speed, float AttackDelay, float VisionRange, std::vector<Vect3f> WayPoints, float ShootingForce)
:CEnemy(Health, Speed, AttackDelay, VisionRange, WayPoints)
,m_ShootingForce(ShootingForce)
,m_Target(v3fZERO)
{
	m_Armor = new CArmor(3.0f, CArmor::Light);
	m_Weapon = new CWeapon(15.0f, CWeapon::Bow);
}

void CRangedEnemy::SetTarget(Vect3f Position)
{
	m_Target = Position;
}

void CRangedEnemy::Attack(float ElapsedTime)
{

}
