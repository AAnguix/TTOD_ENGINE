class 'CRangedEnemyComponent' (CEnemyComponent)
function CRangedEnemyComponent:__init(CLuaGameObject)
	CEnemyComponent.__init(self, CLuaGameObject, "RanguedEnemy")
	self.m_Health=150.0
	self.m_MaxHealth=150.0
	self.m_Speed=2.0
	self.m_AttackDelay=2.0
	self.m_VisionRange=9.0
	self.m_PointRadius=0.66
	
	self.m_ShootingForce=50.0
	
	--self.m_Weapon = "Cristalmanec"
	
	--self.m_Weapon = CWeapon(100,"bow")
	--self.m_Armor = CArmor(5,"light")
end

function CRangedEnemyComponent:Attack()
	
end

function CRangedEnemyComponent:Initialize()
	
	CEnemyComponent.Initialize(self)
	
	local l_Idle = self.m_LuaGameObject:AddState("Idle_State", "idle", 1.0, "OnEnter_Idle_RangedEnemy", "OnUpdate_Idle_RangedEnemy", "OnExit_Idle_RangedEnemy")
	local l_Attack = self.m_LuaGameObject:AddState("Attack_State", "normalAttack", 1.0, "OnEnter_Attack_RangedEnemy", "OnUpdate_Attack_RangedEnemy", "OnExit_Attack_RangedEnemy")
	
	self.m_LuaGameObject:AddBool("IsPlayerInsideVisionRange", false)
	self.m_LuaGameObject:AddBool("AtackPlayer", false)
	
	local l_IdleToAttack = l_Idle:AddTransition("IdleToAttack",l_Attack, false, 0.0, 0.1, 0.1)
	l_IdleToAttack:AddBoolCondition("IsPlayerInsideVisionRange", true)

	local l_AttackToIdle = l_Attack:AddTransition("AttackToIdle", l_Idle, false, 0.0, 0.1, 0.1)
	l_AttackToIdle:AddBoolCondition("IsPlayerInsideVisionRange", false)
end

function CRangedEnemyComponent:Flee(Position, ElapsedTime)
	
	-- local  m_Position = self.m_RObject:GetPosition()
	-- local m_Name = self.m_RObject:GetName()
	-- local  l_Displacement = Position - m_Position
	-- l_Displacement:Normalize(1)

	-- g_PhysXManager():MoveCharacterController(m_Name, l_Displacement*self.m_Speed, ElapsedTime)

	-- if PointInsideCircle(m_Position, Position, self.m_PointRadius) then
		-- return false
	-- end
	
	-- return true
end

function CRangedEnemyComponent:LaunchProjectile(Destination)
	
	--g_PhysXManager:ChangeKinematicState(self.m_Weapon, true)
	--SetParent(CAnimatedInstanceModel* Parent, const std::string &BoneName) AQUI DEBERIA SER NIL
	
	local l_VectorToPlayer = Destination - self.m_LuaGameObject:GetPosition()
	l_VectorToPlayer:Normalize(1.0)
	
	--g_PhysXManager:MoveKinematicActor(self.m_Weapon,l_EnemyPos)
	g_PhysXManager:ChangeKinematicState(self.m_Weapon, false)
	local l_Force = 90
	g_PhysXManager:ChangeGravityState(self.m_Weapon, false)
	g_PhysXManager:ApplyForce(self.m_Weapon, (l_VectorToPlayer*l_Force))
end

function CRangedEnemyComponent:AttachProjectile()
	g_PhysXManager:ChangeKinematicState(self.m_Weapon, true)
	local StartPosition = Vect3f(0.0,0.0,0.0)
	StartPosition = self..m_LuaGameObject:GetPosition()
	local Y1 = 5.0
	local Position = Vect3f(StartPosition.x,Y1,StartPosition.z)
	
	g_PhysXManager:MoveKinematicActor(self.m_Weapon,Position)
	
	--SetParent(CAnimatedInstanceModel* Parent, const std::string &BoneName)
end