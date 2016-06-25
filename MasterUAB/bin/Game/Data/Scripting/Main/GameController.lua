dofile("./Data/Scripting/Weapons/DamageCalculator.lua")

class 'CGameController'

g_Weapon = nil

function CGameController:__init()
	self.m_Entities={}
	self.m_Enemies={}
	self.m_Filename = ""
end

function CGameController:Destroy()
	for i=1, (#self.m_Entities) do
		g_LogManager:Log(self.m_Entities[i]:GetName().. " destroyed")
		if self.m_Entities[i]:GetName() ~= "Player_PlayerScript" then
			self.m_Entities[i] = nil
		end
	end
	for i=1, (#self.m_Enemies) do
		self.m_Enemies[i] = nil
	end
end

function CGameController:GetScripts()
	return self.m_Entities
end

function CGameController:GetEnemies()
	return self.m_Enemies
end

function CGameController:Update(ElapsedTime)
	-- for i=1, (#self.m_Entities) do
		-- self.m_Entities[i]:Update(ElapsedTime)
	-- end
end

function CGameController:LoadXML(Filename)
	self.m_Filename = Filename
	self.m_Entities={}
	
	local l_Pedestal = nil
	
	local l_XMLTreeNode=CXMLTreeNode()
	local l_Loaded=l_XMLTreeNode:LoadFile(Filename)
	
	if l_Loaded == false then
		g_LogManager:Log("Unable to load game entities file")
	end
	
	if l_Loaded and l_XMLTreeNode:GetName() == "game_entities" then
		for i=0, l_XMLTreeNode:GetNumChildren() do
			local l_Element=l_XMLTreeNode:GetChild(i)
			local l_ElemName=l_Element:GetName()
			
			g_LogManager:Log("Going to load entity "..i)
			
			if l_ElemName=="enemy" then
				self:LoadEnemy(l_Element)
			elseif l_ElemName=="player" then
				self:LoadPlayer(l_Element)
			elseif l_ElemName=="pedestal" then
				l_Pedestal = self:LoadPedestal(l_Element)
			elseif l_ElemName=="dragon" then
				self:LoadDragon(l_Element)
			elseif l_ElemName=="shadow_manager" then
				self:LoadShadowManager(l_Element)
				elseif l_ElemName=="light_manager" then
				self:LoadLightManager(l_Element,l_Pedestal)
			end 
		end
		g_LogManager:Log("Game entities loaded.")
	else
		print("File '"..Filename.."'not correctly loaded")
	end
	--g_LogManager:Log("Game entities loaded...")
end 

function CGameController:Reload()
	self:Destroy()
	self:LoadXML(self.m_Filename)
	g_LogManager:Log("GameController reloaded")
end

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

function CGameController:LoadLightManager(XMLTreeNode, Pedestal)
	local l_GameObject = GetGameObject(XMLTreeNode)
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

function CGameController:LoadEnemy(XMLTreeNode)
	local l_GameObject = GetGameObject(XMLTreeNode)
	
	if l_GameObject ~= nil then

		local l_EnemyType = XMLTreeNode:GetPszProperty("enemy_type", "basic_enemy", false)
		local l_EnemyComponent = nil
		
		if l_EnemyType == "basic_enemy" then
			l_EnemyComponent  = CBasicEnemyComponent(l_GameObject)
		elseif l_EnemyType == "ranged_enemy" then
			l_EnemyComponent  = CRangedEnemyComponent(l_GameObject)
		elseif l_EnemyType == "brute_enemy" then
			l_EnemyComponent  = CBruteEnemyComponent(l_GameObject)
		end
		
		
		local l_ComponentName = l_GameObject:GetName().."_Script"
		g_ScriptManager:AddComponent(l_ComponentName,l_GameObject,l_EnemyComponent)
		
		l_EnemyComponent:Initialize()
		CGameController:LoadEnemyChilds(l_EnemyComponent,XMLTreeNode)
		table.insert(self.m_Entities,l_EnemyComponent)
		table.insert(self.m_Enemies,l_EnemyComponent)
	end
end

function CGameController:LoadEnemyChilds(Enemy, XMLTreeNode)
	for i=0, XMLTreeNode:GetNumChildren() do
		local l_Element=XMLTreeNode:GetChild(i)
		if l_Element:GetName()=="way_point" then
			local l_V = Vect3f(0.0,0.0,0.0)
			l_V = l_Element:GetVect3fProperty("way_point",Vect3f(0.0,0.0,0.0),false)
			Enemy:AddWaypoint(l_V)
		elseif l_Element:GetName()=="weapon" then
			self:LoadWeapon(l_Element,Enemy)
		elseif l_Element:GetName()=="armor" then
			self:LoadArmor(l_Element,Enemy)
		end
	end
end	

function CGameController:LoadWeapon(TreeNode, EnemyComponent)
	local l_WeaponGameObject = GetGameObject(TreeNode)
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
		EnemyComponent:SetWeapon(l_ArmorComponent)
	
	end
end

function CGameController:LoadArmor(TreeNode, EnemyComponent)
	local l_ArmorGameObject = GetGameObject(TreeNode)
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

function CGameController:LoadPlayer(XMLTreeNode)
	
	local l_GameObject = GetGameObject(XMLTreeNode)
	local l_RObject = l_GameObject:GetRenderableObject()
	
	g_GameObjectManager:SetPlayer(l_GameObject) --Tells layer manager who is the player.
	g_Player = l_GameObject --Tells Lua ...
	
	local l_PlayerComponent=CPlayerComponent(l_GameObject)
	local l_ComponentName = l_GameObject:GetName().."_Script"
	g_ScriptManager:AddComponent(l_ComponentName,l_GameObject,l_PlayerComponent)
	
	l_PlayerComponent:Initialize()
	g_PlayerComponent = l_PlayerComponent
	
	table.insert(self.m_Entities,l_PlayerComponent)
end

function CGameController:LoadDragon(XMLTreeNode)
	local l_GameObject = GetGameObject(XMLTreeNode)
	local l_RObject = l_GameObject:GetRenderableObject()
	local l_ParticleEmitter = GetParticleEmitter(XMLTreeNode)
	local l_BoneName = XMLTreeNode:GetPszProperty("bone_name", "", false)
	local l_ModelName = XMLTreeNode:GetPszProperty("model_name", "", false)
	
	--local l_CoreModel = g_AnimatedModelManager:GetResource(l_ModelName)
	local l_CoreModel = l_RObject:GetAnimatedCoreModel()
	local l_BoneID = l_CoreModel:GetBoneId(l_BoneName)
	
	if l_GameObject ~= nil and l_ParticleEmitter ~= nil and l_BoneName ~= "" then
		local l_DragonComponent = CDragonComponent(l_GameObject,l_ParticleEmitter,l_BoneID)
		l_GameObject:AddLuaComponent(l_DragonComponent)
		l_DragonComponent:Initialize()
		table.insert(self.m_Entities,l_DragonComponent)
	end
end

function CGameController:LoadPedestal(XMLTreeNode)
	local l_GameObject = GetGameObject(XMLTreeNode)
	local l_PedestalComponent=CPedestalComponent(l_GameObject)
	local l_ComponentName = l_GameObject:GetName().."_PedestalScript"
	g_ScriptManager:AddComponent(l_ComponentName,l_GameObject,l_PedestalComponent)
	table.insert(self.m_Entities,l_PedestalComponent)
	return l_PedestalComponent
end

function GetGameObject(XMLTreeNode)
	local l_GameObject = XMLTreeNode:GetPszProperty("game_object", "", false)
	return g_GameObjectManager:GetResource(l_GameObject)
end

function GetElementFromLayer(XMLTreeNode)
	local l_RenderableObject = XMLTreeNode:GetPszProperty("renderable_object", "", false)
	local l_LayerName = XMLTreeNode:GetPszProperty("layer", "", false)
	local l_RObject = g_LayerManager:GetResource(l_LayerName):GetResource(l_RenderableObject)
	return l_RObject
end
function GetParticleEmitter(XMLTreeNode)
	local l_Particle = XMLTreeNode:GetPszProperty("particle_emiter", "", false)
	local l_LayerName = "particles"
	local l_RObject = g_LayerManager:GetResource(l_LayerName):GetResource(l_Particle)
	return l_RObject
end