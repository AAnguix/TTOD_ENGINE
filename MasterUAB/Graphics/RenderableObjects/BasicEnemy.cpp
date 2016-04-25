#include "BasicEnemy.h"
#include "RenderableObjects\Armor.h"
#include "RenderableObjects\Weapon.h"

void CBasicEnemy::Attack()
{

}

CBasicEnemy::CBasicEnemy(std::vector<Vect3f> WayPoints)
:CEnemy(100.0f,1.0f,1.0f,3.0f,WayPoints)
,m_AttackRange(0.3f)
,m_PlayerInsideAttackRange(false)
{

}

CBasicEnemy::CBasicEnemy(float Health, float Speed, float AttackDelay, float VisionRange, std::vector<Vect3f> WayPoints, float AttackRange)
:CEnemy(Health,Speed,AttackDelay,VisionRange,WayPoints)
,m_AttackRange(AttackRange)
,m_PlayerInsideAttackRange(false)
{
	m_Armor = new CArmor(5.0f, CArmor::Basic);
	m_Weapon = new CWeapon(10.0f, CWeapon::Knife);
}

void CBasicEnemy::MoveToPlayerNearestPoint(Vect3f Destination)
{
	/*float l_Distance;
	if (l_Distance < m_AttackRange)
	{
		m_PlayerInsideAttackRange = true;
	}*/
}