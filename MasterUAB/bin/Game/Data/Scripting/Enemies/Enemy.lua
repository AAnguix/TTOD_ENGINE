class 'CEnemyComponent' (CLUAComponent)
function CEnemyComponent:__init(CRenderableObject, ComponentType)
	CLUAComponent.__init(self,ComponentType)
	self.m_RObject = CRenderableObject
	self.m_Dead=false
	self.m_Health=0.0
	
	self.m_Armor = nil
	self.m_Weapon = nil
	
	self.m_Speed = 0.1
	self.m_Velocity = Vect3f(0.0,0.0,0.0)
	self.m_AttackDelay = 0.0
	self.m_VisionRange = 0.0
	
	self.m_DelayToPatrol = 2.0
	self.m_WayPoints = {}
	self.m_CurrentWayPoint = 1
	self.m_ElapsedTime = 0.0
	
	self.m_Height = 1.4
end

function CEnemyComponent:Initialize()
	--Physyx
	CCharacterCollider.AddCharacterCollider("CharacterCollider", self.m_RObject)
	--l_MaterialName = self.m_RObject:GetAnimatedCoreModel():GetMaterials()[0]:GetName()
	l_MaterialName = "aaa"
	g_PhysXManager:RegisterMaterial(l_MaterialName, 0.1, 0.1, 0.1)
	m_Position = self.m_RObject:GetPosition()
	l_CControlerPos = Vect3f(m_Position.x, m_Position.y, m_Position.z)
	g_PhysXManager:CreateCharacterController(self.m_RObject:GetName(), self.m_Height, 0.3, 30.0, l_CControlerPos, l_MaterialName)
	
	--Animations
	CAnimatorController.AddAnimatorController("AnimatorController", self.m_RObject)
	
	g_LogManager:Log("Enemy "..self.m_RObject:GetName().." created...")
end

function CEnemyComponent:Update(ElapsedTime)
end

function CEnemyComponent:ResetTime()
	self.m_ElapsedTime=0.0
end

function CEnemyComponent:IsDead()
	return self.m_Dead
end
function CEnemyComponent:GetHealth()
	return self.m_Health
end
function CEnemyComponent:GetSpeed()
	return self.m_Speed
end
function CEnemyComponent:GetVisionRange()
	return self.m_VisionRange
end
function CEnemyComponent:GetAttackDelay()
	return self.m_AttackDelay
end
function CEnemyComponent:GetArmor()
	return self.m_Armor 
end
function CEnemyComponent:GetWeapon()
	return self.m_Weapon 
end
function CEnemyComponent:GetRenderableObject()
	return self.m_RObject
end
function CEnemyComponent:AddWaypoint(WayPoint)
	table.insert(self.m_WayPoints,WayPoint)
end

function CEnemyComponent:TakeDamage(PlayerWeapon, PlayerDamage)
	self.m_Health = self.m_Health - Damage
end

function CEnemyComponent:Attack()

end
-- function CEnemyComponent:GetElapsedTime()

function CEnemyComponent:IsPlayerInsideVisionRange(PlayerPosition)

	local l_Distance = 0.0
	local l_Position = self.m_RObject:GetPosition()
	
	l_Distance = math.sqrt((l_Position.x - PlayerPosition.x)^2 + (l_Position.y - PlayerPosition.y)^2 + (l_Position.z - PlayerPosition.z)^2)
	
	-- local s = ""
	-- s = l_Distance.." distance"
	-- g_LogManager:Log(s)
	
	if l_Distance < self.m_VisionRange then
		return true
	end
	
	return false
end

function CEnemyComponent:FollowTriangleWayPoints(ElapsedTime)
	m_Position = self.m_RObject:GetPosition()
	local l_Name = self.m_RObject:GetName()
	
	if (#self.m_WayPoints)>0 then
		local l_Destiny = Vect3f(0.0,0.0,0.0)
		l_Destiny = self.m_WayPoints[self.m_CurrentWayPoint]
		local l_Vector =  l_Destiny - m_Position
		
		self.m_Velocity.x = 0.0
		self.m_Velocity.z = 0.0
		
		if IsPointInsideCircunference(m_Position, l_Destiny, 0.2) == false then
			l_Vector:Normalize(1)
			--local l_VS = l_Vector*self:GetSpeed()
			self.m_Velocity = self.m_Velocity + (l_Vector*self:GetSpeed())
			self.m_Velocity = self.m_Velocity + (g_Gravity*ElapsedTime)
			
			local l_Displacement = self.m_Velocity * ElapsedTime
			self.m_Velocity = g_PhysXManager:DisplacementCharacterController(l_Name, l_Displacement, ElapsedTime)
			
			local l_Forward = self.m_RObject:GetForward()
			l_Forward.y = 0.0
			l_Destiny.y = 0.0
			local l_Angle = CTTODMathUtils.GetAngleToFacePoint(l_Forward, self.m_RObject:GetPosition(), l_Destiny)
			
			local l_CurrentYaw = self.m_RObject:GetYaw()
			
			self.m_RObject:SetYaw( l_CurrentYaw + l_Angle)
		
		else
			self.m_CurrentWayPoint = self.m_CurrentWayPoint+1
		end
		
		if self.m_CurrentWayPoint > (#self.m_WayPoints) then
			self.m_CurrentWayPoint = 1
		end
	--else g_LogManager:Log(self.m_RObject:GetName().." doesn't have waypoints")
	end
end
