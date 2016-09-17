class 'CHealthPotion' (CItem)
function CHealthPotion:__init(Cooldown, HealthPoints)
	CItem.__init(self, Cooldown)
	self.m_HealthPoints = HealthPoints
end

function CHealthPotion:Use()
	if (g_PlayerComponent:FullHealth()==false) and (CItem.Use(self)) then
		g_PlayerComponent:Heal(self.m_HealthPoints)
		--g_SoundManager:PlayEvent(SoundEvent("Play_DrinkPotion"))	
		return true
	else return false
	end
end