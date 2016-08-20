-- DRAGON
function CGameController:LoadDragon(XMLTreeNode)
	local l_Name = XMLTreeNode:GetPszProperty("game_object", "", false)
	local l_LuaGameObject = g_GameController:AddLuaGameObjectHandle(l_Name)
	if l_LuaGameObject ~= nil then
		local l_ComponentName = l_LuaGameObject:GetName()
		--local l_ParticleEmitter = GetParticleEmitter(XMLTreeNode)
		local l_BoneName = XMLTreeNode:GetPszProperty("bone_name", "", false)
		local l_ModelName = XMLTreeNode:GetPszProperty("model_name", "", false)
		
		local l_CoreModel = l_LuaGameObject:GetAnimatedCoreModel()
		local l_BoneID = l_CoreModel:GetBoneId(l_BoneName)
		
		--if l_LuaGameObject ~= nil and l_ParticleEmitter ~= nil and l_BoneName ~= "" then
			--local l_DragonComponent = CDragonComponent(l_LuaGameObject,l_ParticleEmitter,l_BoneID)
			--g_ScriptManager:AddComponent(l_ComponentName,l_LuaGameObject,l_DragonComponent)
			--l_DragonComponent:Initialize()
			--table.insert(self.m_Entities,l_DragonComponent)
		--end
		if l_LuaGameObject ~= nil and l_BoneName ~= "" then
			local l_DragonComponent = CDragonComponent(l_LuaGameObject,l_BoneID)
			g_ScriptManager:AddComponent(l_ComponentName,l_LuaGameObject,l_DragonComponent)
			
			CGameController:LoadSkeleton(l_DragonComponent,XMLTreeNode)
			l_DragonComponent:Initialize()
			g_DragonComponent = l_DragonComponent
			table.insert(self.m_Entities,l_DragonComponent)
			g_LogManager:Log("Se crea el dragon bien")
		else
			g_LogManager:Log("Problemas al crear el dragon")
		end
	else
		g_LogManager:Log("Unable to create dragon")
	end
end

function CGameController:LoadSkeleton(EntityComponent,XMLTreeNode)
	for i=0, XMLTreeNode:GetNumChildren()-1 do
		
		local l_Element=XMLTreeNode:GetChild(i)
		local l_ElemName=l_Element:GetName()
		
		if l_ElemName ~= "" then
			
			for j=0, l_Element:GetNumChildren()-1 do
				local l_ElementEmbedded=l_Element:GetChild(j)
				local l_BoneName = l_ElementEmbedded:GetPszProperty("bone_name", "", false)
				--g_LogManager:Log("LoadSkeleton Bone: "..l_BoneName)
				local l_PhysicsType = l_ElementEmbedded:GetPszProperty("type", "", false)
				local l_PhysicsSize = nil
				
				local l_GameObject = g_Engine:GetGameObjectManager():AddGameObject(l_BoneName);
				local l_LuaGameObject = g_GameController:AddLuaGameObjectHandle(l_BoneName)
				local l_ComponentName = l_LuaGameObject:GetName()
				
				if l_LuaGameObject ~= nil then
				
					
					if l_PhysicsType == "box" then
						l_PhysicsSize = l_ElementEmbedded:GetVect3fProperty("size", Vect3f(0.0,0.0,0.0), false)
					elseif l_PhysicsType == "sphere" then
						l_PhysicsSize = l_ElementEmbedded:GetFloatProperty("size", 0.0, false)
					else
						g_LogManager:Log("Error al setear el tipo de fisica para el hueso: "..l_BoneName)
					end
					l_DragonPhysicsPiece = CDragonPhysicsPiece(EntityComponent.m_LuaGameObject,l_LuaGameObject, l_BoneName, l_PhysicsType, l_PhysicsSize)
					if l_DragonPhysicsPiece ~= nil then
						EntityComponent:AddPxPiece(l_ElemName, l_DragonPhysicsPiece)
						--g_ScriptManager:AddComponent("PUPU",nil,nil)
						g_ScriptManager:AddComponent(l_BoneName,l_LuaGameObject,l_DragonPhysicsPiece)
					else
						g_LogManager:Log("Error al hacer AddPxPiece para el hueso: "..l_BoneName)
					end
				else
					g_LogManager:Log("Unable to create LuaGameObject for: "..l_BoneName)	
				end
			end	
		end 
	
	end
	
	--g_LogManager:Log("Terminamos el LoadSkeleton")
end

-- PLAYER
function CGameController:LoadPlayer(XMLTreeNode)
	local l_Name = XMLTreeNode:GetPszProperty("game_object", "", false)
	g_Player = g_GameController:AddLuaGameObjectHandle(l_Name)
	local l_ComponentName = g_Player:GetName()
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
		local l_DeadEvent = XMLTreeNode:GetPszProperty("dead_event", "", false)
		
		if l_EnemyType == "basic_enemy" then
			l_EnemyComponent  = CBasicEnemyComponent(l_LuaGameObject)
		elseif l_EnemyType == "ranged_enemy" then
			l_EnemyComponent  = CRangedEnemyComponent(l_LuaGameObject)
		elseif l_EnemyType == "brute_enemy" then
			l_EnemyComponent  = CBruteEnemyComponent(l_LuaGameObject)
		end
		
		local l_ComponentName = l_LuaGameObject:GetName()
		g_ScriptManager:AddComponent(l_ComponentName,l_LuaGameObject,l_EnemyComponent)
		l_EnemyComponent:Initialize()
		l_EnemyComponent:SetDeadEvent(l_DeadEvent)
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
