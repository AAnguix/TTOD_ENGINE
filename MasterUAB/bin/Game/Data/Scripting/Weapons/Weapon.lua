class 'CWeaponComponent' (CLUAComponent)
function CWeaponComponent:__init(ComponentType, ParentRObject, MeshRObject, Damage, WeaponType)
	CLUAComponent.__init(self,ComponentType)
	self.m_MeshRObject = MeshRObject --The mesh instance that points to this class
	self.m_ParentRObject = ParentRObject --The A.Model that is going to use this weapon
	self.m_Damage = Damage
	self.m_Type = WeaponType
end

function CWeaponComponent:Initialize(ParentBoneName)
	self.m_MeshRObject:SetParent(m_ParentRObject,ParentBoneName)
end

function CWeaponComponent:Update(ElapsedTime)
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

