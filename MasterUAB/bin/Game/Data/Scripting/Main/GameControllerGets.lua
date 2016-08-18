function CGameController:GetEntities()	return self.m_Entities end
function CGameController:GetEnemies() return self.m_Enemies end
function CGameController:GetArmors() return self.m_Armors end
function CGameController:GetWeapons() return self.m_Weapons end

function CGameController:GetEnemy(EnemyName)
	for i=1, (#self.m_Enemies) do
		if self.m_Enemies[i].m_LuaGameObject:GetName() == EnemyName then
			return self.m_Enemies[i]
		end
	end
	return nil
end