
function CGameController:Destroy()
	-- for i=1, (#self.m_Entities) do
		-- if self.m_Entities[i] ~= nil then
			-- local l_Name = ""
			-- local l_LuaGO = self.m_Entities[i]:GetLuaGameObject()
			-- if (l_LuaGO~=nil) then
				-- l_Name = l_LuaGO:GetName()
				-- if  (l_Name ~= "Player_PlayerScript") then
					-- table.remove(self.m_Entities,i)
				-- end
			-- else 
				-- table.remove(self.m_Entities,i)
			-- end
			-- g_LogManager:Log("Entity "..l_Name.." removed")
		-- end	
	-- end
	-- for i=1, (#self.m_Enemies) do
		-- table.remove(self.m_Enemies,i)
	-- end
	
	self.m_Entities = {}
	self.m_Enemies = {}
	self.m_Armors = {}
	self.m_Weapons = {}
	
	-- self:CleanLuaTable(self.m_Armors)
	-- self:CleanLuaTable(self.m_Weapons)
	g_LogManager:Log("Entities "..#self.m_Entities)
	g_LogManager:Log("Lua GameController destroyed")
end

function CGameController:CleanLuaTable(Table)
	for i=1, (#Table) do
		table.remove(Table,i)
	end
end

function CGameController:RemoveEntity(Name)
	g_LogManager:Log("Removing entity "..Name)
	for i=1, (#self.m_Entities) do
		local l_LuaGO = self.m_Entities[i]:GetLuaGameObject()
		if  (l_LuaGO~=nil) and (l_LuaGO:GetName()) == Name then
			g_LuaGameObjectHandleManager:Remove(Name)
			table.remove(self.m_Entities,i)
		end
	end
end

function CGameController:RemoveEnemy(Name)
	local l_EnemyIndex = 0
	local l_EntityIndex = 0
	
	for i=1, (#self.m_Enemies) do
		local l_LuaGO = self.m_Enemies[i]:GetLuaGameObject()
		if  (l_LuaGO~=nil) and (l_LuaGO:GetName()) == Name then
			l_EnemyIndex = i
		end
	end
	
	for i=1, (#self.m_Entities) do
		local l_LuaGO = self.m_Entities[i]:GetLuaGameObject()
		if  (l_LuaGO~=nil) and (l_LuaGO:GetName()) == Name then
			l_EntityIndex = i
		end
	end
	
	if l_EnemyIndex ~= 0 and l_EntityIndex ~= 0 then
	
		local l_Armor = self.m_Enemies[l_EnemyIndex]:GetArmor()
		local l_Weapon = self.m_Enemies[l_EnemyIndex]:GetWeapon()
		if l_Armor ~= nil then
			self:RemoveElementFromTable(self.m_Armors,l_Armor:GetName())
		end
		if l_Weapon ~= nil then
			self:RemoveElementFromTable(self.m_Weapons,l_Weapon:GetName())
		end
	
		table.remove(self.m_Enemies,l_EnemyIndex)
		table.remove(self.m_Entities,l_EntityIndex)
		
		g_LuaGameObjectHandleManager:Remove(Name)
	else
		g_LogManager:Log("ERROR. Unable to delete enemy "..Name)
	end
end

function CGameController:RemoveElementFromTable(Table, ElementName)
	for i=1, (#Table) do
		local l_LuaGO = Table[i]:GetLuaGameObject()
		if  (l_LuaGO~=nil) and (l_LuaGO:GetName()) == ElementName then
			Table[i] = nil
			g_LuaGameObjectHandleManager:Remove(ElementName)
		end
	end
end
