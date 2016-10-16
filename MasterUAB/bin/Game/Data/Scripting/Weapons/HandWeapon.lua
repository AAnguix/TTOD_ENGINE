class 'CHandWeaponComponent' (CWeaponComponent)
function CHandWeaponComponent:__init(ComponentType, ParentLuaGameObject, ParentBoneName, LuaGameObject, Damage, WeaponType, ColliderAdjustment, ColliderRadius)
	CWeaponComponent.__init(self,ComponentType, ParentLuaGameObject, ParentBoneName, LuaGameObject, Damage, WeaponType, "hand")
	
	self.m_ColliderAdjust = ColliderAdjustment
	self.m_ColliderRadious = ColliderRadius
	
	g_SoundManager:AddComponent(self.m_LuaGameObject:GetName().."_AudioSource", self.m_LuaGameObject)
	self.m_LuaGameObject:AddSound("Hit","Play_DeganAxeImpact")
	self.m_LuaGameObject:AddSound("HitBlocked","Play_HitBlocked")
	--self.m_LuaGameObject:AddSound("PlayerBeaten","Play_BasicEnemySwordImpact")
	
	local l_Result = g_PhysXManager:CreateSphereTrigger(self.m_LuaGameObject:GetName(),ColliderRadius,"",Vect3f(0.0,100.0,0.0),Quatf(0.0,0.0,0.0,1.0),"kinematic")
	if l_Result then
		g_LogManager:Log("Created sphere trigger "..self.m_LuaGameObject:GetName())
	else g_LogManager:Log("Error. Can't create sphere trigger "..self.m_LuaGameObject:GetName())
	end
end

function CHandWeaponComponent:GetLuaGameObject() return self.m_LuaGameObject end
function CHandWeaponComponent:GetParentLuaGameObject() return self.m_ParentLuaGameObject end

function CHandWeaponComponent:Update(ElapsedTime)
	self:AddTime(ElapsedTime)
	self:SetWeaponTransform(ElapsedTime)
	CWeaponComponent.Update(self,ElapsedTime)
end

function CHandWeaponComponent:PlayerAttacksEnemy(EnemyActor)
	local l_AttackingState = g_PlayerComponent:IsAttacking()
	if l_AttackingState then
		local l_Enemy = g_GameController:GetEnemy(EnemyActor)
		if l_Enemy ~=  nil then
			self.m_LuaGameObject:PlayEvent("Hit")
			l_Enemy:TakeDamage(self)
			g_PlayerComponent:SetAttacking(false) 
		end
	end
end

function CHandWeaponComponent:OnTriggerEnter(Actor)
	CWeaponComponent.OnTriggerEnter(self,Actor)
	
	local l_ParentName = self.m_ParentLuaGameObject:GetName()
	
	if (l_ParentName=="Player") then
		if Actor ~= "Player"  then
			self:PlayerAttacksEnemy(Actor)
		end
	else --Parent is an enemy
		self:PlayerAttackedByBasicEnemy(l_ParentName)
	end	
end

function CHandWeaponComponent:PlayerAttackedByBasicEnemy(EnemyActor)
	local l_Parent = g_GameController:GetEnemy(EnemyActor)
	local l_AttackingState = l_Parent:IsAttacking()
	if l_AttackingState then
		if g_PlayerComponent:IsBlocking() then
			local l_Angle = CTTODMathUtils.AngleBetweenVectors(g_Player:GetForward(),l_Parent:GetLuaGameObject():GetForward())
			math.abs(l_Angle)
			if (l_Angle > g_PlayerComponent:GetMinBlockAngle() and l_Angle < g_PlayerComponent:GetMaxBlockAngle()) then
				self.m_LuaGameObject:PlayEvent("HitBlocked")
				l_Parent:SetAttacking(false)  
			else
				g_LogManager:Log("no esta bloqueando")
				self.m_LuaGameObject:PlayEvent("Hit")
				g_PlayerComponent:TakeDamage(self)
				l_Parent:SetAttacking(false)  
			end
		else
			self.m_LuaGameObject:PlayEvent("Hit")
			g_PlayerComponent:TakeDamage(self)
			l_Parent:SetAttacking(false)  --Only executed once/attack
		end
		
	else
		--g_LogManager:Log("Choca trigger y player pero no se estan atacando")
	end
end

function CHandWeaponComponent:SetWeaponTransform(ElapsedTime)
	local l_BoneTransform = Mat44f()
	local l_ParentTransform = Mat44f()
	l_BoneTransform = self.m_ParentLuaGameObject:GetBoneTransformationMatrix(self.m_BoneID)
	l_ParentTransform = self.m_ParentLuaGameObject:GetTransform()
	
	local l_Transform = Mat44f()
	--l_Transform = self:Get3DMaxTransform()
	l_Transform = self:WeaponAjustment()
	
	--l_Transform:SetIdentity()
	local l_Matrix = Mat44f()
	l_Matrix = l_Transform*l_BoneTransform*l_ParentTransform
	local l_Position = Vect3f(0.0,0.0,0.0)
	
	l_Position = l_Matrix:GetWorldPos()
	local l_Yaw = l_Matrix:GetYaw()
	local l_Pitch = l_Matrix:GetPitch()
	local l_Roll = l_Matrix:GetRoll()
	--local l_Quat = Quatf(l_Yaw, l_Pitch+(0.3), l_Roll)
	local l_Quat = Quatf(l_Yaw, l_Pitch, l_Roll)
	g_PhysXManager:MoveKinematicActor(self.m_LuaGameObject:GetName(), l_Position, l_Quat)
end

function CHandWeaponComponent:Get3DMaxTransform()
	
	local l_Yaw = self.m_LuaGameObject:GetYaw()
	local l_Pitch = self.m_LuaGameObject:GetPitch()
	local l_Roll = self.m_LuaGameObject:GetRoll()

	local l_RotX = Mat44f()
	local l_RotY = Mat44f()
	local l_RotZ = Mat44f()
	local l_Translation = Mat44f()
	l_Translation:SetIdentity()
	--l_Translation:Translate(self.m_BasePosition)
	l_Translation:Translate(self.m_LuaGameObject:GetPosition())
	
	l_RotX:SetIdentity()
	l_RotX:RotByAngleX(l_Pitch)
	
	l_RotY:SetIdentity()
	l_RotY:RotByAngleY(l_Yaw)
	
	l_RotZ:SetIdentity()
	l_RotZ:RotByAngleZ(l_Roll)
	
	local l_Tranform = Mat44f()
	l_Tranform = l_RotX*l_RotY*l_RotZ*l_Translation
	return l_Tranform

end

function CHandWeaponComponent:WeaponAjustment()
	local l_Translation = Mat44f()
	l_Translation:SetIdentity()
	l_Translation:Translate(self.m_ColliderAdjust)
	return l_Translation
end