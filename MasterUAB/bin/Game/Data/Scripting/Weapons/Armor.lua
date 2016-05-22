class 'CArmorComponent' (CLUAComponent)
function CArmorComponent:__init(ComponentType, ParentRObject, MeshRObject, Resistance, ArmorType)
	CLUAComponent.__init(self,ComponentType)
	self.m_MeshRObject = MeshRObject 
	self.m_ParentRObject = ParentRObject 
	self.m_Resistance = Resistance
	self.m_Type = ArmorType
end

function CArmorComponent:Update(ElapsedTime)
end

function CArmorComponent:GetDamage()
	return self.m_Damage
end

function CArmorComponent:GetType()
	return self.m_Type
end

function CArmorComponent:GetMeshRObject()
	return self.m_MeshRObject
end

function CArmorComponent:GetParentRObject()
	return self.m_ParentRObject
end

