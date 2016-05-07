#include "BasicEnemy.h"
#include "RenderableObjects\Armor.h"
#include "RenderableObjects\Weapon.h"
#include "Animation\Transition.h"

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

	GetAnimatorController()->AddState("Idle_State", "Idle_Anim", 1.0f, "OnEnter_Idle_BasicEnemy", "OnUpdate_Idle_BasicEnemy", "OnExit_Idle_BasicEnemy");
	GetAnimatorController()->AddState("MoveToAttack_State", "MoveToAttack_Anim", 1.0f, "OnEnter_MoveToAttack_BasicEnemy", "OnUpdate_MoveToAttack_BasicEnemy", "OnExit_MoveToAttack_BasicEnemy");
	GetAnimatorController()->AddState("Attack_State", "Attack_Anim", 1.0f, "OnEnter_Attack_BasicEnemy", "OnUpdate_Attack_BasicEnemy", "OnExit_Attack_BasicEnemy");
	GetAnimatorController()->AddState("Patrol_State", "Patrol_Anim", 1.0f, "OnEnter_Patrol_BasicEnemy", "OnUpdate_Patrol_BasicEnemy", "OnExit_Patrol_BasicEnemy");

	
	GetAnimatorController()->AddBool("IsPlayerInsideVisionRange", true);
	GetAnimatorController()->AddBool("DelayToPatrol", true);
	GetAnimatorController()->AddBool("AtackPlayer", true);
	GetAnimatorController()->AddTrigger("GoBackToIdle",true);

	CTransition* l_IdleToMoveToAttack = GetAnimatorController()->GetState("Idle_State")->AddTransition("IdleToMoveToAttack", GetAnimatorController()->GetState("MoveToAttack_State"), false, 0.0f);
	l_IdleToMoveToAttack->AddBoolCondition("IsPlayerInsideVisionRange", true);

	CTransition* l_IdleToPatrol = GetAnimatorController()->GetState("Idle_State")->AddTransition("IdleToPatrol", GetAnimatorController()->GetState("Patrol_State"), false, 0.0f);
	l_IdleToPatrol->AddBoolCondition("DelayToPatrol", true);

	CTransition* l_MoveToAttackToAttack = GetAnimatorController()->GetState("MoveToAttack_State")->AddTransition("MoveToAttackToAttack", GetAnimatorController()->GetState("Attack_State"), false, 0.0f);
	l_MoveToAttackToAttack->AddBoolCondition("AtackPlayer", true);

	CTransition* l_MoveToAttackToIdle = GetAnimatorController()->GetState("MoveToAttack_State")->AddTransition("MoveToAttackToIdle", GetAnimatorController()->GetState("Idle_State"), false, 0.0f);
	l_MoveToAttackToIdle->AddBoolCondition("IsPlayerInsideVisionRange", false);

	CTransition* l_PatrolToAttack = GetAnimatorController()->GetState("Patrol_State")->AddTransition("PatrolToAttack", GetAnimatorController()->GetState("Attack_State"), false, 0.0f);
	l_PatrolToAttack->AddBoolCondition("IsPlayerInsideVisionRange", true);

	CTransition* l_AttacktoIdle = GetAnimatorController()->GetState("Attack_State")->AddTransition("AttacktoIdle", GetAnimatorController()->GetState("Idle_State"), false, 0.0f);
	l_AttacktoIdle->AddTriggerCondition("GoBackToIdle");
}

void CBasicEnemy::MoveToPlayerNearestPoint(Vect3f Destination)
{
	/*float l_Distance;
	if (l_Distance < m_AttackRange)
	{
		m_PlayerInsideAttackRange = true;
	}*/
}