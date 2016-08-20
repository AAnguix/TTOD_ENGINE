class 'CBasicEnemyComponent' (CEnemyComponent)
function CBasicEnemyComponent:__init(CGameObject)
	CEnemyComponent.__init(self, CGameObject,"BasicEnemy")
	self.m_Health=100.0
	self.m_MaxHealth=100.0
	self.m_Speed=1.2
	self.m_AttackDelay=2.5
	self.m_AttackRange=2.0
	self.m_VisionRange=5.0
	self.m_DelayToPatrol = 3.0
	self.m_HitDisplacement = 0.4
	
	self.m_GuiAvatar = "basic_enemy_avatar_image"
end

function CBasicEnemyComponent:Initialize()
	
	CEnemyComponent.Initialize(self)
	local l_Idle = self.m_LuaGameObject:AddState("Idle_State", "idle", 1.0, "OnEnter_Idle_BasicEnemy", "OnUpdate_Idle_BasicEnemy", "OnExit_Idle_BasicEnemy")
	local l_MoveToAttack = self.m_LuaGameObject:AddState("MoveToAttack_State", "walk", 1.0, "OnEnter_MoveToAttack_BasicEnemy", "OnUpdate_MoveToAttack_BasicEnemy", "OnExit_MoveToAttack_BasicEnemy")
	local l_Attack = self.m_LuaGameObject:AddState("Attack_State", "normalAttack", 1.0, "OnEnter_Attack_BasicEnemy", "OnUpdate_Attack_BasicEnemy", "OnExit_Attack_BasicEnemy")
	local l_Patrol = self.m_LuaGameObject:AddState("Patrol_State", "walk", 1.0, "OnEnter_Patrol_BasicEnemy", "OnUpdate_Patrol_BasicEnemy", "OnExit_Patrol_BasicEnemy")
    local l_Hit = self.m_LuaGameObject:AddState("Hit_State", "injured", 1.0, "OnEnter_Hit_BasicEnemy", "OnUpdate_Hit_BasicEnemy", "OnExit_Hit_BasicEnemy")
	
	self.m_LuaGameObject:AddBool("IsPlayerInsideVisionRange", false)
	self.m_LuaGameObject:AddBool("DelayToPatrol", false)
	self.m_LuaGameObject:AddTrigger("AttackPlayer", false)
	self.m_LuaGameObject:AddTrigger("GotHit", false)

	local l_IdleToPatrol = l_Idle:AddTransition("IdleToPatrol", l_Patrol, false, 0.1)
	l_IdleToPatrol:AddBoolCondition("DelayToPatrol", true)
	l_IdleToPatrol:AddBoolCondition("IsPlayerInsideVisionRange", false)
	
	local l_IdleToMoveToAttack = l_Idle:AddTransition("IdleToMoveToAttack", l_MoveToAttack, false, 0.1)
	l_IdleToMoveToAttack:AddBoolCondition("IsPlayerInsideVisionRange", true)
	
	local l_IdleToAttack = l_Idle:AddTransition("IdleToAttack", l_Attack, false,  0.1, 0.25)
	l_IdleToAttack:AddTriggerCondition("AttackPlayer")
	
	local l_IdleToHit = l_Idle:AddTransition("IdleToHit", l_Hit, false,  0.1, 0.25)
	l_IdleToHit:AddTriggerCondition("GotHit")
	
	local l_PatrolToMoveToAttack = l_Patrol:AddTransition("PatrolToMoveToAttack", l_MoveToAttack, false, 0.1)
	l_PatrolToMoveToAttack:AddBoolCondition("IsPlayerInsideVisionRange", true)
	
	local l_MoveToAttackToAttack = l_MoveToAttack:AddTransition("MoveToAttackToAttack", l_Attack, false, 0.1, 0.1)
	l_MoveToAttackToAttack:AddTriggerCondition("AttackPlayer")	
	
	local l_MoveToAttackToIdle = l_MoveToAttack:AddTransition("MoveToAttackToIdle", l_Idle, false, 0.1)
	l_MoveToAttackToIdle:AddBoolCondition("IsPlayerInsideVisionRange", false)

	local l_AttacktoIdle = l_Attack:AddTransition("AttacktoIdle", l_Idle, true, 0.1)
	--l_AttacktoIdle:AddTriggerCondition("GoBackToIdle")
	
	local l_AttacktoHit = l_Attack:AddTransition("AttacktoHit", l_Hit, false, 0.5, 0.5)
	l_AttacktoHit:AddTriggerCondition("GotHit")
	
	local l_HittoIdle = l_Hit:AddTransition("HittoIdle", l_Idle, true, 0.5)
	
	self.m_AStar = CAStar()
end

 
function CBasicEnemyComponent:Update(ElapsedTime)
    CEnemyComponent.Update(self,ElapsedTime)
    self:MovementController(ElapsedTime)
end

function CBasicEnemyComponent:MovementController(ElapsedTime)
	local l_Name = self.m_LuaGameObject:GetName()
	self.m_Velocity.x = 0.0
	self.m_Velocity.z = 0.0
	
	if self.M_FollowWaypoints then
		self:FollowTriangleWayPoints(ElapsedTime)
	elseif self.M_AttackMovement then
		self:MoveWithAStar(ElapsedTime)
	end
	
	self.m_Velocity = self.m_Velocity + (g_Gravity*ElapsedTime)
	if ElapsedTime>0.0 then
		self.m_Velocity = g_PhysXManager:DisplacementCharacterController(l_Name, (self.m_Velocity * ElapsedTime), ElapsedTime)
	else 
		self.m_Velocity = g_PhysXManager:DisplacementCharacterController(l_Name, (self.m_Velocity), ElapsedTime)
	end 
end