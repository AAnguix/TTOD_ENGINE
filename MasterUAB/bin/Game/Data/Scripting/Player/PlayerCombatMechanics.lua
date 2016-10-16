-- Prevents player from hitting multiple times an enemy in a single attack
function CPlayerComponent:IsAttacking() return self.m_Attacking end
function CPlayerComponent:IsBlocking() return self.m_Blocking end
function CPlayerComponent:IsInteracting() return self.m_Interacting end

function CPlayerComponent:SetAttacking(State) self.m_Attacking = State end
function CPlayerComponent:SetBlocking(State) self.m_Blocking = State end
function CPlayerComponent:SetInteracting(State) self.m_Interacting = State end

-- Disables multiple attacksw input instructions while player is attacking
function CPlayerComponent:IsAttackFinished() return self.m_AttackFinished end
function CPlayerComponent:SetAttackFinished(Value) self.m_AttackFinished = Value end

-- Check if an enemy is close to switch to Combat Idle
function CPlayerComponent:CheckForSurroundingEnemies()
	local l_Enemies = g_GameController:GetEnemies()
	for i=1, (#l_Enemies) do
		local l_EnemyPos = l_Enemies[i]:GetLuaGameObject():GetPosition()
		local l_Distance = (l_EnemyPos-self.m_LuaGameObject:GetPosition()):Length()
		if (l_Distance < self.m_DistanceToEnterCombatIdle) then
			self.m_LuaGameObject:SetBool("SurroundingEnemies", true)
			return true
		end
	end
	self.m_LuaGameObject:SetBool("SurroundingEnemies", false) 
end

function CPlayerComponent:TakeDamage(EnemyWeapon)
	--self.m_AudioSource:PlayEvent("SonidoDePrueba")
	local l_Armor = "heroic"
	if( self.m_CurrentArmor~=nil) then
		l_Armor = self.m_CurrentArmor:GetType()
	end
	local l_DamageCalculated = g_DamageCalculator:CalculateDamage(l_Armor,EnemyWeapon,EnemyWeapon:GetDamage())
	self:TakeSimpleDamage(l_DamageCalculated)
end

function CPlayerComponent:TakeSimpleDamage(Damage)
	if self.m_Health > 0.0 then
		if((self.m_Health - Damage)<=0.0) then
			self.m_Health = 0.0
			self:Die()
		else
			self.m_Health = self.m_Health - Damage
			if(self.m_FullHealth) then
				self.m_FullHealth = false
			end
			local l_Camera = g_CameraControllerManager:GetCurrentCameraController()
			local l_CameraRotation = Vect3f(l_Camera:GetYaw(),l_Camera:GetPitch(),0.0)
			l_Camera:StartShaking(l_CameraRotation, 400, 60, -0.15, 0.15, 60, -0.15, 0.15)
		end
	end
end

function CPlayerComponent:SetBurningState()
    local l_RenderableObjectTechnique = g_RenderableObjectTechniqueManager:GetResource("burning_character_technique")
    self.m_LuaGameObject:SetTemporalRenderableObjectTechnique(l_RenderableObjectTechnique)    
    self.m_BurningState = true
    self.m_BurningStateCounter = 0.0
end   

function CPlayerComponent:SetBurningStateTimer(ElapsedTime)
    g_LogManager:Log("SetBurningStateTimer: ")
    self.m_BurningStateCounter = self.m_BurningStateCounter + ElapsedTime
    if (self.m_BurningStateCounter >= self.m_BurningStateTime) then
         self.m_LuaGameObject:SetTemporalRenderableObjectTechnique(nil)
         self.m_BurningState = false
	end
end 

function CPlayerComponent:GetClosestEnemy(Enemies)
	local l_ClosestEnemy = nil
	local l_MinDistance = 0.0
	for i=1, (#Enemies) do
		local l_EnemyPos = Enemies[i]:GetLuaGameObject():GetPosition()
		local l_Distance = (l_EnemyPos-self.m_LuaGameObject:GetPosition()):Length()
		if ( (l_Distance < self.m_AttackingFacingDistance)and((l_MinDistance==0.0)or(l_Distance <= l_MinDistance)) ) then
			l_ClosestEnemy = Enemies[i]
			l_MinDistance = l_Distance
		end
	end
	return l_ClosestEnemy
end

function CPlayerComponent:CalculateAttackDirection(ElapsedTime)

	if (not self.m_FacingFinished) then
		self:FaceDirection(ElapsedTime)
	end
end

function CPlayerComponent:FaceDirection(ElapsedTime)
	self.m_FacingFinished = CTTODMathUtils.FaceDirection(self, self.m_LuaGameObject, self.m_LuaGameObject:GetYaw(), self.m_YawBeforeFacing, 
	self.m_YawAfterFacing, self.m_RotationAngle, self.m_RotationDuration, self.m_AngleMargin)
end

-- function CPlayerComponent:FaceDirection(ElapsedTime)
	-- if(self.m_RotationDuration == 0.0) then
		-- g_LogManager:Log("Error. RotationDuration is 0")
	-- end
		
	-- local l_Pi = 3.14159265359
	-- local l_Pi_2 = (l_Pi/2.0)
	
	-- local ThreeQuarter = l_Pi + l_Pi_2
	-- local l_CurrentYaw = self.m_LuaGameObject:GetYaw()
	-- local l_Difference = self.m_YawAfterFacing - self.m_LuaGameObject:GetYaw()
	
	-- if((l_CurrentYaw>ThreeQuarter)and(self.m_YawAfterFacing<(l_Pi/2.0))) then
		-- l_Difference = (2*l_Pi) - l_CurrentYaw + self.m_YawAfterFacing
	-- elseif((l_CurrentYaw<0.0) and(self.m_YawAfterFacing<0.0)) then
		-- l_Difference = math.abs(math.abs(l_CurrentYaw) - math.abs(self.m_YawAfterFacing))
	-- end
	
	-- l_Difference = math.abs(l_Difference)
	
	-- if (l_Difference>self.m_AngleMargin) then
		-- local l_Angle = (self:GetTimer()*self.m_RotationAngle)/self.m_RotationDuration
		-- local l_NewYaw = CTTODMathUtils.GetFixedAngle(self.m_YawBeforeFacing + l_Angle)
		-- if(l_NewYaw<0.0 and self.m_YawAfterFacing<0.0) then
			-- if(self.m_RotationAngle<0.0 and l_NewYaw<self.m_YawAfterFacing)then
				-- self.m_FacingFinished = true
				-- self.m_LuaGameObject:SetYaw(self.m_YawAfterFacing)
				-- return true
			-- elseif(self.m_RotationAngle>0.0 and l_NewYaw>self.m_YawAfterFacing)then
				-- self.m_LuaGameObject:SetYaw(self.m_YawAfterFacing)
				-- self.m_FacingFinished = true
				-- return true
			-- end
		-- elseif(l_NewYaw>0.0 and self.m_YawAfterFacing>0.0) then
			-- if(self.m_RotationAngle<0.0 and l_NewYaw<self.m_YawAfterFacing)then
				-- self.m_FacingFinished = true
				-- self.m_LuaGameObject:SetYaw(self.m_YawAfterFacing)
				-- return true
			-- elseif(self.m_RotationAngle>0.0 and l_NewYaw>self.m_YawAfterFacing)then
				-- self.m_LuaGameObject:SetYaw(self.m_YawAfterFacing)
				-- self.m_FacingFinished = true
				-- return true
			-- end
		-- end
		
		-- self.m_LuaGameObject:SetYaw(l_NewYaw)
	-- else 
		-- self.m_LuaGameObject:SetYaw(self.m_YawAfterFacing)
		-- self.m_FacingFinished = true
	-- end
-- end

function CPlayerComponent:FaceEnemy(Forward, ClosestEnemy, ElapsedTime)
	local l_EnemyPos = ClosestEnemy:GetLuaGameObject():GetPosition()
	local l_Angle = CTTODMathUtils.GetAngleToFacePoint(Forward, self.m_LuaGameObject:GetPosition(), l_EnemyPos)    
	if (math.abs(l_Angle)>self.m_AngleMargin) then
		local l_CurrentYaw = self.m_LuaGameObject:GetYaw()
		self.m_LuaGameObject:SetYaw(CTTODMathUtils.CalculateNewAngle(l_Angle, l_CurrentYaw, self.m_RotationVelocity, ElapsedTime))
	else 
		self.m_FacingFinished = true
	end
end

function CPlayerComponent:ClosestHit(ObjectPos)
    
    self.m_TossedDirection = Vect3f(0.0,0.0,0.0)
    local l_PlayerPos = self.m_LuaGameObject:GetPosition()
    
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
    
    local l_FarthestDistance = 10.0
    for i=0,#l_Directions do
        
        local l_Position = self.m_LuaGameObject:GetPosition() + l_Directions[i]        
        local l_Distance = (ObjectPos-l_Position):Length()
        --g_LogManager:Log("Pos Hueso")
        --g_LogManager:Log(ObjectPos)
        --g_LogManager:Log("Pos Player")
        --g_LogManager:Log(l_PlayerPos)
        --g_LogManager:Log("Pos+ Direccion")
        --g_LogManager:Log(l_Position)
        --g_LogManager:Log("Distancia: "..l_Distance)
    --if l_Distance > l_FarthestDistance then
        if l_Distance < l_FarthestDistance then
            l_FarthestDistance = l_Distance
            --self.m_TossedDirection = l_Directions[i]*3.0
            local Ray =  SRaycastData()
            local Raycast = g_PhysXManager:Raycast(self.m_LuaGameObject:GetPosition(), l_Directions[i]*(-1.0), 5.0, Ray)
            self.m_TossedDirection = (l_Directions[i]*(-1.0))*3.0
			
        end
        
    end
    g_LogManager:Log("m_TossedDirection en ClosestHit")
	g_LogManager:Log(self.m_TossedDirection)    
    local Ray2 =  SRaycastData()
    local Raycast = g_PhysXManager:Raycast(self.m_LuaGameObject:GetPosition(), self.m_TossedDirection , 40.0, Ray2)
end