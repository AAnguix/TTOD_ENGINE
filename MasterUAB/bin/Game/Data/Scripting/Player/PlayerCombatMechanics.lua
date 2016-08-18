-- Prevents player from hitting multiple times an enemy in a single attack
function CPlayerComponent:IsAttacking() return self.m_Attacking end
function CPlayerComponent:SetAttacking(state) self.m_Attacking = state end

-- Disables multiple attacksw input instructions while player is attacking
function CPlayerComponent:IsAttackFinished() return self.m_AttackFinished end
function CPlayerComponent:SetAttackFinished(Value) self.m_AttackFinished = Value end

function CPlayerComponent:TakeDamage(EnemyWeapon)
	--self.m_AudioSource:PlayEvent("SonidoDePrueba")
	local l_Armor = "heroic"
	if( self.m_CurrentArmor~=nil) then
		l_Armor = self.m_CurrentArmor:GetType()
	end
	
	local l_DamageCalculated = g_DamageCalculator:CalculateDamage(l_Armor,EnemyWeapon,EnemyWeapon:GetDamage())
	if self.m_Health > 0.0 then
		if((self.m_Health - l_DamageCalculated)<=0.0) then
			self.m_Health = 0.0
			g_EventManager:FireEvent("PLAYER_IS_DEAD")
		else
			self.m_Health = self.m_Health - l_DamageCalculated
			local l_Camera = g_CameraControllerManager:GetCurrentCameraController()
			local l_CameraRotation = Vect3f(l_Camera:GetYaw(),l_Camera:GetPitch(),0.0)
			l_Camera:StartShaking(l_CameraRotation, 400, 60, -0.15, 0.15, 60, -0.15, 0.15)
		end
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
	local l_ClosestEnemy = self:GetClosestEnemy(g_GameController:GetEnemies())
	local l_Forward = self.m_LuaGameObject:GetForward()
	if l_ClosestEnemy~=nil then
		self:FaceEnemy(l_Forward, l_ClosestEnemy, ElapsedTime)
	else 
		--self:AttackCameraDirection(l_Forward, ElapsedTime)
	end
end

function CPlayerComponent:AttackCameraDirection(Forward, ElapsedTime)
	local l_Angle = CTTODMathUtils.AngleBetweenVectors(self.m_AttackDirection, Forward)  
	local l_CurrentYaw = self.m_LuaGameObject:GetYaw()
	if (math.abs(l_Angle)>self.m_AngleMargin) then
		self.m_LuaGameObject:SetYaw(CTTODMathUtils.CalculateNewAngle(l_Angle, l_CurrentYaw, self.m_RotationVelocity, ElapsedTime))
	end
end

function CPlayerComponent:FaceEnemy(Forward, ClosestEnemy, ElapsedTime)
	local l_EnemyPos = ClosestEnemy:GetLuaGameObject():GetPosition()
	local l_Angle = CTTODMathUtils.GetAngleToFacePoint(Forward, self.m_LuaGameObject:GetPosition(), l_EnemyPos)    
	--g_LogManager:Log("Angle del FaceEnemy: ".. l_Angle)
	local l_CurrentYaw = self.m_LuaGameObject:GetYaw()
	self.m_LuaGameObject:SetYaw(CTTODMathUtils.CalculateNewAngle(l_Angle, l_CurrentYaw, self.m_RotationVelocity, ElapsedTime))
end

function CPlayerComponent:FaceAttackDirection(ElapsedTime)
	local l_CurrentYaw = self.m_LuaGameObject:GetYaw()
	local l_Angle = g_CameraControllerManager:GetCurrentCameraController():GetYaw() - l_CurrentYaw
	self.m_LuaGameObject:SetYaw(CTTODMathUtils.CalculateNewAngle(l_Angle, l_CurrentYaw, self.m_RotationVelocity, ElapsedTime))
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
        g_LogManager:Log("Pos Hueso")
        g_LogManager:Log(ObjectPos)
        g_LogManager:Log("Pos Player")
        g_LogManager:Log(l_PlayerPos)
        g_LogManager:Log("Pos+ Direccion")
        g_LogManager:Log(l_Position)
        g_LogManager:Log("Distancia: "..l_Distance)
    --if l_Distance > l_FarthestDistance then
        if l_Distance < l_FarthestDistance then
            l_FarthestDistance = l_Distance
            --self.m_TossedDirection = l_Directions[i]*3.0
            local Ray =  SRaycastData()
            local Raycast = g_PhysXManager:Raycast(self.m_LuaGameObject:GetPosition(), l_Directions[i]*(-1.0), 15.0, Ray)
            self.m_TossedDirection = (l_Directions[i]*(-1.0))*3.0
        end
        
    end
        
    local Ray =  SRaycastData()
    local Raycast = g_PhysXManager:Raycast(self.m_LuaGameObject:GetPosition(), self.m_TossedDirection , 40.0, Ray)
end