
function CGameController:Destroy()
	
	self.m_Entities = {}
	self.m_Enemies = {}
	self.m_Armors = {}
	self.m_Weapons = {}
	
	-- self:CleanLuaTable(self.m_Armors)
	-- self:CleanLuaTable(self.m_Weapons)
	g_LogManager:Log("Entities: "..#self.m_Entities)
	g_LogManager:Log("Enemies: "..#self.m_Enemies)
	g_LogManager:Log("Armors: "..#self.m_Armors)
	g_LogManager:Log("Weapons: "..#self.m_Weapons)
	g_LogManager:Log("GameController destroyed")
end

function CGameController:RemoveEnemyArmorAndWeapon(EnemyName)
	
	local l_EnemyIndex = self:GetElementIndex(self.m_Enemies, EnemyName)
	
	if l_EnemyIndex ~= -1 then
		local l_Armor = self.m_Enemies[l_EnemyIndex]:GetArmor()
		local l_Weapon = self.m_Enemies[l_EnemyIndex]:GetWeapon()
		if l_Armor ~= nil then
			self:RemoveElementFromTable(self.m_Armors,l_Armor:GetName())
		else 
			g_LogManager:Log("Enemy "..EnemyName.." doesn't have a armor to be removed")
		end
		if l_Weapon ~= nil then
			if l_Weapon:GetStyle()=="hand" then
				g_PhysXManager:DeleteTrigger(l_Weapon:GetName())
			end
			self:RemoveElementFromTable(self.m_Weapons,l_Weapon:GetName())
		else
			g_LogManager:Log("Enemy "..EnemyName.." doesn't have a weapon to be removed")
		end
	end
end


function CGameController:GetElementIndex(Table, ElementName)
	for i=1, (#Table) do
		local l_LuaGO = Table[i]:GetLuaGameObject()
		if  (l_LuaGO~=nil) and (l_LuaGO:GetName()) == ElementName then
			return i
		end
	end
	return -1
end

function CGameController:RemoveElementFromTable(Table, ElementName)
	local l_ElementIndex = self:GetElementIndex(Table, ElementName)
	if  l_ElementIndex ~= -1 then
		g_LuaGameObjectHandleManager:Remove(ElementName)
		table.remove(Table,l_ElementIndex)
	end
end
