class 'PlayerLUA'

function PlayerLUA:__init(Health, Damage)
	self.m_Health=Health
	self.m_Damage=5.0
	self.m_Speed=1.0
end

function PlayerLUA:GetHealth()
	return self.m_Health
end

function PlayerLUA:GetDamage()
	return self.m_Damage
end

function PlayerLUA:GetSpeed()
	return self.m_Speed
end

function PlayerLUA:Damage(Damage)
	self.m_Health = self.m_Health - Damage
end

g_PlayerLUA=PlayerLUA(100.0)