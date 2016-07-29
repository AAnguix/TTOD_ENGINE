class 'CEnemyComponent' (CLUAComponent)
function CEnemyComponent:__init(CLuaGameObject,EnemyType)
	CLUAComponent.__init(self,CLuaGameObject:GetName().."_"..EnemyType.."Script")
	self.m_LuaGameObject = CLuaGameObject
	self.m_Health = 100.0
	self.m_MaxHealth = 100.0
	
	self.m_Armor = nil
	self.m_Weapon = nil
	
	self.m_Speed = 0.0
	self.m_Velocity = Vect3f(0.0,0.0,0.0)
	self.m_AttackDelay = 0.0
	self.m_VisionRange = 0.0
	self.m_RotationVelocity = 5.0
	
	self.m_DelayToPatrol = 2.0
	self.m_WayPoints = {}
	self.m_CurrentWayPoint = 1
	
	--Character collider
	self.m_Height = 1.4
	self.m_Density = 30
	self.m_Radius = 0.3
	
	--Movements
	self.M_FollowWaypoints = false
	self.M_AttackMovement = false
	
	--Conditionals
	self.m_Attacking = false
	self_m_GotHit = false
	
	self.m_GuiAvatar = nil
	self.m_DieDelay = 3.0
end

function CEnemyComponent:GetAvatar() return self.m_GuiAvatar end

function CEnemyComponent:Initialize()

	-- local l_GameObject = self.m_LuaGameObject:GetGameObject()
	local l_ComponentName = self.m_LuaGameObject:GetName().."_CharacterCollider"
	g_PhysXManager:AddCharacterColliderComponent(self.m_LuaGameObject:GetName(), self.m_LuaGameObject, self.m_Height, self.m_Radius, self.m_Density)
	
	local l_ACName = self.m_LuaGameObject:GetName().."_AnimatorController"
	g_AnimatorControllerManager:AddComponent(l_ACName, self.m_LuaGameObject)
	
	g_LogManager:Log("Enemy "..self.m_LuaGameObject:GetName().." created...")
end

function CEnemyComponent:Update(ElapsedTime)
	self:MovementController(ElapsedTime)
	-- if self.m_Health<=0.0 then
		-- self.m_DieDelay = self.m_DieDelay - ElapsedTime
		-- if self.m_DieDelay<=0.0 then
			-- g_GameController:RemoveEntity(self.m_LuaGameObject:GetName())
		-- end
	-- end
end

function CEnemyComponent:GetHealth() return self.m_Health end
function CEnemyComponent:GetMaxHealth() return self.m_MaxHealth end
function CEnemyComponent:GetSpeed()	return self.m_Speed end
function CEnemyComponent:GetVisionRange() return self.m_VisionRange end
function CEnemyComponent:GetAttackDelay() return self.m_AttackDelay end
function CEnemyComponent:GetArmor()	return self.m_Armor end
function CEnemyComponent:SetArmor(Armor) self.m_Armor = Armor end
function CEnemyComponent:GetWeapon() return self.m_Weapon end
function CEnemyComponent:SetWeapon(Weapon) self.m_Weapon = Weapon end
function CEnemyComponent:GetLuaGameObject() return self.m_LuaGameObject end
function CEnemyComponent:IsAttacking() return self.m_Attacking end
function CEnemyComponent:SetAttacking(state) self.m_Attacking = state end
function CEnemyComponent:IsHit() return self.m_GotHit end
function CEnemyComponent:SetHitState(state) self.m_GotHit = state end

function CEnemyComponent:AddWaypoint(WayPoint)
	table.insert(self.m_WayPoints,WayPoint)
end

function CEnemyComponent:TakeDamage(PlayerWeapon)
	--self.m_AudioSource:PlayEvent("SonidoDePrueba")
	if g_ShowHealthBarManager ~= nil then
		g_ShowHealthBarManager:SetLastEnemyHit(self)
		g_EventManager:FireEvent("ENEMY_TAKES_DAMAGE")
	end
	local l_Armor = ""
	if self.m_Armor ~= nil then l_Armor = self.m_Armor:GetType() end
	local l_PlayerDamage = PlayerWeapon:GetDamage()
	local l_DamageCalculated = g_DamageCalculator:CalculateDamage(l_Armor,PlayerWeapon,l_PlayerDamage)

	if self.m_Health >= 0.0 then
		if((self.m_Health - l_DamageCalculated)<0.0) then
			self.m_Health = 0.0
			local l_Name = self.m_LuaGameObject:GetName()
			g_GameController:RemoveEnemy(l_Name)
			g_GameController:RemoveEntity(l_Name)
		else
			self.m_Health = self.m_Health - l_DamageCalculated
		end
	end
end

function CEnemyComponent:CalculateNewAngle(Angle, CurrentYaw, Velocity, ElapsedTime)
	local l_Result = 0.0
	if Angle < 0.0 then
		if (Velocity*(-1)*ElapsedTime) < Angle then
			l_Result = CurrentYaw + (Angle*ElapsedTime)
		else l_Result = CurrentYaw + (Velocity*(-1)*ElapsedTime)
		end
	else
		if (Velocity*ElapsedTime) > Angle then
			l_Result = CurrentYaw + (Angle*ElapsedTime)
		else l_Result = CurrentYaw + (Velocity*ElapsedTime)
		end
	end
	return l_Result
end

function CEnemyComponent:LookAtPoint(Point, ElapsedTime)
	local l_Forward = self.m_LuaGameObject:GetForward()
	l_Forward.y = 0.0
	local l_Angle = CTTODMathUtils.GetAngleToFacePoint(l_Forward, self.m_LuaGameObject:GetPosition(), Point)	
	-- -k*T    si el ángulo es negativo. K es la velocidad
	--  si -k*T < angulo, aplicas el angulo, y no la formula, para que no se pase de rotación
	local l_CurrentYaw = self.m_LuaGameObject:GetYaw()
	local l_Velocity = self.m_RotationVelocity
	self.m_LuaGameObject:SetYaw(self:CalculateNewAngle(l_Angle, l_CurrentYaw, l_Velocity, ElapsedTime))
end

function CEnemyComponent:MovementController(ElapsedTime)
	local l_Name = self.m_LuaGameObject:GetName()
	self.m_Velocity.x = 0.0
	self.m_Velocity.z = 0.0
	
	if self.M_FollowWaypoints then
		self:FTWP(ElapsedTime)
	-- elseif self.M_AttackMovement then
		-- self:MoveWithAStar(ElapsedTime)
	end
	
	self.m_Velocity = self.m_Velocity + (g_Gravity*ElapsedTime)
	if ElapsedTime>0.0 then
		self.m_Velocity = g_PhysXManager:DisplacementCharacterController(l_Name, (self.m_Velocity * ElapsedTime), ElapsedTime)
	else 
		self.m_Velocity = g_PhysXManager:DisplacementCharacterController(l_Name, (self.m_Velocity), ElapsedTime)
	end 
end
function CEnemyComponent:SetFTWP(Value) self.M_FollowWaypoints = Value end
function CEnemyComponent:SetMAM(Value) self.M_AttackMovement = Value end


function CEnemyComponent:FollowTriangleWayPoints(ElapsedTime)
	m_Position = self.m_LuaGameObject:GetPosition()
	local l_Name = self.m_LuaGameObject:GetName()
	
	if (#self.m_WayPoints)>0 then
		local l_Destiny = Vect3f(0.0,0.0,0.0)
		l_Destiny = self.m_WayPoints[self.m_CurrentWayPoint]
		local l_Vector =  l_Destiny - m_Position
		
		self.m_Velocity.x = 0.0
		self.m_Velocity.z = 0.0
		
		if CTTODMathUtils.PointInsideCircle(l_Destiny, m_Position, 0.2) == false then
			l_Vector:Normalize(1)
			self.m_Velocity = self.m_Velocity + (l_Vector*self:GetSpeed())
			self.m_Velocity = self.m_Velocity + (g_Gravity*ElapsedTime)
			self.m_Velocity = g_PhysXManager:DisplacementCharacterController(l_Name,(self.m_Velocity * ElapsedTime * self.m_Speed), ElapsedTime)
			self:LookAtPoint(l_Destiny, ElapsedTime)
		else
			self.m_CurrentWayPoint = self.m_CurrentWayPoint+1
		end
		
		if self.m_CurrentWayPoint > (#self.m_WayPoints) then
			self.m_CurrentWayPoint = 1
		end
	--else g_LogManager:Log(self.m_LuaGameObject:GetName().." doesn't have waypoints")
	end
end

function CEnemyComponent:FTWP(ElapsedTime)
	local l_Position = self.m_LuaGameObject:GetPosition()
	local l_Name = self.m_LuaGameObject:GetName()
	
	if (#self.m_WayPoints)>0 then
		--g_LogManager:Log("CurrentWayPoint"..self.m_CurrentWayPoint)
		local l_Destiny = Vect3f(0.0,0.0,0.0)
		l_Destiny = self.m_WayPoints[self.m_CurrentWayPoint]
		local l_Vector =  l_Destiny - l_Position
		
		if CTTODMathUtils.PointInsideCircle(l_Destiny, l_Position, 0.2) == false then
			--g_LogManager:Log("fuera")
			l_Vector:Normalize(1)
			self.m_Velocity = self.m_Velocity + (l_Vector*self:GetSpeed())
			--self.m_Velocity = self.m_Velocity + (g_Gravity*ElapsedTime)
			--self.m_Velocity = g_PhysXManager:DisplacementCharacterController(l_Name,(self.m_Velocity * ElapsedTime * self.m_Speed), ElapsedTime)
			self:LookAtPoint(l_Destiny, ElapsedTime)
		else
			--g_LogManager:Log("dentro")
			self.m_CurrentWayPoint = self.m_CurrentWayPoint+1
		end
		
		if self.m_CurrentWayPoint > (#self.m_WayPoints) then
			self.m_CurrentWayPoint = 1
		end
	end
end

function CEnemyComponent:MTPNP(ElapsedTime)
	
	local l_Enemy = self.m_LuaGameObject
	local l_EnemyPos = l_Enemy:GetPosition()
	local l_PlayerPos = g_Player:GetPosition()
	
	local l_VectorToPlayer = l_PlayerPos - l_EnemyPos
	l_VectorToPlayer:Normalize(1.0)
	
	local l_Forward = l_Enemy:GetForward()
	l_Forward.y = 0.0
	
	local l_NewAngle = 0.0 
	
	self.m_Velocity.x = 0.0
	self.m_Velocity.z = 0.0
	self.m_Velocity = self.m_Velocity + (l_VectorToPlayer*self:GetSpeed())
	--self.m_Velocity = self.m_Velocity + (g_Gravity*ElapsedTime)
	if ElapsedTime>0.0 then
		--self.m_Velocity = g_PhysXManager:DisplacementCharacterController(l_Enemy:GetName(), (self.m_Velocity * ElapsedTime), ElapsedTime)
		self:LookAtPoint(l_PlayerPos,ElapsedTime)
	else 
		--self.m_Velocity = g_PhysXManager:DisplacementCharacterController(l_Enemy:GetName(), (self.m_Velocity), ElapsedTime)
		self:LookAtPoint(l_PlayerPos,0.0)
	end 
end

function CEnemyComponent:MoveWithAStar(ElapsedTime)

	local l_Enemy = self.m_LuaGameObject
	local l_EnemyPos = l_Enemy:GetPosition()
	local l_PlayerPos = g_Player:GetPosition()

	local l_Points = g_AIManager:SearchAStarPath(l_EnemyPos,l_PlayerPos,false)
	
	if (l_Points~= nil and l_Points:size() >= 0) then
	
		g_LogManager:Log("Points:")
		--g_LogManager:Log(l_Points:at(0))
		--g_LogManager:Log(l_EnemyPos)
		
		local l_VectorToPlayer = l_Points:at(0) - l_EnemyPos
		l_VectorToPlayer:Normalize(1.0)
		
		g_LogManager:Log(l_VectorToPlayer)
		
		self.m_Velocity.x = 0.0
		self.m_Velocity.z = 0.0
		self.m_Velocity = self.m_Velocity + (l_VectorToPlayer*self:GetSpeed())
		
		if ElapsedTime>0.0 then
			self:LookAtPoint(l_PlayerPos,ElapsedTime)
		else 
			self:LookAtPoint(l_PlayerPos,0.0)
		end 
	end
end

function CEnemyComponent:IsThisOK() return g_LogManager:Log("Pupu de negro") end
