class 'CBasicEnemyComponent' (CEnemyComponent)
function CBasicEnemyComponent:__init(CRenderableObject)
	CEnemyComponent.__init(self, CRenderableObject, "basic")
	self.m_Health=100.0
	self.m_Speed=1.2
	self.m_AttackDelay=1.5
	self.m_AttackRange=0.65
	self.m_VisionRange=5.0
	self.m_DelayToPatrol = 3.0
	
	-- self.m_Weapon = CWeapon(10,"knife")
	-- self.m_Armor = CArmor(5,"basic")
end

function CBasicEnemyComponent:Attack()
	
end

function CBasicEnemyComponent:Initialize()
	
	CEnemyComponent.Initialize(self)
	local l_AnimatorController = CEnemyComponent.GetRenderableObject(self):GetAnimatorController()
	
	local l_Idle = l_AnimatorController:AddState("Idle_State", "idle", 1.0, "OnEnter_Idle_BasicEnemy", "OnUpdate_Idle_BasicEnemy", "OnExit_Idle_BasicEnemy")
	local l_MoveToAttack = l_AnimatorController:AddState("MoveToAttack_State", "run", 1.0, "OnEnter_MoveToAttack_BasicEnemy", "OnUpdate_MoveToAttack_BasicEnemy", "OnExit_MoveToAttack_BasicEnemy")
	local l_Attack = l_AnimatorController:AddState("Attack_State", "normalAttack", 1.0, "OnEnter_Attack_BasicEnemy", "OnUpdate_Attack_BasicEnemy", "OnExit_Attack_BasicEnemy")
	local l_Patrol = l_AnimatorController:AddState("Patrol_State", "walk", 1.0, "OnEnter_Patrol_BasicEnemy", "OnUpdate_Patrol_BasicEnemy", "OnExit_Patrol_BasicEnemy")
                                                   
	l_AnimatorController:AddBool("IsPlayerInsideVisionRange", false)
	l_AnimatorController:AddBool("DelayToPatrol", false)
	l_AnimatorController:AddTrigger("AttackPlayer", false)

	local l_IdleToPatrol = l_Idle:AddTransition("IdleToPatrol", l_Patrol, false, 0.0, 0.1, 0.1)
	l_IdleToPatrol:AddBoolCondition("DelayToPatrol", true)
	l_IdleToPatrol:AddBoolCondition("IsPlayerInsideVisionRange", false)
	
	local l_IdleToMoveToAttack = l_Idle:AddTransition("IdleToMoveToAttack", l_MoveToAttack, false, 0.0, 0.1, 0.1)
	l_IdleToMoveToAttack:AddBoolCondition("IsPlayerInsideVisionRange", true)
	
	local l_PatrolToMoveToAttack = l_Patrol:AddTransition("PatrolToMoveToAttack", l_MoveToAttack, false,0.0, 0.1, 0.1)
	l_PatrolToMoveToAttack:AddBoolCondition("IsPlayerInsideVisionRange", true)
	
	local l_MoveToAttackToAttack = l_MoveToAttack:AddTransition("MoveToAttackToAttack", l_Attack, false, 0.0, 0.1, 0.1)
	l_MoveToAttackToAttack:AddTriggerCondition("AttackPlayer")	
	
	local l_MoveToAttackToIdle = l_MoveToAttack:AddTransition("MoveToAttackToIdle", l_Idle, false, 0.0, 0.1, 0.1)
	l_MoveToAttackToIdle:AddBoolCondition("IsPlayerInsideVisionRange", false)
	
	local l_IdleToAttack = l_Idle:AddTransition("IdleToAttack", l_Attack, false, 0.0, 0.1, 0.1)
	l_IdleToAttack:AddTriggerCondition("AttackPlayer")	

	l_AttacktoIdle = l_Attack:AddTransition("AttacktoIdle", l_Idle, true, 0.0, 0.1, 0.1)
	--l_AttacktoIdle:AddTriggerCondition("GoBackToIdle")
end

function CBasicEnemyComponent:MoveToPlayerNearestPoint(PlayerPos, ElapsedTime)
	
	local l_Enemy = self.m_RObject
	local l_EnemyPos = l_Enemy:GetPosition()
	
	local l_VectorToPlayer = PlayerPos - l_EnemyPos
	l_VectorToPlayer:Normalize(1.0)
	
	local l_Forward = l_Enemy:GetForward()
	l_Forward.y = 0.0
	
	local l_NewAngle = 0.0 
	
	self.m_Velocity.x = 0.0
	self.m_Velocity.z = 0.0
	self.m_Velocity = self.m_Velocity + (l_VectorToPlayer*self.m_Speed)
	self.m_Velocity = self.m_Velocity + (g_Gravity*ElapsedTime)
	if ElapsedTime>0.0 then
		self.m_Velocity = g_PhysXManager:DisplacementCharacterController(l_Enemy:GetName(), (self.m_Velocity * ElapsedTime), ElapsedTime)
		self:LookAtPoint(PlayerPos,ElapsedTime)
	else 
		self.m_Velocity = g_PhysXManager:DisplacementCharacterController(l_Enemy:GetName(), (self.m_Velocity), ElapsedTime)
		self:LookAtPoint(PlayerPos,0.0)
	end 
end