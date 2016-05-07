class 'CEnemyComponent' (CLUAComponent)
function CEnemyComponent:__init(CRenderableObject)
	
	self.m_RObject = CRenderableObject
	self.m_Dead=false
	self.m_Health=0.0
	
	self.m_Armor = nil
	self.m_Weapon = nil
	
	self.m_Speed = 0.0
	self.m_AttackDelay = 0.0
	self.m_VisionRange = 0.0
	
	self.m_DelayToPatrol = 0.0
	self.m_WayPoints = {}
	self.m_CurrentWayPoint = 0
	self.m_ElapsedTime = 0.0
	
	self.m_Height = 1.4
	
	--Physyx
	CCharacterCollider.AddCharacterCollider("CharacterCollider", CRenderableObject)
	--l_MaterialName = CRenderableObject:GetAnimatedCoreModel():GetMaterials()[0]:GetName()
	l_MaterialName = "aaa"
	g_PhysXManager:RegisterMaterial(l_MaterialName, 0.1, 0.1, 0.1)
	m_Position = CRenderableObject:GetPosition()
	l_CControlerPos = Vect3f(m_Position.x, m_Position.y, m_Position.z)
	g_PhysXManager:CreateCharacterController(CRenderableObject:GetName(), self.m_Height, 0.3, 30.0, l_CControlerPos, l_MaterialName)
	
	--Animations
	CAnimatorController.AddAnimatorController("AnimatorController", CRenderableObject)
	
	g_LogManager:Log("Enemy "..CRenderableObject:GetName().." created...")
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

-- end

function CEnemyComponent:IsPlayerInsideVisionRange(PlayerPosition)

	l_Distance = 0.0
	l_IsPlayerInsideVisionRange = false
	l_Position = GetPosition()
	
	l_Distance = sqrt(pow(l_Position.x - PlayerPosition.x, 2) + pow(l_Position.y - PlayerPosition.y, 2) + pow(l_Position.z - PlayerPosition.z, 2))

	if l_Distance < self.m_VisionRange then
		l_IsPlayerInsideVisionRange = true
	end
	
	return l_IsPlayerInsideVisionRange
end

function CEnemyComponent:FollowTriangleWayPoints(ElapsedTime)

	m_Position = self.m_RObject:GetPosition()
	
	if (#self.m_WayPoints)>1 then
		l_Vector = self.m_WayPoints[self.m_CurrentWayPoint] - m_Position;
		l_Vector.Normalize()

		if self:PointInsideCircle(m_Position, self.m_WayPoints[m_CurrentWayPoint], 0.2) == false then
			self.m_RObject:SetPosition(m_Position + ((l_Vector*ElapsedTime*m_Speed)))
		else
			self.m_CurrentWayPoint = self.m_CurrentWayPoint+1
		end
		
		if self.m_CurrentWayPoint >= (#self.m_WayPoints) then
			self.m_CurrentWayPoint = 0
		end
	end
end

