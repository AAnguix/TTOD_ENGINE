class 'CBruteEnemyComponent' (CEnemyComponent)
function CBruteEnemyComponent:__init(CRenderableObject)
	CEnemyComponent.__init(self, CRenderableObject)
	self.m_Health=300.0
	self.m_Speed=0.5
	self.m_AttackDelay=1.0
	self.m_VisionRange=2.0
	self.m_PointRadius=0.3
	
	self.m_ChargeForce=20.0
	self.m_Weapon = CWeapon(50,"physique_strength")
	self.m_Armor = CArmor(20,"heavy")
	
	self:Initialize()
end

function CBruteEnemyComponent:Attack()
	
end

function CBruteEnemyComponent:Initialize()
	
	local l_AnimatorController = self.m_RObject:GetAnimatorController()

	local l_Idle = l_AnimatorController:AddState("Idle_State", "Idle_Anim", 1.0, "OnEnter_Idle_BruteEnemy", "OnUpdate_Idle_BruteEnemy", "OnExit_Idle_BruteEnemy")
	local l_Chargue = self.m_RObject:GetAnimatorController():AddState("Charge_State", "Charge_State", 1.0, "OnEnter_Charge_BruteEnemy", "OnUpdate_Charge_BruteEnemy", "OnExit_Charge_BruteEnemy")
	local l_Patrol = self.m_RObject:GetAnimatorController():AddState("Patrol_State", "Patrol_Anim", 1.0, "OnEnter_Patrol_BruteEnemy", "OnUpdate_Patrol_BruteEnemy", "OnExit_Patrol_BruteEnemy")

	l_AnimatorController:AddBool("IsPlayerInsideVisionRange", true)
	l_AnimatorController:AddBool("DelayToPatrol", true)
	l_AnimatorController:AddTrigger("GoBackToIdle", true)

	local l_IdleToCharge = l_Idle:AddTransition("IdleToCharge", l_Chargue, false, 0.0, 0.1, 0.1)
	l_IdleToCharge:AddBoolCondition("IsPlayerInsideVisionRange", true);

	local l_IdleToPatrol = l_Idle:AddTransition("IdleToPatrol", l_Patrol, false, 0.0, 0.1, 0.1)
	l_IdleToPatrol:AddBoolCondition("DelayToPatrol", true)

	local l_ChargetoIdle = l_Chargue:AddTransition("ChargetoIdle", l_Idle, false, 0.0, 0.1, 0.1)
	l_ChargetoIdle:AddTriggerCondition("GoBackToIdle")

	local l_PatrolToCharge = l_Patrol:AddTransition("PatrolToCharge", l_Chargue, false, 0.0, 0.1, 0.1)
	l_PatrolToCharge:AddBoolCondition("IsPlayerInsideVisionRange", true)	
end

function CBruteEnemyComponent:Chargue(Position, ElapsedTime)
	
	local  m_Position = self.m_RObject
	local m_Name = self.m_RObject:GetName()
	local  l_Displacement = Position - m_Position
	l_Displacement:Normalize()

	g_PhysXManager():MoveCharacterController(m_Name, l_Displacement*self.m_Speed, ElapsedTime)

	if PointInsideCircle(m_Position, Position, self.m_PointRadius) then
		return false
	end
	
	return true
end