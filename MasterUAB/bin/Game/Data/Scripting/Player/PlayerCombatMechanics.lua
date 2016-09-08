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
	if self.m_Health > 0.0 then
		if((self.m_Health - l_DamageCalculated)<=0.0) then
			self.m_Health = 0.0
			self:Die()
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
	local l_Forward = self.m_LuaGameObject:GetForward():Normalize(1)
	if l_ClosestEnemy~=nil then
		self:FaceEnemy(l_Forward, l_ClosestEnemy, ElapsedTime)
	else 
		-- local l_AttackDirection = g_CameraControllerManager:GetCurrentCameraController():GetForward():Normalize(1)
		self:FaceDirection(l_Forward, self.m_AttackDirection, ElapsedTime)
	end
end

function CPlayerComponent:FaceDirection(Forward, NewDirection, ElapsedTime)
	local l_Angle = CTTODMathUtils.AngleBetweenVectors(NewDirection, Forward)  
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