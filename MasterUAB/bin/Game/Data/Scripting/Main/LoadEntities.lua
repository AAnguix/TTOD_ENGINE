dofile("./Data/Scripting/Main/LoadCharacters.lua")

-- SHADOWMANAGER
function CGameController:LoadShadowManager(XMLTreeNode)
	local l_RObject = GetElementFromLayer(XMLTreeNode)
	local l_ShadowManager=CShadowManager()
	local l_ComponentName = "ShadowManager_Script"
	
	g_ScriptManager:AddComponent(l_ComponentName,l_RObject,l_ShadowManager)
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
	local l_GameObject = l_LuaGameObjectHandle:GetGameObject()
	local l_DestructibleWall=CDestructibleWall(l_LuaGameObjectHandle)
	local l_ComponentName = "DestructibleWall_Script"
	g_ScriptManager:AddComponent(l_ComponentName,l_GameObject,l_DestructibleWall)
	table.insert(self.m_Entities,l_DestructibleWall)
end

-- LIGHTMANAGER
function CGameController:LoadLightManager(XMLTreeNode, Pedestal)
	local l_GameObject = g_GameController:GetLuaGameObjectHandle(XMLTreeNode)
	local l_LightManager=CLightManager(Pedestal)
	local l_ComponentName = "LightManager_Script"
	
	g_ScriptManager:AddComponent(l_ComponentName,l_GameObject,l_LightManager)
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
function CGameController:LoadWeapon(TreeNode, EnemyComponent)
	local l_WeaponGameObject = GetLuaGameObjectHandle(TreeNode)
	if l_WeaponGameObject ~= nil then
		
		local l_Parent = EnemyComponent.m_RObject
		local l_BoneName = TreeNode:GetPszProperty("parent_bone", "", false)
		local l_WeaponType = TreeNode:GetPszProperty("type", "", false)
		local l_Damage = TreeNode:GetFloatProperty("damage", 0.0, false)
													--ComponentType, ParentRObject, MeshRObject, Damage, WeaponType)
		local l_WeaponComponent = CWeaponComponent("weapon",l_Parent, l_BoneName, l_WeaponGameObject, l_Damage, l_WeaponType)
		
		g_Weapon = l_WeaponComponent
		-- table.insert(self.m_Entities,l_WeaponComponent)
		local l_ComponentName = l_WeaponGameObject:GetName().."_Script"
		g_ScriptManager:AddComponent(l_ComponentName,l_WeaponGameObject,l_WeaponComponent)
		EnemyComponent:SetWeapon(l_WeaponComponent)
	
	end
end

-- ARMOR
function CGameController:LoadArmor(TreeNode, EnemyComponent)
	local l_ArmorGameObject = GetLuaGameObjectHandle(TreeNode)
	if l_ArmorGameObject ~= nil then
	
		local l_Parent = EnemyComponent.m_RObject
		local l_BoneName = TreeNode:GetPszProperty("parent_bone", "", false)
		local l_ArmorType = TreeNode:GetPszProperty("type", "", false)
		local l_Damage = TreeNode:GetFloatProperty("resistance", 0.0, false)
	
		local l_ArmorComponent  = CArmorComponent("armor",l_Parent, l_BoneName, l_ArmorGameObject, l_Damage, l_ArmorType)
		l_ArmorGameObject:AddLuaComponent(l_ArmorComponent)
		EnemyComponent:SetArmor(l_ArmorComponent)
		-- l_ArmorComponent:Initialize(l_BoneName)
		
		table.insert(self.m_Entities,l_ArmorComponent)
	end
end

-- PEDESTAL
function CGameController:LoadPedestal(XMLTreeNode)
	local l_GameObject = GetLuaGameObjectHandle(XMLTreeNode)
	local l_PedestalComponent=CPedestalComponent(l_GameObject)
	local l_ComponentName = l_GameObject:GetName().."_PedestalScript"
	g_ScriptManager:AddComponent(l_ComponentName,l_GameObject,l_PedestalComponent)
	table.insert(self.m_Entities,l_PedestalComponent)
	return l_PedestalComponent
end
