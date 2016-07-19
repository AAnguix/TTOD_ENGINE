class 'CWeaponComponent' (CLUAComponent)
function CWeaponComponent:__init(ComponentType, ParentLuaGameObject, ParentBoneName, LuaGameObject, Damage, WeaponType)
	CLUAComponent.__init(self,ComponentType)
	self.m_LuaGameObject = LuaGameObject
	self.m_ParentLuaGameObject = ParentLuaGameObject --The A.Model that is going to use this weapon
	self.m_Damage = Damage
	self.m_Type = WeaponType
	self.m_BoneID = self.m_ParentLuaGameObject:GetAnimatedCoreModel():GetBoneId(ParentBoneName)
	self.m_LuaGameObject:SetParent(self.m_ParentLuaGameObject,ParentBoneName)
	self.m_BasePosition = self.m_LuaGameObject:GetPosition()
	g_LogManager:Log(self.m_BasePosition)
	self.m_BaseYaw = self.m_LuaGameObject:GetYaw()
	g_LogManager:Log("BSeYaw "..self.m_BaseYaw)
	self.m_BasePitch = self.m_LuaGameObject:GetPitch()
	g_LogManager:Log("BPitch "..self.m_BasePitch)
	self.m_BaseRoll = self.m_LuaGameObject:GetRoll()
	g_LogManager:Log("BRoll "..self.m_BaseRoll)
	g_LogManager:Log("Weapon created")
end

-- function CWeaponComponent:Initialize(ParentBoneName)
	
-- end

function CWeaponComponent:Update(ElapsedTime)
	self:SetWeaponTransform()
	--g_LogManager:Log("Actualizando arma")
end

function CWeaponComponent:SetWeaponTransform()
	local l_BoneTransform = Mat44f()
	local l_ParentTransform = Mat44f()
	l_BoneTransform = self.m_ParentLuaGameObject:GetBoneTransformationMatrix(self.m_BoneID)
	l_ParentTransform = self.m_ParentLuaGameObject:GetTransform()
	
	local l_Transform = Mat44f()
	l_Transform = self:Get3DMaxTransform()
	local l_Matrix = Mat44f()
	l_Matrix = l_Transform*l_BoneTransform*l_ParentTransform
	local l_Position = Vect3f(0.0,0.0,0.0)
	
	l_Position = l_Matrix:GetWorldPos()
	local l_Yaw = l_Matrix:GetYaw()
	local l_Pitch = l_Matrix:GetPitch()
	local l_Roll = l_Matrix:GetRoll()
	--g_LogManager:Log(l_Position)	
	--g_LogManager:Log(l_Yaw)	
	--g_LogManager:Log(l_Pitch)	
	--g_LogManager:Log(l_Roll)	
	local l_Quat = Quatf(l_Yaw, l_Pitch, l_Roll)
	g_PhysXManager:MoveKinematicActor(self.m_LuaGameObject:GetName(), l_Position, l_Quat)
end

function CWeaponComponent:Get3DMaxTransform()

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
	--l_RotX:RotByAngleX(self.m_BasePitch)
	l_RotX:RotByAngleX(l_Pitch)
	l_RotY:SetIdentity()
	--l_RotY:RotByAngleY(self.m_BaseYaw)
	l_RotY:RotByAngleY(l_Yaw)
	l_RotZ:SetIdentity()
	--l_RotZ:RotByAngleZ(self.m_BaseRoll)
	l_RotZ:RotByAngleZ(l_Roll)
	
	local l_Tranform = Mat44f()
	l_Tranform = l_RotX*l_RotY*l_RotZ*l_Translation
	return l_Tranform

end

function CWeaponComponent:GetDamage()
	return self.m_Damage
end

function CWeaponComponent:GetType()
	return self.m_Type
end

function CWeaponComponent:GetMeshLuaGameObject()
	return self.m_LuaGameObject
end

function CWeaponComponent:GetParentLuaGameObject()
	return self.m_ParentLuaGameObject
end

