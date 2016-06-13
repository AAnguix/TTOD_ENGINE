class 'CWeaponComponent' (CLUAComponent)
function CWeaponComponent:__init(ComponentType, ParentRObject, ParentBoneName, GameObject, Damage, WeaponType)
	CLUAComponent.__init(self,ComponentType)
	self.m_GameObject = GameObject
	self.m_MeshRObject = GameObject:GetRenderableObject() 
	self.m_ParentRObject = ParentRObject --The A.Model that is going to use this weapon
	self.m_Damage = Damage
	self.m_Type = WeaponType
	self.m_BoneID = self.m_ParentRObject:GetAnimatedCoreModel():GetBoneId(ParentBoneName)
	self.m_MeshRObject:SetParent(self.m_ParentRObject,ParentBoneName)
	g_LogManager:Log("Weapon created")
end

-- function CWeaponComponent:Initialize(ParentBoneName)
	
-- end

function CWeaponComponent:Update(ElapsedTime)
	-- self:SetWeaponTransform()
	-- g_LogManager:Log("Actualizando arma")
end

function CWeaponComponent:SetWeaponTransform()
	-- local l_BoneTransform = self.m_ParentRObject:GetBoneTransformationMatrix(self.m_BoneID)
	-- local l_ParentTransform = self.m_ParentRObject:GetTransform()
		
	-- g_ContextManager:SetWorldMatrix(l_BoneTransform*l_ParentTransform)
	-- -- g_ContextManager:Draw(g_RenderManager:GetDebugRender():GetAxis())
	
	-- --ChildGetTransform	 --self.m_MeshRObject:GetTransform()*l_BoneTransform*l_ParentTransform
	-- local l_Transform = self.m_MeshRObject:GetTransform()
	-- --local l_Transform = Get3DMaxTransform()
	-- g_ContextManager:SetWorldMatrix(l_Transform*l_BoneTransform*l_ParentTransform)
	-- self.m_MeshRObject:GetStaticMesh():Render(g_RenderManager)
end

function CWeaponComponent:Get3DMaxTransform()

	local l_Yaw = self.m_MeshRObject:GetYaw()
	local l_Pitch = self.m_MeshRObject:GetPitch()
	local l_Roll = self.m_MeshRObject:GetRoll()

	local l_RotX = Mat44f()
	local l_RotY = Mat44f()
	local l_RotZ = Mat44f()
	local l_Translation = Mat44f()
	l_Translation:SetIdentity()
	l_Translation:Translate(self.m_MeshRObject:GetPosition())
	l_RotX:SetIdentity()
	l_RotX:RotByAngleX(l_Pitch+1.57)
	l_RotY:SetIdentity()
	l_RotY:RotByAngleY(l_Yaw)
	l_RotZ:SetIdentity()
	l_RotZ:RotByAngleZ(l_Roll)

	local l_Tranform = l_RotY*l_RotZ*l_RotX*l_Translation
	return l_Tranform

end

function CWeaponComponent:GetDamage()
	return self.m_Damage
end

function CWeaponComponent:GetType()
	return self.m_Type
end

function CWeaponComponent:GetMeshRObject()
	return self.m_MeshRObject
end

function CWeaponComponent:GetParentRObject()
	return self.m_ParentRObject
end

