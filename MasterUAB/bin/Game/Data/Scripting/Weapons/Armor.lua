class 'CArmorComponent' (CLUAComponent)
function CArmorComponent:__init(ComponentType, ParentRObject, ParentBoneName, MeshRObject, Resistance, ArmorType)
	CLUAComponent.__init(self,ComponentType)
	self.m_MeshRObject = MeshRObject 
	self.m_ParentRObject = ParentRObject 
	self.m_Resistance = Resistance
	self.m_Type = ArmorType
	self.m_MeshRObject:SetParent(m_ParentRObject,ParentBoneName)
end

-- function CArmorComponent:Initialize(ParentBoneName)
	
-- end

function CArmorComponent:Update(ElapsedTime)
end

function CArmorComponent:GetResistance()
	return self.m_Resistance
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

