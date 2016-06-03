class 'CWeaponComponent' (CLUAComponent)
function CWeaponComponent:__init(ComponentType, ParentRObject, ParentBoneName, MeshRObject, Damage, WeaponType)
	CLUAComponent.__init(self,ComponentType)
	self.m_MeshRObject = MeshRObject --The mesh instance that points to this class
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
	-- g_RenderManager:GetContextManager:SetWorldMatrix(self:GetWeaponTransform())
	g_LogManager:Log("Actualizando arma")
end

function CWeaponComponent:GetWeaponTransform()
	-- local l_TransformMatrix = Mat44f()
	-- local l_ParentTransform = self:GetParentRObject():GetTransform()
	-- local l_BoneTransform = self:GetParentRObject():GetBoneTransformationMatrix(self.m_BoneID)
	-- local l_WeaponLocalTransform = self.m_MeshRObject:GetTransform()
	
	-- l_TransformMatrix = l_WeaponLocalTransform * l_BoneTransform * l_ParentTransform
	
	-- return l_TransformMatrix
end

function CWeaponComponent:DrawAxis()
	-- g_ContextManager:SetWorldMatrix(self:GetWeaponTransform());
	-- g_ContextManager:Draw(GetAxis());
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

