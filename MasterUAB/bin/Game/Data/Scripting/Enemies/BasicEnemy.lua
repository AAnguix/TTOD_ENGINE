class 'CBasicEnemyComponent' (CEnemyComponent)
function CBasicEnemyComponent:__init(CRenderableObject)
	CEnemyComponent.__init(self, CRenderableObject, "basic")
	self.m_Health=100.0
	self.m_Speed=1.0
	self.m_AttackDelay=1.0
	self.m_VisionRange=2.0
	self.m_DelayToPatrol = 4.0
	
	self.m_AttackRange=0.3
	self.m_Weapon = CWeapon(10,"knife")
	self.m_Armor = CArmor(5,"basic")
end

function CBasicEnemyComponent:Attack()
	
end

function CBasicEnemyComponent:Initialize()
	
	CEnemyComponent.Initialize(self)
	local l_AnimatorController = CEnemyComponent.GetRenderableObject(self):GetAnimatorController()
	
	local l_Idle = l_AnimatorController:AddState("Idle_State", "idle", 1.0, "OnEnter_Idle_BasicEnemy", "OnUpdate_Idle_BasicEnemy", "OnExit_Idle_BasicEnemy")
	local l_MoveToAttack = l_AnimatorController:AddState("MoveToAttack_State", "walk", 1.0, "OnEnter_MoveToAttack_BasicEnemy", "OnUpdate_MoveToAttack_BasicEnemy", "OnExit_MoveToAttack_BasicEnemy")
	local l_Attack = l_AnimatorController:AddState("Attack_State", "normalAttack", 1.0, "OnEnter_Attack_BasicEnemy", "OnUpdate_Attack_BasicEnemy", "OnExit_Attack_BasicEnemy")
	local l_Patrol = l_AnimatorController:AddState("Patrol_State", "walk", 1.0, "OnEnter_Patrol_BasicEnemy", "OnUpdate_Patrol_BasicEnemy", "OnExit_Patrol_BasicEnemy")
                                                   
	l_AnimatorController:AddBool("IsPlayerInsideVisionRange", false)
	l_AnimatorController:AddBool("DelayToPatrol", false)
	l_AnimatorController:AddTrigger("AtackPlayer", false)

	local l_IdleToMoveToAttack = l_Idle:AddTransition("IdleToMoveToAttack", l_MoveToAttack, false, 0.0, 0.1, 0.1)
	l_IdleToMoveToAttack:AddBoolCondition("IsPlayerInsideVisionRange", true)

	local l_IdleToPatrol = l_Idle:AddTransition("IdleToPatrol", l_Patrol, false, 0.0, 0.1, 0.1)
	l_IdleToPatrol:AddBoolCondition("DelayToPatrol", true)
	l_IdleToPatrol:AddBoolCondition("IsPlayerInsideVisionRange", false)

	local l_MoveToAttackToAttack = l_MoveToAttack:AddTransition("MoveToAttackToAttack", l_Attack, false, 0.0, 0.1, 0.1)
	l_MoveToAttackToAttack:AddTriggerCondition("AtackPlayer")

	--local l_MoveToAttackToIdle = l_MoveToAttack:AddTransition("MoveToAttackToIdle", l_Idle, false, 0.0, 0.1, 0.1)
	--l_MoveToAttackToIdle:AddBoolCondition("IsPlayerInsideVisionRange", false)

	local l_PatrolToAttack = l_Patrol:AddTransition("PatrolToAttack", l_Attack, false, 0.0, 0.1, 0.1)
	l_PatrolToAttack:AddBoolCondition("IsPlayerInsideVisionRange", true)

	--l_AttacktoIdle = l_Attack:AddTransition("AttacktoIdle", l_Idle, false, 0.0, 0.1, 0.1)
	--l_AttacktoIdle:AddTriggerCondition("GoBackToIdle")
end

function CBasicEnemyComponent:MoveToPlayerNearestPoint()

end

function CBasicEnemyComponent:FollowPlayer(ElapsedTime)
	
	local l_Player = GetPlayer()
	local l_Enemy = self.m_AnimatedInstance
	
	local l_PlayerPos = l_Player:GetPosition()
	local l_EnemyPos = l_Enemy:GetPosition()
	
	local l_VectorToPlayer = l_PlayerPos - l_EnemyPos
	l_VectorToPlayer:Normalize(1.0)
	
	local l_Yaw = l_Enemy:GetYaw()
	local l_Forward = l_Enemy:GetForward()
	
	local l_Dot = l_Forward*l_VectorToPlayer
	--if l_Dot>1.0 then l_Dot=1.0 end
	
	local l_NewAngle = math.acos(l_Dot)
	
	local l_Cross = l_Forward:cross(l_VectorToPlayer)
	
	if l_Cross.y < 0.0 then
		l_NewAngle = (l_NewAngle*(-1.0))
	end 
	
	l_Enemy:SetPosition(l_EnemyPos+(l_VectorToPlayer*ElapsedTime))
	
	l_Enemy:SetYaw(l_Yaw+(l_NewAngle*ElapsedTime))
	
end