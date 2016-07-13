class 'CHealthPotion' (CItem)
function CHealthPotion:__init(Cooldown, HealthPoints)
	CItem.__init(self, Cooldown)
	self.m_HealthPoints = HealthPoints
end

function CHealthPotion:Use()
	if (g_PlayerComponent:FullHealth()==false) and (CItem.Use(self)) then
		g_PlayerComponent:Health(self.m_HealthPoints)
		return true
	else return false
	end
end