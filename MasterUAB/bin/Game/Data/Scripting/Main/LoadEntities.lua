-- SHADOWMANAGER
function CGameController:LoadShadowManager(XMLTreeNode)
	local l_Name = XMLTreeNode:GetPszProperty("game_object", "", false)
	local l_LuaGameObject = g_GameController:AddLuaGameObjectHandle(l_Name)
	local l_ShadowManager = CShadowManager()
	
	g_ScriptManager:AddComponent(l_Name,l_LuaGameObject,l_ShadowManager)
	table.insert(self.m_Entities,l_ShadowManager)
	
	for i=0, XMLTreeNode:GetNumChildren()-1 do
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
	local l_Name = XMLTreeNode:GetPszProperty("game_object", "", false)
	local l_LuaGameObjectHandle = g_GameController:AddLuaGameObjectHandle(l_Name)
	local l_DestructibleWall=CDestructibleWall(l_LuaGameObjectHandle)
	g_ScriptManager:AddComponent(l_Name,l_LuaGameObjectHandle,l_DestructibleWall)
	table.insert(self.m_Entities,l_DestructibleWall)
end

-- LIGHTMANAGER
function CGameController:LoadLightManager(XMLTreeNode, Pedestal)
	local l_Name = XMLTreeNode:GetPszProperty("game_object", "", false)
	local l_LuaGameObject = g_GameController:AddLuaGameObjectHandle(l_Name)
	local l_LightManager = CLightManager(Pedestal)
	
	g_ScriptManager:AddComponent(l_Name,l_LuaGameObject,l_LightManager)
	table.insert(self.m_Entities,l_LightManager)
	
	for i=0, XMLTreeNode:GetNumChildren()-1 do
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
		local l_WeaponType = TreeNode:GetPszProperty("type", "", false)
		
		--g_DamageCalculator:LogArmors()
		--g_DamageCalculator:LogWeapons()
		
		if g_DamageCalculator:WeaponExists(l_WeaponType) then
			
			local l_Parent = EntityComponent.m_LuaGameObject
			local l_ParentName = EntityComponent.m_LuaGameObject:GetName()
			local l_BoneName = TreeNode:GetPszProperty("parent_bone", "", false)
			local l_WeaponName = TreeNode:GetPszProperty("game_object","",false)
			local l_Damage = TreeNode:GetFloatProperty("damage", 0.0, false)
			local l_WeaponStyle = TreeNode:GetPszProperty("weapon_style","",false)
		
			local l_PositionAdjustment = TreeNode:GetVect3fProperty("position_adjustment", Vect3f(0.0,0.0,0.0), false)
			local l_ColliderRadius = TreeNode:GetFloatProperty("collider_radius", 0.0, false)
			
			local l_OnPlayerContactParticles = TreeNode:GetPszProperty("on_player_contact_particles","",false)
			local l_ProjectileParticles = TreeNode:GetPszProperty("projectile_particles","",false)
			
			local l_WeaponComponent = nil
			if l_WeaponStyle == 'throwing' then
				g_LogManager:Log("Creamos una arma throwing")
				l_WeaponComponent = CThrowingWeaponComponent(l_WeaponName,l_Parent, l_BoneName, l_PositionAdjustment, l_WeaponLuaGameObject, l_Damage, l_WeaponType, l_OnPlayerContactParticles, l_ProjectileParticles)	
			elseif l_WeaponStyle == 'hand' then
				l_WeaponComponent = CHandWeaponComponent(l_WeaponName,l_Parent, l_BoneName, l_WeaponLuaGameObject, l_Damage, l_WeaponType,l_PositionAdjustment,l_ColliderRadius)
			else
				g_LogManager:Log("Error. Can't load weapon "..l_Name..". WeaponType '"..l_WeaponStyle.."' isn't valid")
			end
			
			table.insert(g_GameController:GetWeapons(),l_WeaponComponent)
		
			g_ScriptManager:AddComponent(l_Name,l_WeaponLuaGameObject,l_WeaponComponent)
			if l_ParentName == 'Player' and l_WeaponComponent ~= nil then
				EntityComponent:AddWeapon(l_WeaponComponent)
				g_LogManager:Log("Arma: "..l_WeaponName..". Agregada al "..l_ParentName)
			elseif l_WeaponComponent ~= nil then
				EntityComponent:SetWeapon(l_WeaponComponent)
				g_LogManager:Log("Arma: "..l_WeaponName..". Agregada al "..l_ParentName)
			else
				g_LogManager:Log("Arma: "..l_WeaponName..". Error al agregar el Componente")
			end
		else
			g_LogManager:Log("Error. Can't load weapon "..l_Name..". WeaponType '"..l_WeaponType.."' isn't created")
		end
	end
end

-- ARMOR
function CGameController:LoadArmor(TreeNode, EntityComponent)
	local l_Name = TreeNode:GetPszProperty("game_object", "", false)
	local l_ArmorLuaGameObject = g_GameController:AddLuaGameObjectHandle(l_Name)
	if l_ArmorLuaGameObject ~= nil then
		local l_ArmorType = TreeNode:GetPszProperty("type", "", false)
		if g_DamageCalculator:ArmorExists(l_ArmorType) then
			local l_Parent = EntityComponent.m_LuaGameObject
			local l_ParentName = EnemyComponent.m_LuaGameObject:GetName()
			local l_BoneName = TreeNode:GetPszProperty("parent_bone", "", false)
			local l_ArmorName = TreeNode:GetPszProperty("game_object","Armor",false)
			local l_Damage = TreeNode:GetFloatProperty("resistance", 0.0, false)
			local l_ArmorComponent = CArmorComponent(l_ArmorName,l_Parent, l_BoneName, l_ArmorLuaGameObject, l_Damage, l_ArmorType)
			table.insert(g_GameController:GetArmors(),l_ArmorComponent)
			g_ScriptManager:AddComponent(l_Name,l_ArmorLuaGameObject,l_ArmorComponent)
			
			if l_ParentName == 'Player' then
				EnemyComponent:AddArmor(l_ArmorComponent)
			else
				EnemyComponent:SetArmor(l_ArmorComponent)
			end
		else
			g_LogManager:Log("Error. Can't load armor "..l_Name..". ArmorType '"..l_ArmorType.."' isn't created")
		end
	end
end

-- PEDESTAL
function CGameController:LoadPedestal(XMLTreeNode)
	local l_Name = XMLTreeNode:GetPszProperty("game_object", "", false)
	local l_LuaGameObject = g_GameController:AddLuaGameObjectHandle(l_Name)
	local l_PedestalComponent=CPedestalComponent(l_LuaGameObject)
	g_ScriptManager:AddComponent(l_Name,l_LuaGameObject,l_PedestalComponent)
	table.insert(self.m_Entities,l_PedestalComponent)
	return l_PedestalComponent
end

-- MAGIC_BALL
function CGameController:LoadMagicBall(XMLTreeNode)
	if g_MagicBall==nil then 
		local l_Name = XMLTreeNode:GetPszProperty("game_object", "", false)
		g_MagicBall = g_GameController:AddLuaGameObjectHandle(l_Name)
		--g_LogManager:Log("Magic ball created")
		-- local l_MagicBallComponent=CMagicBallComponent(g_MagicBall)
		-- local l_ComponentName = g_MagicBall.m_LuaGameObject:GetName()
		-- g_ScriptManager:AddComponent(l_ComponentName,g_MagicBall,l_MagicBallComponent)
		-- table.insert(self.m_Entities,l_MagicBallComponent)
	else
		g_LogManager:Log("Can't create magic ball, there is one created already")
	end
end

-- STATUE
function CGameController:LoadStatue(XMLTreeNode)
	if g_MagicBall ~= nil then
		local l_Name = XMLTreeNode:GetPszProperty("game_object", "", false)
		local l_LuaGameObject = g_GameController:AddLuaGameObjectHandle(l_Name)
		local l_Statue=CStatueComponent(l_LuaGameObject, g_MagicBall)
		g_ScriptManager:AddComponent(l_Name,l_LuaGameObject,l_Statue)
		table.insert(self.m_Entities,l_Statue)
		g_LogManager:Log("Statue created")
	else 
		g_LogManager:Log("Can't create statue because magic ball is null")
	end
end
