#include "BruteEnemy.h"
#include "Engine.h"
#include "PhysXManager.h"
#include "Weapon.h"
#include "Armor.h"

void CBruteEnemy::Attack(float ElapsedTime)
{

}

CBruteEnemy::CBruteEnemy(float Health, float Speed, float AttackDelay, float VisionRange, std::vector<Vect3f> WayPoints, float ChargeForce)
:CEnemy(Health, Speed, AttackDelay,VisionRange,WayPoints)
,m_ChargeForce(ChargeForce)
{
	m_Armor = new CArmor(10.0f, CArmor::Heavy);
	m_Weapon = new CWeapon(20.0f, CWeapon::PhysiqueStrength);
}

bool CBruteEnemy::Charge(Vect3f Position, float ElapsedTime)
{
	Vect3f l_Displacement = v3fZERO;

	l_Displacement = Position - m_Position;
	l_Displacement.Normalize();

	CEngine::GetSingleton().GetPhysXManager()->MoveCharacterController(m_Name, l_Displacement*m_Speed, ElapsedTime);

	if (PointInsideCircle(m_Position, Position, POINT_RADIUS))
	{
		return false;
	}
	return true;
}