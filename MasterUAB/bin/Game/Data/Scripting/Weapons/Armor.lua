class 'CArmorComponent' (CLUAComponent)
function CArmorComponent:__init(ComponentType, ParentLuaGameObject, ParentBoneName, LuaGameObject, Resistance, ArmorType)
	CLUAComponent.__init(self,ComponentType)
	self.m_LuaGameObject = LuaGameObject
	self.m_ParentLuaGameObject = ParentLuaGameObject 
	self.m_Resistance = Resistance
	self.m_Type = ArmorType
	self.m_LuaGameObject:SetParent(m_ParentLuaGameObject,ParentBoneName)
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

function CArmorComponent:GetLuaGameObject()
	return self.m_LuaGameObject
end

function CArmorComponent:GetParentLuaGameObject()
	return self.m_ParentLuaGameObject
end

