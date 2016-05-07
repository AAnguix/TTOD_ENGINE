class 'CWeapon'

function CWeapon:__init(Damage,Type)
	self.m_Damage = Damage
	self.m_Type = Type
end

function CWeapon:GetDamage()
	return self.m_Damage
end