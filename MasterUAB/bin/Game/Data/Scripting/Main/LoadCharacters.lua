-- DRAGON
function CGameController:LoadDragon(XMLTreeNode)
	local l_Name = XMLTreeNode:GetPszProperty("game_object", "", false)
	g_Dragon = g_GameController:AddLuaGameObjectHandle(l_Name)
	local l_ComponentName = g_Dragon:GetName().."_Script"
	
	local l_ParticleEmitter = GetParticleEmitter(XMLTreeNode)
	local l_BoneName = XMLTreeNode:GetPszProperty("bone_name", "", false)
	local l_ModelName = XMLTreeNode:GetPszProperty("model_name", "", false)
	
	local l_CoreModel = g_Dragon:GetAnimatedCoreModel()
	local l_BoneID = l_CoreModel:GetBoneId(l_BoneName)
	
	if g_Dragon ~= nil and l_ParticleEmitter ~= nil and l_BoneName ~= "" then
		local l_DragonComponent = CDragonComponent(g_Dragon,l_ParticleEmitter,l_BoneID)
		g_ScriptManager:AddComponent(l_ComponentName,g_Dragon,l_DragonComponent)
		l_DragonComponent:Initialize()
		table.insert(self.m_Entities,l_DragonComponent)
	end
end

-- PLAYER
function CGameController:LoadPlayer(XMLTreeNode)
	local l_Name = XMLTreeNode:GetPszProperty("game_object", "", false)
	g_Player = g_GameController:AddLuaGameObjectHandle(l_Name)
	local l_ComponentName = g_Player:GetName().."_Script"
 	
	g_Engine:GetGameObjectManager():SetPlayer(g_Player:GetGameObject()) --Tells layer manager who is the player.
	
	local l_PlayerComponent=CPlayerComponent(g_Player)
	g_ScriptManager:AddComponent(l_ComponentName,g_Player,l_PlayerComponent)
	
	l_PlayerComponent:Initialize()
	CGameController:LoadPlayerChilds(l_PlayerComponent,XMLTreeNode)
	g_PlayerComponent = l_PlayerComponent
	
	table.insert(self.m_Entities,l_PlayerComponent)
	g_LogManager:Log("Player initialized...")
end

function CGameController:LoadPlayerChilds(Enemy, XMLTreeNode)
	g_LogManager:Log("Loading player childs")
	for i=0, XMLTreeNode:GetNumChildren()-1 do
		local l_Element=XMLTreeNode:GetChild(i)
		if l_Element:GetName()=="weapon" then
			self:LoadWeapon(l_Element,Enemy)
		elseif l_Element:GetName()=="armor" then
			self:LoadArmor(l_Element,Enemy)
		end
	end
end	

-- ORCS
function CGameController:LoadEnemy(XMLTreeNode)
	local l_Name = XMLTreeNode:GetPszProperty("game_object", "", false)
	
	local l_LuaGameObject = g_GameController:AddLuaGameObjectHandle(l_Name)
	if l_LuaGameObject ~= nil then
		local l_EnemyType = XMLTreeNode:GetPszProperty("enemy_type", "basic_enemy", false)
		local l_EnemyComponent = nil
		
		if l_EnemyType == "basic_enemy" then
			l_EnemyComponent  = CBasicEnemyComponent(l_LuaGameObject)
		elseif l_EnemyType == "ranged_enemy" then
			l_EnemyComponent  = CRangedEnemyComponent(l_LuaGameObject)
		elseif l_EnemyType == "brute_enemy" then
			l_EnemyComponent  = CBruteEnemyComponent(l_LuaGameObject)
		end
		
		local l_ComponentName = l_LuaGameObject:GetName().."_Script"
		g_ScriptManager:AddComponent(l_ComponentName,l_LuaGameObject,l_EnemyComponent)
		l_EnemyComponent:Initialize()
		CGameController:LoadEnemyChilds(l_EnemyComponent,XMLTreeNode)
		table.insert(self.m_Entities,l_EnemyComponent)
		table.insert(self.m_Enemies,l_EnemyComponent)
	end
end

function CGameController:LoadEnemyChilds(Enemy, XMLTreeNode)
	for i=0, XMLTreeNode:GetNumChildren()-1 do
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

function CGameController:LoadShowHealthBarManager(XMLTreeNode)
	local l_Name = XMLTreeNode:GetPszProperty("game_object", "", false)
	
	local l_LuaGameObject = g_GameController:AddLuaGameObjectHandle(l_Name)
	if l_LuaGameObject ~= nil then
		local l_ShowHealthBarComponent  = CShowHealthBarManagerComponent(l_LuaGameObject)
		local l_ComponentName = l_LuaGameObject:GetName().."_Script"
		g_ScriptManager:AddComponent(l_ComponentName,l_LuaGameObject,l_ShowHealthBarComponent)
		g_ShowHealthBarManager = l_ShowHealthBarComponent
		table.insert(self.m_Entities,l_ShowHealthBarComponent)
	end
end

