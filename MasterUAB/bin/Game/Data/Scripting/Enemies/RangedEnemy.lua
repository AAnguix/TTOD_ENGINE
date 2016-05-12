class 'CRangedEnemyComponent' (CEnemyComponent)
function CRangedEnemyComponent:__init(CRenderableObject)
	CEnemyComponent.__init(self, CRenderableObject, "rangued")
	self.m_Health=150.0
	self.m_Speed=2.0
	self.m_AttackDelay=2.0
	self.m_VisionRange=5.0
	self.m_PointRadius=0.3
	
	self.m_ShootingForce=50.0
	self.m_Weapon = CWeapon(100,"bow")
	self.m_Armor = CArmor(5,"light")
end

function CRangedEnemyComponent:Attack()
	
end

function CRangedEnemyComponent:Initialize()
	
	CEnemyComponent.Initialize(self)
	local l_AnimatorController = self.m_RObject:GetAnimatorController()

	l_AnimatorController:AddState("Idle_State", "idle", 1.0, "OnEnter_Idle_RangedEnemy", "OnUpdate_Idle_RangedEnemy", "OnExit_Idle_RangedEnemy")
	l_AnimatorController:AddState("Attack_State", "attack", 1.0, "OnEnter_Attack_RangedEnemy", "OnUpdate_Attack_RangedEnemy", "OnExit_Attack_RangedEnemy")
   
	l_AnimatorController:AddBool("IsPlayerInsideVisionRange", false)
	l_AnimatorController:AddBool("AtackPlayer", false)

	local l_IdleToAttack = l_AnimatorController:GetState("Idle_State"):AddTransition("IdleToAttack", GetAnimatorController():GetState("Attack_State"), false, 0.0, 0.1, 0.1)
	l_IdleToAttack:AddBoolCondition("IsPlayerInsideVisionRange", true)

	local l_AttackToIdle = l_AnimatorController:GetState("Attack_State"):AddTransition("AttackToIdle", GetAnimatorController():GetState("Idle_State"), false, 0.0, 0.1, 0.1)
	l_AttackToIdle:AddBoolCondition("IsPlayerInsideVisionRange", false)
end

function CRangedEnemyComponent:Chargue(Position, ElapsedTime)
	
	local  m_Position = self.m_RObject:GetPosition()
	local m_Name = self.m_RObject:GetName()
	local  l_Displacement = Position - m_Position
	l_Displacement:Normalize(1)

	g_PhysXManager():MoveCharacterController(m_Name, l_Displacement*self.m_Speed, ElapsedTime)

	if PointInsideCircle(m_Position, Position, self.m_PointRadius) then
		return false
	end
	
	return true
end