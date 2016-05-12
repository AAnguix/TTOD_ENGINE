class 'CHealthPotion' (CItem)
function CHealthPotion:__init(Cooldown, HealthPoints)
	CItem.__init(self, Cooldown)
	self.m_HealthPoints = HealthPoints
end

function CHealthPotion:Use()
	if CItem.Use(self) then
		--g_Player:AddHealth
		return true
	else return false
	end
end