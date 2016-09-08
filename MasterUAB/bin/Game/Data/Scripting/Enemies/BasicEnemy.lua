class 'CBasicEnemyComponent' (CEnemyComponent)
function CBasicEnemyComponent:__init(CGameObject)
	CEnemyComponent.__init(self, CGameObject,"BasicEnemy")
	self.m_Health=10.0
	self.m_MaxHealth=self.m_Health
	self.m_Speed=1.2
	self.m_AttackDelay=1.5 + (math.random()*0.5)
	self.m_AttackRange=2.0
	self.m_VisionRange=5.0
	self.m_DelayToPatrol = 3.0
	self.m_HitDisplacement = 0.3
	
    self.m_AttackStepBackDistance = 1.5
	self.m_AttackStepBackDisplacement = 0.2
	
	self.m_GuiAvatar = "basic_enemy_avatar_image"
end

function CBasicEnemyComponent:Initialize()
	
	CEnemyComponent.Initialize(self)
	
	local l_Idle = self.m_LuaGameObject:AddState("Idle_State", "idle", 1.0, "OnEnter_Idle_BasicEnemy", "OnUpdate_Idle_BasicEnemy", "OnExit_Idle_BasicEnemy")
	local l_CombatIdle = self.m_LuaGameObject:AddState("CombatIdle_State", "alert", 1.0, "OnEnter_CombatIdle_BasicEnemy", "OnUpdate_CombatIdle_BasicEnemy", "OnExit_CombatIdle_BasicEnemy")
	local l_MoveToAttack = self.m_LuaGameObject:AddState("MoveToAttack_State", "walk", 1.0, "OnEnter_MoveToAttack_BasicEnemy", "OnUpdate_MoveToAttack_BasicEnemy", "OnExit_MoveToAttack_BasicEnemy")
	local l_Attack = self.m_LuaGameObject:AddState("Attack_State", "normalAttack", 1.0, "OnEnter_Attack_BasicEnemy", "OnUpdate_Attack_BasicEnemy", "OnExit_Attack_BasicEnemy")
	local l_Patrol = self.m_LuaGameObject:AddState("Patrol_State", "walk", 1.0, "OnEnter_Patrol_BasicEnemy", "OnUpdate_Patrol_BasicEnemy", "OnExit_Patrol_BasicEnemy")
    local l_Hit = self.m_LuaGameObject:AddState("Hit_State", "injured", 1.0, "OnEnter_Hit_BasicEnemy", "OnUpdate_Hit_BasicEnemy", "OnExit_Hit_BasicEnemy")
	local l_Dead = self.m_LuaGameObject:AddState("Dead_State", "die", 1.0, "OnEnter_Dead_BasicEnemy", "OnUpdate_Dead_BasicEnemy", "OnExit_Dead_BasicEnemy")
	
	self.m_LuaGameObject:AddBool("IsPlayerInsideVisionRange", false)
	self.m_LuaGameObject:AddBool("DelayToPatrol", false)
	self.m_LuaGameObject:AddTrigger("AttackPlayer", false)
	self.m_LuaGameObject:AddTrigger("GotHit", false)
	self.m_LuaGameObject:AddBool("Dead", false)

	local l_IdleToPatrol = l_Idle:AddTransition("IdleToPatrol", l_Patrol, false, 0.1)
	l_IdleToPatrol:AddBoolCondition("DelayToPatrol", true)
	l_IdleToPatrol:AddBoolCondition("IsPlayerInsideVisionRange", false)
	
	local l_IdleToMoveToAttack = l_Idle:AddTransition("IdleToMoveToAttack", l_MoveToAttack, false, 0.1)
	l_IdleToMoveToAttack:AddBoolCondition("IsPlayerInsideVisionRange", true)
	
	local l_CombatIdleToIdle = l_CombatIdle:AddTransition("CombatIdleToIdle", l_Idle, true, 0.1)
	l_CombatIdleToIdle:AddBoolCondition("IsPlayerInsideVisionRange", false)
	
	local l_CombatIdleToAttack = l_CombatIdle:AddTransition("CombatIdleToAttack", l_Attack, false,  0.1, 0.25)
	l_CombatIdleToAttack:AddTriggerCondition("AttackPlayer")
	
	local l_CombatIdleToHit = l_CombatIdle:AddTransition("CombatIdleToHit", l_Hit, false,  0.1, 0.25)
	l_CombatIdleToHit:AddTriggerCondition("GotHit")
	
	local l_PatrolToMoveToAttack = l_Patrol:AddTransition("PatrolToMoveToAttack", l_MoveToAttack, false, 0.1)
	l_PatrolToMoveToAttack:AddBoolCondition("IsPlayerInsideVisionRange", true)
	
	local l_MoveToAttackToAttack = l_MoveToAttack:AddTransition("MoveToAttackToAttack", l_Attack, false, 0.1, 0.1)
	l_MoveToAttackToAttack:AddTriggerCondition("AttackPlayer")	
	
	local l_MoveToAttackToIdle = l_MoveToAttack:AddTransition("MoveToAttackToIdle", l_Idle, false, 0.1)
	l_MoveToAttackToIdle:AddBoolCondition("IsPlayerInsideVisionRange", false)

	local l_AttackToCombatIdle = l_Attack:AddTransition("AttackToCombatIdle", l_CombatIdle, true, 0.1)
	--l_AttacktoIdle:AddTriggerCondition("GoBackToIdle")
	
	local l_HitToAttack = l_Hit:AddTransition("HitToAttack", l_Attack, true, 0.1, 0.1)
	
	local l_AnyToDead = self.m_LuaGameObject:AddAnyStateTransition("AnyStateToDead",l_Dead, false, 0.1, 0.1)
	l_AnyToDead:AddBoolCondition("Dead", true)
	
	local l_DeadToIdle = l_Dead:AddTransition("DeadToIdle",l_Idle, true, 0.1)
	l_DeadToIdle:AddBoolCondition("Dead", false)
	
	self.m_AStar = CAStar()
end

 
function CBasicEnemyComponent:Update(ElapsedTime)
    CEnemyComponent.Update(self,ElapsedTime)
	if (not self.m_Dead) then
		self:MovementController(ElapsedTime)
	end 
end

function CBasicEnemyComponent:MovementController(ElapsedTime)
	local l_Name = self.m_LuaGameObject:GetName()
	
	if self.m_FollowWaypoints then
		self:FollowTriangleWayPoints(ElapsedTime)
	elseif self.m_AttackMovement then
		self:MoveWithAStar(ElapsedTime)
	end
	
	self.m_Velocity = self.m_Velocity + (g_Gravity*ElapsedTime)
	
	if ElapsedTime>0.0 then
		self.m_Velocity = g_PhysXManager:DisplacementCharacterController(l_Name, (self.m_Velocity * ElapsedTime), ElapsedTime)
	else 
		self.m_Velocity = g_PhysXManager:DisplacementCharacterController(l_Name, (self.m_Velocity), ElapsedTime)
	end 
	
	self.m_Velocity.x = 0.0
	self.m_Velocity.y = 0.0
	self.m_Velocity.z = 0.0
end