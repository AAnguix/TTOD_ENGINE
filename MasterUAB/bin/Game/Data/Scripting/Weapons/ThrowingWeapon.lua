class 'CThrowingWeaponComponent' (CWeaponComponent)
function CThrowingWeaponComponent:__init(ComponentType, ParentLuaGameObject, ParentBoneName, PositionAdjustment, LuaGameObject, 
	Damage, WeaponType, OnPlayerContactParticlesName, ProjectileParticlesName)
	CWeaponComponent.__init(self,ComponentType, ParentLuaGameObject, ParentBoneName, LuaGameObject, Damage, WeaponType, "throwing")
	--self.m_LuaGameObject:SetParent(self.m_ParentLuaGameObject,ParentBoneName)
	self.m_PositionAdjustment = PositionAdjustment
	self.m_Parented = true
	if(OnPlayerContactParticlesName~="") then
		self.m_OnPlayerContactParticlesLuaGameObject = g_GameController:AddLuaGameObjectHandle(OnPlayerContactParticlesName)
		self.m_OnPlayerContactParticlesLuaGameObject:EnableRenderableObject(false)
	end
	self.m_OnPlayerContactParticlesEnabled = false
	self.m_OnPlayerContactParticlesDuration = 1.0
	self.m_OnPlayerContactParticlesTimer = 0.0
	self.m_OnPlayerContactPoint = Vect3f(0.0,0.0,0.0)
	if(ProjectileParticlesName~="") then
		self.m_ProjectileParticlesLuaGameObject = g_GameController:AddLuaGameObjectHandle(ProjectileParticlesName)
		self.m_ProjectileParticlesLuaGameObject:EnableRenderableObject(false)
	end
	self.m_ProjectileParticlesEnabled = false
	self.m_ProjectileParticlesDuration = 4.0
end

dofile("./Data/Scripting/Weapons/ThrowingWeaponParticles.lua")

function CThrowingWeaponComponent:Update(ElapsedTime)
	CWeaponComponent.Update(self,ElapsedTime)
	local l_Position = Vect3f(0.0,0.0,0.0)
	if self:IsParented() then
		local l_Matrix = Mat44f()
		l_Matrix = self:GetWeaponTransform()
		self:MoveActor(self.m_LuaGameObject:GetName(), l_Matrix)
	end
	if self.m_ProjectileParticlesLuaGameObject ~= nil then
		self:UpdateProjectileParticles(ElapsedTime)
	end
	if self.m_OnPlayerContactParticlesLuaGameObject ~= nil then
		self:UpdateOnPlayerContactParticles(ElapsedTime)
	end
end

function CThrowingWeaponComponent:PlayerAttacksEnemy(EnemyActor)
	local l_AttackingState = g_PlayerComponent:IsAttacking()
	if l_AttackingState then
		local l_Enemy = g_GameController:GetEnemy(EnemyActor)
		if l_Enemy ~=  nil then
			l_Enemy:TakeDamage(self)
			g_PlayerComponent:SetAttacking(false) 
		end
	end
end

function CThrowingWeaponComponent:OnTriggerEnter(Actor)
	
	CWeaponComponent.OnTriggerEnter(self,Actor)
	local l_ParentName = self.m_ParentLuaGameObject:GetName()
	
	if (l_ParentName=="Player") then
		if Actor ~= "Player"  then
			self:PlayerAttacksEnemy(Actor)
		end
	elseif (Actor=="Player") then
		if self.m_ProjectileParticlesEnabled then
			self.m_LuaGameObject:EnableRenderableObject(false)	
			self:EnableProjectileParticles(false)
			
			self:EnableOnPlayerContactParticles(true)
			self.m_OnPlayerContactPoint = self.m_LuaGameObject:GetPosition()
			self.m_OnPlayerContactParticlesLuaGameObject:SetPosition(self.m_OnPlayerContactPoint)
		end
		self:PlayerAttackedByRangeEnemy(l_ParentName)
	end	
end

function CThrowingWeaponComponent:PlayerAttackedByRangeEnemy(EnemyActor)
	local l_Parent = g_GameController:GetEnemy(EnemyActor)
	local l_AttackingState = l_Parent:IsAttacking()
	if l_AttackingState == false then
		if g_PlayerComponent:IsBlocking() then
			local l_Angle = CTTODMathUtils.AngleBetweenVectors(g_Player:GetForward(),l_Parent:GetLuaGameObject():GetForward())
			math.abs(l_Angle)
		
			if (l_Angle > g_PlayerComponent:GetMinBlockAngle() and l_Angle < g_PlayerComponent:GetMaxBlockAngle()) then
			else
				g_PlayerComponent:TakeDamage(self)
			end
		else
			g_PlayerComponent:TakeDamage(self)
		end
	end
end

function CThrowingWeaponComponent:OnTriggerExit(Actor)
	CWeaponComponent.OnTriggerExit(self,Actor)
end

function CThrowingWeaponComponent:GetLuaGameObject() return self.m_LuaGameObject end
function CThrowingWeaponComponent:SetParentState(state) self.m_Parented = state end
function CThrowingWeaponComponent:IsParented() return self.m_Parented end

function CThrowingWeaponComponent:MoveActor(Actor,Matrix)
	local l_Position = Vect3f(0.0,0.0,0.0)
	l_Position = Matrix:GetWorldPos()
	local l_Yaw = Matrix:GetYaw()
	local l_Pitch = Matrix:GetPitch()
	local l_Roll = Matrix:GetRoll()
	local l_Quat = Quatf(l_Yaw, l_Pitch, l_Roll)
	g_PhysXManager:MoveKinematicActor(self.m_LuaGameObject:GetName(), l_Position, l_Quat)
end

function CThrowingWeaponComponent:GetWeaponTransform()
	local l_BoneTransform = Mat44f()
	local l_ParentTransform = Mat44f()
	l_BoneTransform = self.m_ParentLuaGameObject:GetBoneTransformationMatrix(self.m_BoneID)
	l_ParentTransform = self.m_ParentLuaGameObject:GetTransform()
	
	local l_Transform = Mat44f()
	l_Transform = self:WeaponAdjustment()
	local l_Matrix = Mat44f()
	l_Matrix = l_Transform*l_BoneTransform*l_ParentTransform
	return l_Matrix
end

function CThrowingWeaponComponent:WeaponAjustment()
    local l_Translation = Mat44f()
    l_Translation:SetIdentity()
    l_Translation:Translate(self.m_PositionAdjustment)
	return l_Translation
end