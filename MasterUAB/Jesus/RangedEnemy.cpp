#include "RangedEnemy.h"
#include "Engine.h"
#include "PhysXManager.h"
#include "Weapon.h"
#include "Armor.h"
#include "Animation\Transition.h"

CRangedEnemy::CRangedEnemy(float Health, float Speed, float AttackDelay, float VisionRange, std::vector<Vect3f> WayPoints, float ShootingForce)
:CEnemy(Health, Speed, AttackDelay, VisionRange, WayPoints)
,m_ShootingForce(ShootingForce)
,m_Target(v3fZERO)
{
	m_Armor = new CArmor(3.0f, CArmor::Light);
	m_Weapon = new CWeapon(15.0f, CWeapon::Bow);

	GetAnimatorController()->AddState("Idle_State", "Idle_Anim", 1.0f, "OnEnter_Idle_RangedEnemy", "OnUpdate_Idle_RangedEnemy", "OnExit_Idle_RangedEnemy");
	GetAnimatorController()->AddState("Attack_State", "Attack_Anim", 1.0f, "OnEnter_Attack_RangedEnemy", "OnUpdate_Attack_RangedEnemy", "OnExit_Attack_RangedEnemy");
	
	GetAnimatorController()->AddBool("IsPlayerInsideVisionRange", false);
	GetAnimatorController()->AddBool("AtackPlayer", false);

	CTransition* l_IdleToAttack = GetAnimatorController()->GetState("Idle_State")->AddTransition("IdleToAttack", GetAnimatorController()->GetState("Attack_State"), false, 0.0f);
	l_IdleToAttack->AddBoolCondition("IsPlayerInsideVisionRange", true);

	CTransition* l_AttackToIdle = GetAnimatorController()->GetState("Attack_State")->AddTransition("AttackToIdle", GetAnimatorController()->GetState("Idle_State"), false, 0.0f);
	l_AttackToIdle->AddBoolCondition("IsPlayerInsideVisionRange", false);
	 

	
}

void CRangedEnemy::SetTarget(Vect3f Position)
{
	m_Target = Position;
}

void CRangedEnemy::Attack(float ElapsedTime)
{

}
