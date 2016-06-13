class 'CBruteEnemyComponent' (CEnemyComponent)
function CBruteEnemyComponent:__init(CGameObject)
	CEnemyComponent.__init(self, CGameObject, "BruteEnemy")
	self.m_Health=300.0
	self.m_Speed=1.5
	self.m_AttackDelay=1.0
	self.m_VisionRange=6.0
	self.m_PointRadius=0.63
	self.m_DelayToPatrol = 3.0
	
	self.m_CollisionSphereName = self.m_RObject:GetName().."_collision_sphere"
	self.m_CollisionSphereRadius = 0.5
	self.m_PositionToAttack = nil
	self.m_ChargeForce=20.0
	--self.m_Weapon = CWeapon(50,"physique_strength")
	--self.m_Armor = CArmor(20,"heavy")
end

function CBruteEnemyComponent:Attack()
	
end

function CBruteEnemyComponent:Initialize()
	
	CEnemyComponent.Initialize(self)
	local l_AnimatorController = CEnemyComponent.GetGameObject(self):GetAnimatorController()

	local l_Idle = l_AnimatorController:AddState("Idle_State", "idle", 1.0, "OnEnter_Idle_BruteEnemy", "OnUpdate_Idle_BruteEnemy", "OnExit_Idle_BruteEnemy")
	local l_Charge = self.m_RObject:GetAnimatorController():AddState("Charge_State", "run", 1.0, "OnEnter_Charge_BruteEnemy", "OnUpdate_Charge_BruteEnemy", "OnExit_Charge_BruteEnemy")
	local l_Patrol = self.m_RObject:GetAnimatorController():AddState("Patrol_State", "walk", 1.0, "OnEnter_Patrol_BruteEnemy", "OnUpdate_Patrol_BruteEnemy", "OnExit_Patrol_BruteEnemy")
	local l_Melee = self.m_RObject:GetAnimatorController():AddState("Melee_State", "iraAttack", 1.0, "OnEnter_Melee_BruteEnemy", "OnUpdate_Melee_BruteEnemy", "OnExit_Melee_BruteEnemy")
	
	l_AnimatorController:AddTrigger("IsPlayerInsideVisionRange", false)
	l_AnimatorController:AddTrigger("GoBackToIdle", false)
	l_AnimatorController:AddTrigger("MeleeAttack", false)
	l_AnimatorController:AddBool("DelayToPatrol", false)
	

	local l_IdleToCharge = l_Idle:AddTransition("IdleToCharge", l_Charge, false, 0.0, 0.1, 0.1)
	l_IdleToCharge:AddTriggerCondition("IsPlayerInsideVisionRange");
	
	local l_IdleToMelee = l_Idle:AddTransition("IdleToMelee", l_Melee, false, 0.0, 0.1, 0.1)
	l_IdleToMelee:AddTriggerCondition("MeleeAttack");

	local l_IdleToPatrol = l_Idle:AddTransition("IdleToPatrol", l_Patrol, false, 0.0, 0.1, 0.1)
	l_IdleToPatrol:AddBoolCondition("DelayToPatrol", true)

	local l_ChargetoIdle = l_Charge:AddTransition("ChargetoIdle", l_Idle, true, 0.0, 0.1, 0.1)
	l_ChargetoIdle:AddTriggerCondition("GoBackToIdle")
	
	local l_MeleetoIdle = l_Melee:AddTransition("MeleetoIdle", l_Idle, true, 0.0, 0.1, 0.1)
	
	local l_PatrolToCharge = l_Patrol:AddTransition("PatrolToCharge", l_Charge, false, 0.0, 0.1, 0.1)
	l_PatrolToCharge:AddTriggerCondition("IsPlayerInsideVisionRange")
	
	--Sphere to detect collisions with player
	--CPhysXManager::CreateRigidDynamicSphere(const std::string &Name, const float &Radius, const std::string Material, const Vect3f &Position, const Quatf &Orientation, int Group, float Density, bool IsKinematic)
	g_PhysXManager:RegisterMaterial("brute_enemy_trigger", 30.0, 40.0, 0.0);
	local l_Quat = Quatf(0.0, 0.0, 0.0, 1.0)
	local l_TriggerPosition = self.m_RObject:GetPosition()
	--l_TriggerPosition.y = 1.0
	g_PhysXManager:CreateRigidDynamicSphere(self.m_CollisionSphereName, self.m_CollisionSphereRadius,"brute_enemy_trigger",l_TriggerPosition,l_Quat,0,1.0,true)
	g_PhysXManager:SetShapeAsTrigger(self.m_CollisionSphereName)
end

function CBruteEnemyComponent:Charge(ElapsedTime)
	--g_LogManager:Log(m_PositionToAttack.x.." Player Pos X")
	--g_LogManager:Log(m_PositionToAttack.y.." Player Pos y")
	--g_LogManager:Log(m_PositionToAttack.z.." Player Pos z")
	local l_Enemy = self.m_RObject
	local l_EnemyPos = l_Enemy:GetPosition()
	local ChargeSpeed = self.m_Speed * 40
	local  l_Displacement = m_PositionToAttack - l_EnemyPos
	l_Displacement:Normalize(1.0)
	
	local l_Forward = l_Enemy:GetForward()
	l_Forward.y = 0.0
	
	self.m_Velocity.x = 0.0
	self.m_Velocity.z = 0.0
	self.m_Velocity = self.m_Velocity + (l_Displacement*ChargeSpeed)
	self.m_Velocity = self.m_Velocity + (g_Gravity*ElapsedTime)
	
	if ElapsedTime>0.0 then
		self.m_Velocity = g_PhysXManager:DisplacementCharacterController(l_Enemy:GetName(), (self.m_Velocity * ElapsedTime), ElapsedTime)
	else 
		self.m_Velocity = g_PhysXManager:DisplacementCharacterController(l_Enemy:GetName(), (self.m_Velocity), ElapsedTime)
	end
	
	if CTTODMathUtils.PointInsideCircle(m_PositionToAttack, l_EnemyPos, self.m_PointRadius) == true then
		return true
	end
	
	return false
end

function CBruteEnemyComponent:SetPosititonToAttack(Position)
	self.m_PositionToAttack = Position
end