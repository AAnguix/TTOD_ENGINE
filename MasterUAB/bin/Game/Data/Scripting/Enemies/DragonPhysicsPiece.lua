class 'CDragonPhysicsPiece' (CLUAComponent)
function CDragonPhysicsPiece:__init(ParentLuaGameObject, BoneName, BoneType, BoneSize)
	g_LogManager:Log("Creando Dragon Piece")
	self.m_ParentLuaGameObject = ParentLuaGameObject --The A.Model that is going to use this weapon
	self.m_BoneName = BoneName
	self.m_BoneID = self.m_ParentLuaGameObject:GetAnimatedCoreModel():GetBoneId(BoneName)
	self.m_BoneSize = BoneSize
	self.m_BoneType = BoneType
	--self.m_BasePosition = self.m_LuaGameObject:GetPosition()
	
	--self.m_BaseYaw = self.m_LuaGameObject:GetYaw()
	
	--self.m_BasePitch = self.m_LuaGameObject:GetPitch()

	--self.m_BaseRoll = self.m_LuaGameObject:GetRoll()
end

function CDragonPhysicsPiece:Update(ElapsedTime)
	self:SetTransform()
	--g_LogManager:Log("Actualizando arma")
	
	
end

function CDragonPhysicsPiece:GetWorldPos()
	local l_BoneTransform = Mat44f()
	local l_ParentTransform = Mat44f()
	l_BoneTransform = self.m_ParentLuaGameObject:GetBoneTransformationMatrix(self.m_BoneID)
	l_ParentTransform = self.m_ParentLuaGameObject:GetTransform()
	local l_Transform = Mat44f()
	l_Transform:SetIdentity()
	local l_Matrix = Mat44f()
	l_Matrix = l_Transform*l_BoneTransform*l_ParentTransform
	local l_Position = Vect3f(0.0,0.0,0.0)
	l_Position = l_Matrix:GetWorldPos()
	return l_Position
end

function CDragonPhysicsPiece:GetRotation()
	local l_BoneTransform = Mat44f()
	local l_ParentTransform = Mat44f()
	l_BoneTransform = self.m_ParentLuaGameObject:GetBoneTransformationMatrix(self.m_BoneID)
	l_ParentTransform = self.m_ParentLuaGameObject:GetTransform()
	local l_Transform = Mat44f()
	l_Transform:SetIdentity()
	local l_Matrix = Mat44f()
	l_Matrix = l_Transform*l_BoneTransform*l_ParentTransform
	local l_Position = Vect3f(0.0,0.0,0.0)
	l_Position = l_Matrix:GetWorldPos()
	local l_Yaw = l_Matrix:GetYaw()
	local l_Pitch = l_Matrix:GetPitch() + 1.57
	local l_Roll = l_Matrix:GetRoll()
	local l_Quat = Quatf(l_Yaw, l_Pitch, l_Roll)
	return l_Quat
end
	
function CDragonPhysicsPiece:SetTransform()
	local l_BoneTransform = Mat44f()
	local l_ParentTransform = Mat44f()
	l_BoneTransform = self.m_ParentLuaGameObject:GetBoneTransformationMatrix(self.m_BoneID)
	l_ParentTransform = self.m_ParentLuaGameObject:GetTransform()
	
	local l_Transform = Mat44f()
	--l_Transform = self:Get3DMaxTransform()
	l_Transform:SetIdentity()
	local l_Matrix = Mat44f()
	l_Matrix = l_Transform*l_BoneTransform*l_ParentTransform
	--l_Matrix = l_Transform*l_ParentTransform*l_BoneTransform
	
	--l_Matrix = l_ParentTransform*l_BoneTransform*l_Transform
	--l_Matrix = l_ParentTransform*l_Transform*l_BoneTransform
	--l_Matrix = l_BoneTransform*l_ParentTransform*l_Transform   --Parece q pone bien pero en lugares diferentes
	--l_Matrix = l_BoneTransform*l_Transform*l_ParentTransform
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
	g_PhysXManager:MoveKinematicActor(self.m_BoneName, l_Position, l_Quat)
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

function CWeaponComponent:GetParentLuaGameObject()
	return self.m_ParentLuaGameObject
end


function CDragonPhysicsPiece:OnTriggerEnter(Actor)
	if Subject == "Player" then
		
		g_Player:SetTrigger("TossedByDragon")
		local l_bonePos = self:GetWorldPos()
		local l_PlayerPos = g_Player:GetPosition()
		local l_Direction = (l_PlayerPos - l_bonePos) * 3.0 
		l_Direction:Normalize(1)
		g_PlayerComponent:SetTossedDirection(l_Direction)
	end	
end

function CDragonPhysicsPiece:OnTriggerExit(Actor)
	
end