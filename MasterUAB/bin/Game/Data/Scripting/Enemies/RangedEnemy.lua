class 'CRangedEnemyComponent' (CEnemyComponent)
function CRangedEnemyComponent:__init(CGameObject)
	CEnemyComponent.__init(self, CGameObject, "RangedEnemy")
	self.m_Health=140.0
	self.m_MaxHealth=self.m_Health
	self.m_Speed=4.0
	self.m_AttackDelay=2.0
	self.m_VisionRange=12.0
	self.m_PointRadius=0.66
	self.m_EscapeRadius = 4.5
	self.m_EscapeDistance= 4.0
	self.m_LastTimeFrightened = false
	self.m_ShootingForce= 150.0
	
	self.m_SpawnBallDelay = 1.5
	self.m_LaunchBallDelay = 0.8
	
	self.m_GuiAvatar = "basic_enemy_avatar_image"
	
	--self.m_Weapon = "Cristalmanec"
	--self.m_JumpDestination = nil
	self.m_JumpDestination = nil
	--self.m_Weapon = CWeapon(100,"bow")
	--self.m_Armor = CArmor(5,"light")
	self.EnergyBallThrowed = false
end

function CRangedEnemyComponent:Attack()
	
end

function CRangedEnemyComponent:Initialize()
	
	CEnemyComponent.Initialize(self)
	
	self.m_LuaGameObject:AddSound("OrcShamanJump","Play_OrcShamanJump")
	self.m_LuaGameObject:AddSound("OrcShamanAttack","Play_OrcShamanAttack")
	self.m_LuaGameObject:AddSound("OrcDead","Play_OrcShamanDead")
	
	local l_Idle = self.m_LuaGameObject:AddState("Idle_State", "idle", 1.0, "OnEnter_Idle_RangedEnemy", "OnUpdate_Idle_RangedEnemy", "OnExit_Idle_RangedEnemy")
	local l_Attack = self.m_LuaGameObject:AddState("Attack_State", "normalAttack", 1.0, "OnEnter_Attack_RangedEnemy", "OnUpdate_Attack_RangedEnemy", "OnExit_Attack_RangedEnemy")
	local l_RunAway = self.m_LuaGameObject:AddState("RunAway_State", "walk", 1.0, "OnEnter_RunAway_RangedEnemy", "OnUpdate_RunAway_RangedEnemy", "OnExit_RunAway_RangedEnemy")
	local l_Frightened = self.m_LuaGameObject:AddState("Frightened_State", "injured", 1.0, "OnEnter_Frightened_RangedEnemy", "OnUpdate_Frightened_RangedEnemy", "OnExit_Frightened_RangedEnemy")
	local l_Dead = self.m_LuaGameObject:AddState("Dead_State", "die", 1.0, "OnEnter_Dead_RangedEnemy", "OnUpdate_Dead_RangedEnemy", "OnExit_Dead_RangedEnemy")
	
	self.m_LuaGameObject:AddBool("IsPlayerInsideVisionRange", false)
	--self.m_LuaGameObject:AddBool("AtackPlayer1", false)
	
	self.m_LuaGameObject:AddTrigger("PlayerInsideEscapeRange", false)
	self.m_LuaGameObject:AddTrigger("AttackPlayer", false)
	self.m_LuaGameObject:AddTrigger("Frightened", false)
	self.m_LuaGameObject:AddBool("Dead", false)
	
	local l_IdleToAttack = l_Idle:AddTransition("IdleToAttack",l_Attack, false, 0.1, 0.1)
	l_IdleToAttack:AddTriggerCondition("AttackPlayer")

	local l_IdleToRunAway = l_Idle:AddTransition("IdleToRunAway",l_RunAway, false, 0.1,0.2)
	l_IdleToRunAway:AddTriggerCondition("PlayerInsideEscapeRange")
	
	local l_AttackToIdle = l_Attack:AddTransition("AttackToIdle", l_Idle, true, 0.1)
	
	local l_RunAwayToAttack = l_RunAway:AddTransition("RunAwayToAttack",l_Attack, true, 0.1, 0.1)
	
	
	local l_RunAwayToFrightened = l_RunAway:AddTransition("RunAwayToFrightened",l_Frightened, false, 0.1, 0.1)
	l_RunAwayToFrightened:AddTriggerCondition("Frightened")
	
	local l_FrightenedToIdle = l_Frightened:AddTransition("FrightenedToIdle",l_Idle, true, 0.1)
	
	local l_AnyToDead = self.m_LuaGameObject:AddAnyStateTransition("AnyStateToDead",l_Dead, false, 0.1, 0.1)
	l_AnyToDead:AddBoolCondition("Dead", true)
	
	local l_DeadToIdle = l_Dead:AddTransition("DeadToIdle",l_Idle, true, 0.1)
	l_DeadToIdle:AddBoolCondition("Dead", false)
end

function CRangedEnemyComponent:Update(ElapsedTime)
	CEnemyComponent.Update(self,ElapsedTime)
	self:MovementController(ElapsedTime)
end

function CRangedEnemyComponent:MovementController(ElapsedTime)
	
	local l_Name = self.m_LuaGameObject:GetName()
	
	self.m_Velocity.x = 0.0
	self.m_Velocity.z = 0.0
	
	if self.M_AttackMovement then
		self:MoveToPoint(ElapsedTime)
	end
	
	self.m_Velocity = self.m_Velocity + Vect3f(0.0,-10.0,0.0)
	
	if ElapsedTime>0.0 then
		self.m_Velocity = g_PhysXManager:DisplacementCharacterController(l_Name, (self.m_Velocity * ElapsedTime), ElapsedTime)
	else 
		self.m_Velocity = g_PhysXManager:DisplacementCharacterController(l_Name, (self.m_Velocity), ElapsedTime)
	end 
end


function CRangedEnemyComponent:CalculateJumpDestination()
	
	self.m_JumpDestination = Vect3f(0.0,0.0,0.0)
	local l_PlayerPos = g_Player:GetPosition()
	local l_PlayerPosCentre = Vect3f(l_PlayerPos.x,(l_PlayerPos.y+0.3),l_PlayerPos.z)
	local l_EnemyPos = self.m_LuaGameObject:GetPosition()
	local l_EnemyPosCentre = Vect3f(l_EnemyPos.x,(l_EnemyPos.y+0.3),l_EnemyPos.z)
	
	local l_Directions = {}
	l_Directions[0] = self.m_LuaGameObject:GetForward()
	l_Directions[1] = self.m_LuaGameObject:GetForward()*(-1.0)
	l_Directions[2] = self.m_LuaGameObject:GetRight()*(-1.0)
	l_Directions[3] = self.m_LuaGameObject:GetRight()
	
	l_Directions[4] = self.m_LuaGameObject:GetForward() + self.m_LuaGameObject:GetRight()*(-1.0)
	l_Directions[4]:Normalize(1)
	l_Directions[5] = self.m_LuaGameObject:GetForward() + self.m_LuaGameObject:GetRight()
	l_Directions[5]:Normalize(1)
	l_Directions[6] = self.m_LuaGameObject:GetForward()*(-1.0) + self.m_LuaGameObject:GetRight()*(-1.0)
	l_Directions[6]:Normalize(1)
	l_Directions[7] = self.m_LuaGameObject:GetForward()*(-1.0) + self.m_LuaGameObject:GetRight()
	l_Directions[7]:Normalize(1)
	
	local l_FarthestDistance = 0.0
	
	local l_RaycastData =  SRaycastData()
	
	for i=0,#l_Directions do
		
		
		local l_Raycast = g_PhysXManager:Raycast(l_EnemyPosCentre, l_Directions[i], self.m_EscapeDistance, l_RaycastData)
		
		
		if l_Raycast then 
			
			local l_Direction = l_Directions[i] * (l_RaycastData.m_Distance - 0.1) 
			local l_Position = self.m_LuaGameObject:GetPosition() + l_Direction
			local l_Distance = (l_PlayerPos-l_Position):Length()
			
			if l_Distance > l_FarthestDistance then
				l_FarthestDistance = l_Distance
				self.m_JumpDestination = l_Position
			end
			
		else
			
			local l_Direction = l_Directions[i] * self.m_EscapeDistance
			local l_Position = self.m_LuaGameObject:GetPosition() + l_Direction		
			local l_Distance = (l_PlayerPos-l_Position):Length()
	
			if l_Distance > l_FarthestDistance then
				l_FarthestDistance = l_Distance
				self.m_JumpDestination = l_Position
			end
			
		end
		
	end 
	
	
end

function CRangedEnemyComponent:LaunchProjectile(Destiny)
	--g_PhysXManager:ChangeKinematicState(self.m_Weapon, true)
	--SetParent(CAnimatedInstanceModel* Parent, const std::string &BoneName) AQUI DEBERIA SER NIL
	if self.m_Weapon ~= nil then
		local l_Matrix = Mat44f()
		l_Matrix = self.m_Weapon:GetWeaponTransform()
		local l_Position = Vect3f(0.0,0.0,0.0)
		l_Position = l_Matrix:GetWorldPos()
		
		local l_VectorToPlayer = Destiny - l_Position
		l_VectorToPlayer:Normalize(1.0)
		self.m_Weapon:SetParentState(false)
		local l_RaycastData =  SRaycastData()
		local l_Raycast = g_PhysXManager:Raycast(l_Position, l_VectorToPlayer, 30.0, l_RaycastData)
		--g_PhysXManager:MoveKinematicActor(self.m_Weapon,l_EnemyPos)
		g_PhysXManager:ChangeKinematicState(self.m_Weapon:GetName(), false)
		g_PhysXManager:ChangeGravityState(self.m_Weapon:GetName(), false)
		g_PhysXManager:ApplyForce(self.m_Weapon:GetName(), (l_VectorToPlayer*self.m_ShootingForce))
	end
end

function CRangedEnemyComponent:AttachProjectile()
	if self.m_Weapon ~= nil then
		g_PhysXManager:ChangeKinematicState(self.m_Weapon:GetName(), true)
		self.m_Weapon:SetParentState(true)
		self.m_Weapon:EnableProjectileParticles(true)
		self.m_Weapon:GetLuaGameObject():EnableRenderableObject(true)	
	else
		g_LogManager:Log("Can't AttachProjectile to Ranged Enemy, Weapon is a nil value")
	end
end

function CRangedEnemyComponent:MoveToPoint(ElapsedTime)
	
	local l_Enemy = self.m_LuaGameObject
	local l_EnemyPos = l_Enemy:GetPosition()
	local l_PlayerPos = g_Player:GetPosition()
	
	
	local l_VectorToDestination = self.m_JumpDestination - l_EnemyPos
	l_VectorToDestination:Normalize(1.0)
	
	local l_Forward = l_Enemy:GetForward()
	l_Forward.y = 0.0
	
	local l_NewAngle = 0.0 
	
	self.m_Velocity.x = 0.0
	self.m_Velocity.z = 0.0
	self.m_Velocity = self.m_Velocity + (l_VectorToDestination*(self:GetSpeed()))
	--self.m_Velocity = self.m_Velocity + (g_Gravity*ElapsedTime)
	if ElapsedTime>0.0 then
		--self.m_Velocity = g_PhysXManager:DisplacementCharacterController(l_Enemy:GetName(), (self.m_Velocity * ElapsedTime), ElapsedTime)
		self:LookAtPoint(l_PlayerPos,ElapsedTime)
	else 
		--self.m_Velocity = g_PhysXManager:DisplacementCharacterController(l_Enemy:GetName(), (self.m_Velocity), ElapsedTime)
		self:LookAtPoint(l_PlayerPos,0.0)
	end 
end