dofile("./Data/Scripting/Main/LoadCharacters.lua")

-- SHADOWMANAGER
function CGameController:LoadShadowManager(XMLTreeNode)
	local l_LuaGameObject = g_GameController:AddLuaGameObjectHandle(XMLTreeNode)
	local l_ShadowManager = CShadowManager()
	local l_ComponentName = "ShadowManager_Script"
	
	g_ScriptManager:AddComponent(l_ComponentName,l_LuaGameObject,l_ShadowManager)
	table.insert(self.m_Entities,l_ShadowManager)
	
	for i=0, XMLTreeNode:GetNumChildren() do
		local l_Element=XMLTreeNode:GetChild(i)
		if l_Element:GetName()=="spot_light" then
			local l_SpotLightName = l_Element:GetPszProperty("name","",false)
			local l_Spotlight = g_LightManager:GetResource(l_SpotLightName)
			if l_Spotlight == nil then
				g_LogManager:Log("Unable to find "..l_SpotLightName.." to add it to ShadowManager")
			else
				l_ShadowManager:AddSpotlight(l_Spotlight)
			end
		end
	end
end

-- DESTRUCTIBLEWALL
function CGameController:LoadDestructibleWall(XMLTreeNode)
	local l_LuaGameObjectHandle = g_GameController:AddLuaGameObjectHandle(XMLTreeNode)
	local l_DestructibleWall=CDestructibleWall(l_LuaGameObjectHandle)
	local l_ComponentName = "DestructibleWall_Script"
	g_ScriptManager:AddComponent(l_ComponentName,l_LuaGameObjectHandle,l_DestructibleWall)
	table.insert(self.m_Entities,l_DestructibleWall)
end

-- LIGHTMANAGER
function CGameController:LoadLightManager(XMLTreeNode, Pedestal)
	local l_LuaGameObject = g_GameController:AddLuaGameObjectHandle(XMLTreeNode)
	local l_LightManager = CLightManager(Pedestal)
	local l_ComponentName = "LightManager_Script"
	
	g_ScriptManager:AddComponent(l_ComponentName,l_LuaGameObject,l_LightManager)
	table.insert(self.m_Entities,l_LightManager)
	
	for i=0, XMLTreeNode:GetNumChildren() do
		local l_Element=XMLTreeNode:GetChild(i)
		if l_Element:GetName()=="light" then
			local l_LightName = l_Element:GetPszProperty("name","",false)
			local l_Light = g_LightManager:GetResource(l_LightName)
			if l_Light == nil then
				g_LogManager:Log("Unable to find "..l_LightName.." to add it to LightManager")
			else
				l_LightManager:AddLight(l_Light)
			end
		end
	end
	-- l_LightManager:DisableLights()
end

-- WEAPON
function CGameController:LoadWeapon(TreeNode, EntityComponent)

	local l_Name = TreeNode:GetPszProperty("game_object", "", false)
	local l_WeaponLuaGameObject = g_GameController:AddLuaGameObjectHandle(l_Name)
	if l_WeaponLuaGameObject ~= nil then
		local l_Parent = EntityComponent.m_LuaGameObject
		local l_ParentName = EntityComponent.m_LuaGameObject:GetName()
		local l_BoneName = TreeNode:GetPszProperty("parent_bone", "", false)
		local l_WeaponName = TreeNode:GetPszProperty("game_object","",false)
		local l_WeaponType = TreeNode:GetPszProperty("type", "", false)
		local l_Damage = TreeNode:GetFloatProperty("damage", 0.0, false)								--ComponentType, ParentRObject, MeshRObject, Damage, WeaponType)
		local l_WeaponComponent = CWeaponComponent(l_WeaponName,l_Parent, l_BoneName, l_WeaponLuaGameObject, l_Damage, l_WeaponType)
		table.insert(g_GameController:GetEntities(),l_WeaponComponent)
		local l_ComponentName = l_WeaponLuaGameObject:GetName().."_Script"
		g_ScriptManager:AddComponent(l_ComponentName,l_WeaponLuaGameObject,l_WeaponComponent)
		
		if l_ParentName == 'Player' then
			EntityComponent:AddWeapon(l_WeaponComponent)
		else
			EntityComponent:SetWeapon(l_WeaponComponent)
		end
		--return l_WeaponComponent
	end
end

-- ARMOR
function CGameController:LoadArmor(TreeNode, EntityComponent)
	local l_Name = TreeNode:GetPszProperty("game_object", "", false)
	local l_ArmorLuaGameObject = g_GameController:AddLuaGameObjectHandle(l_Name)
	if l_ArmorLuaGameObject ~= nil then
	
		local l_Parent = EntityComponent.m_LuaGameObject
		local l_ParentName = EnemyComponent.m_LuaGameObject:GetName()
		
		local l_BoneName = TreeNode:GetPszProperty("parent_bone", "", false)
		local l_ArmorName = TreeNode:GetPszProperty("game_object","Armor",false)
		local l_ArmorType = TreeNode:GetPszProperty("type", "", false)
		local l_Damage = TreeNode:GetFloatProperty("resistance", 0.0, false)
	
		local l_ArmorComponent = CArmorComponent(l_ArmorName,l_Parent, l_BoneName, l_ArmorLuaGameObject, l_Damage, l_ArmorType)
		table.insert(g_GameController:GetEntities(),l_ArmorComponent)
		local l_ComponentName = l_ArmorLuaGameObject:GetName().."_Script"
		g_ScriptManager:AddComponent(l_ComponentName,l_ArmorLuaGameObject,l_ArmorComponent)
		
		if l_ParentName == 'Player' then
			EnemyComponent:AddArmor(l_ArmorComponent)
		else
			EnemyComponent:SetArmor(l_ArmorComponent)
		end
		
		table.insert(self.m_Entities,l_ArmorComponent)
	end
end

-- PEDESTAL
function CGameController:LoadPedestal(XMLTreeNode)
	local l_LuaGameObject = g_GameController:AddLuaGameObjectHandle(XMLTreeNode)
	local l_PedestalComponent=CPedestalComponent(l_LuaGameObject)
	local l_ComponentName = l_LuaGameObject.m_LuaGameObject:GetName().."_PedestalScript"
	g_ScriptManager:AddComponent(l_ComponentName,l_LuaGameObject,l_PedestalComponent)
	table.insert(self.m_Entities,l_PedestalComponent)
	return l_PedestalComponent
end
