-- DRAGON
function CGameController:LoadDragon(XMLTreeNode)
	local l_GameObject = GetLuaGameObjectHandle(XMLTreeNode)
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

-- ORCS
function CGameController:LoadEnemy(XMLTreeNode)
	local l_GameObject = GetLuaGameObjectHandle(XMLTreeNode)
	
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

-- PLAYER
function CGameController:LoadPlayer(XMLTreeNode)
	
	g_Player = g_GameController:AddLuaGameObjectHandle(XMLTreeNode)
	local l_GameObject = g_Player:GetGameObject()
	local l_ComponentName = l_GameObject:GetName().."_Script"
	
 	g_Engine:GetGameObjectManager():SetPlayer(l_GameObject) --Tells layer manager who is the player.
	
	local l_PlayerComponent=CPlayerComponent(g_Player)
	g_ScriptManager:AddComponent(l_ComponentName,l_GameObject,l_PlayerComponent)
	
	l_PlayerComponent:Initialize()
	g_PlayerComponent = l_PlayerComponent
	
	table.insert(self.m_Entities,l_PlayerComponent)
end