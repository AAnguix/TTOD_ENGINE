class 'CEnemyComponent' (CLUAComponent)
function CEnemyComponent:__init(CRenderableObject, ComponentType)
	CLUAComponent.__init(self,ComponentType)
	self.m_RObject = CRenderableObject
	self.m_Dead=false
	self.m_Health=0.0
	
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
	self.m_Radius = 0.3
	self.m_Density = 30
	
	--Components
	self.m_Animator = nil 
	self.m_AudioSource = nil
end

function CEnemyComponent:Initialize()

	local l_CharacterCollider = CCharacterCollider.AddCharacterCollider("CharacterCollider", self.m_RObject)
	if l_CharacterCollider ~= nil then 
		local l_Material = l_CharacterCollider:GetPhysxMaterial()
		local l_MaterialName = l_Material:GetName()
		g_PhysXManager:RegisterMaterial(l_MaterialName, l_Material:GetStaticFriction(), l_Material:GetDynamicFriction(), l_Material:GetRestitution())
		m_Position = self.m_RObject:GetPosition()
		l_CControlerPos = Vect3f(m_Position.x, m_Position.y, m_Position.z)
		g_PhysXManager:CreateCharacterController(self.m_RObject:GetName(), self.m_Height, self.m_Radius , self.m_Density, l_CControlerPos, l_MaterialName)
	end
	
	self.m_Animator = CAnimatorController.AddAnimatorController("AnimatorController", self.m_RObject)
	
	g_LogManager:Log("Enemy "..self.m_RObject:GetName().." created...")
end

function CEnemyComponent:Update(ElapsedTime)
end

function CEnemyComponent:GetAnimator() return self.m_Animator end
function CEnemyComponent:GetAudioSource() return self.m_AudioSource end

function CEnemyComponent:IsDead() return self.m_Dead end
function CEnemyComponent:GetHealth() return self.m_Health end
function CEnemyComponent:GetSpeed()	return self.m_Speed end
function CEnemyComponent:GetVisionRange() return self.m_VisionRange end
function CEnemyComponent:GetAttackDelay() return self.m_AttackDelay end
function CEnemyComponent:GetArmor()	return self.m_Armor end
function CEnemyComponent:SetArmor(Armor) self.m_Armor = Armor end
function CEnemyComponent:GetWeapon() return self.m_Weapon end
function CEnemyComponent:SetWeapon(Weapon) self.m_Weapon = Weapon end
function CEnemyComponent:GetRenderableObject() return self.m_RObject end

function CEnemyComponent:AddWaypoint(WayPoint)
	table.insert(self.m_WayPoints,WayPoint)
end

function CEnemyComponent:TakeDamage(PlayerWeapon, PlayerDamage)
	self.m_Health = self.m_Health - Damage
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
	local l_Forward = self.m_RObject:GetForward()
	l_Forward.y = 0.0
	local l_Angle = CTTODMathUtils.GetAngleToFacePoint(l_Forward, self.m_RObject:GetPosition(), Point)	
	-- -k*T    si el ángulo es negativo. K es la velocidad
	--  si -k*T < angulo, aplicas el angulo, y no la formula, para que no se pase de rotación
	local l_CurrentYaw = self.m_RObject:GetYaw()
	local l_Velocity = self.m_RotationVelocity
	self.m_RObject:SetYaw(self:CalculateNewAngle(l_Angle, l_CurrentYaw, l_Velocity, ElapsedTime))
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
	--else g_LogManager:Log(self.m_RObject:GetName().." doesn't have waypoints")
	end
end


