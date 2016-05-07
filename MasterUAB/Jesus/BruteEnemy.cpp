#include "BruteEnemy.h"
#include "Engine.h"
#include "PhysXManager.h"
#include "Weapon.h"
#include "Armor.h"
#include "Animation\Transition.h"

void CBruteEnemy::Attack(float ElapsedTime)
{

}

CBruteEnemy::CBruteEnemy(float Health, float Speed, float AttackDelay, float VisionRange, std::vector<Vect3f> WayPoints, float ChargeForce)
:CEnemy(Health, Speed, AttackDelay,VisionRange,WayPoints)
,m_ChargeForce(ChargeForce)
{
	m_Armor = new CArmor(10.0f, CArmor::Heavy);
	m_Weapon = new CWeapon(20.0f, CWeapon::PhysiqueStrength);

	GetAnimatorController()->AddState("Idle_State", "Idle_Anim", 1.0f, "OnEnter_Idle_BruteEnemy", "OnUpdate_Idle_BruteEnemy", "OnExit_Idle_BruteEnemy");
	GetAnimatorController()->AddState("Charge_State", "Charge_State", 1.0f, "OnEnter_Charge_BruteEnemy", "OnUpdate_Charge_BruteEnemy", "OnExit_Charge_BruteEnemy");
	GetAnimatorController()->AddState("Patrol_State", "Patrol_Anim", 1.0f, "OnEnter_Patrol_BruteEnemy", "OnUpdate_Patrol_BruteEnemy", "OnExit_Patrol_BruteEnemy");


	GetAnimatorController()->AddBool("IsPlayerInsideVisionRange", true);
	GetAnimatorController()->AddBool("DelayToPatrol", true);
	GetAnimatorController()->AddTrigger("GoBackToIdle", true);
	
	CTransition* l_IdleToCharge = GetAnimatorController()->GetState("Idle_State")->AddTransition("IdleToCharge", GetAnimatorController()->GetState("Charge_State"), false, 0.0f);
	l_IdleToCharge->AddBoolCondition("IsPlayerInsideVisionRange", true);

	CTransition* l_IdleToPatrol = GetAnimatorController()->GetState("Idle_State")->AddTransition("IdleToPatrol", GetAnimatorController()->GetState("Patrol_State"), false, 0.0f);
	l_IdleToPatrol->AddBoolCondition("DelayToPatrol", true);

	CTransition* l_ChargetoIdle = GetAnimatorController()->GetState("Charge_State")->AddTransition("ChargetoIdle", GetAnimatorController()->GetState("Idle_State"), false, 0.0f);
	l_ChargetoIdle->AddTriggerCondition("GoBackToIdle");

	CTransition* l_PatrolToCharge = GetAnimatorController()->GetState("Patrol_State")->AddTransition("PatrolToCharge", GetAnimatorController()->GetState("Charge_State"), false, 0.0f);
	l_PatrolToCharge->AddBoolCondition("IsPlayerInsideVisionRange", true);
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