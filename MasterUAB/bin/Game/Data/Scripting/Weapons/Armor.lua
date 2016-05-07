class 'CArmor'

function CArmor:__init(Resistance,Type)
	self.m_Resistance = Resistance
	self.m_Type = Type
end

function CArmor:GetResistance()
	return self.m_Resistance
end