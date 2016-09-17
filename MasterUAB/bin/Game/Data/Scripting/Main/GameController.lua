class 'CGameController'
function CGameController:__init()
	self:ClearTables()
end

-- Includes
dofile("./Data/Scripting/Weapons/DamageCalculator.lua")
dofile("./Data/Scripting/Main/LoadCharacters.lua")
dofile("./Data/Scripting/Main/LoadEntities.lua")

dofile("./Data/Scripting/Main/GameControllerRemover.lua")

function CGameController:ClearTables()
	self.m_Entities = {}
	self.m_Enemies = {}
	self.m_Armors = {}
	self.m_Weapons = {}
	self.m_Filename = ""
end

function CGameController:RestartLastCheckPoint()
	--Reproducir sonido
	g_LogManager:Log("Player is DEAD")
	g_Engine:SetTimeScale(1.0)
	self:Destroy()
	local l_CurrentLevel = g_Engine:GetCurrentLevel()
	g_Engine:UnloadLevel(l_CurrentLevel)
	g_Engine:LoadLevel(l_CurrentLevel)
	self:LoadXML("Data/Level"..l_CurrentLevel.."/game_entities.xml")
end

dofile("./Data/Scripting/Main/GameControllerGets.lua")

function CGameController:Update(ElapsedTime)
end

function CGameController:LoadLevel(Level)
	g_Engine:AddLevel(Level)
	g_Engine:LoadLevel(Level)
	LuaMain() --Load new LUA behavior (per frame)
	self:LoadXML("Data/Level"..Level.."/game_entities.xml")
	g_AIManager:LoadLevel(Level)
	g_ItemManager:LoadItems(Level)
end

function CGameController:LoadLevelIndependentData()
	local l_LuaGameObject = g_GameController:AddLuaGameObjectHandle("healthBarManager")
	g_ShowHealthBarManager  = CShowHealthBarManagerComponent(l_LuaGameObject)
	g_ScriptManager:AddComponent("healthBarManager",l_LuaGameObject,g_ShowHealthBarManager)
	table.insert(self.m_Entities,g_ShowHealthBarManager)
end

function CGameController:LoadXML(Filename)
	
	self:ClearTables()
	self.m_Filename = Filename
	
	local l_Pedestal = nil
	
	local l_XMLTreeNode=CXMLTreeNode()
	local l_Loaded=l_XMLTreeNode:LoadFile(Filename)
	
	if l_Loaded == false then
		g_LogManager:Log("Unable to load game entities file")
	end
	
	if l_Loaded and l_XMLTreeNode:GetName() == "game_entities" then
		for i=0, l_XMLTreeNode:GetNumChildren()-1 do
			local l_Element=l_XMLTreeNode:GetChild(i)
			local l_ElemName=l_Element:GetName()
			
			g_LogManager:Log("Loading entity "..i)
			
			if l_ElemName=="enemy" then
				self:LoadEnemy(l_Element)
			elseif l_ElemName=="player" then
				self:LoadPlayer(l_Element)
			
			-- elseif l_ElemName=="magic_ball" then
				-- self:LoadMagicBall(l_Element)
			-- elseif l_ElemName=="statue" then
				-- self:LoadStatue(l_Element)
			elseif l_ElemName=="dragon" then
				self:LoadDragon(l_Element)
				
			elseif l_ElemName=="pedestal" then
				l_Pedestal = self:LoadPedestal(l_Element)
			elseif l_ElemName=="shadow_manager" then
				self:LoadShadowManager(l_Element)
			elseif l_ElemName=="light_manager" then
				self:LoadLightManager(l_Element,l_Pedestal)
			elseif l_ElemName=="destructible_wall" then
				self:LoadDestructibleWall(l_Element)
			end 
			
			g_LogManager:Log("Entity "..i.." loaded")
		end
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

function CGameController:AddLuaGameObjectHandle(GameObjectName)
	g_LogManager:Log("Adding handle "..GameObjectName)
	local l_GameObject = g_Engine:GetGameObjectManager():GetResource(GameObjectName)
	if l_GameObject ~= nil then
		local l_LuaGameObjectHandle = g_LuaGameObjectHandleManager:Add(l_GameObject)
		return l_LuaGameObjectHandle
	else
		g_LogManager:Log("Error. Trying to create handle with null GameObject. "..GameObjectName)
	end
	return nil
end

function CGameController:GetElementFromLayer(XMLTreeNode)
	local l_RenderableObject = XMLTreeNode:GetPszProperty("renderable_object", "", false)
	local l_LayerName = XMLTreeNode:GetPszProperty("layer", "", false)
	local l_RObject = g_LayerManager:GetResource(l_LayerName):GetResource(l_RenderableObject)
	return l_RObject
end
function CGameController:GetParticleEmitter(XMLTreeNode)
	local l_Particle = XMLTreeNode:GetPszProperty("particle_emiter", "", false)
	local l_LayerName = "particles"
	local l_RObject = g_LayerManager:GetResource(l_LayerName):GetResource(l_Particle)
	return l_RObject
end

function CGameController:PrintEnemyNames()
	g_LogManager:Log("There are "..#self.m_Enemies.." enemies")
	for i=1, (#self.m_Enemies) do
		if(self.m_Enemies[i].m_LuaGameObject ~= nil) then
			g_LogManager:Log(self.m_Enemies[i].m_LuaGameObject:GetName())
		else
			g_LogManager:Log("nil")
		end
	end
end

function CGameController:PrintEntitiesNames()
	g_LogManager:Log("There are "..#self.m_Entities.." entities")
	for i=(#self.m_Entities), 1, -1 do
		if(self.m_Entities[i].m_LuaGameObject ~= nil) then
			g_LogManager:Log(self.m_Entities[i].m_LuaGameObject:GetName())
		else
			g_LogManager:Log("nil")
		end
	end
end

function CGameController:PrintWeaponNames()
	g_LogManager:Log("There are "..#self.m_Weapons.." weapons")
	for i=1, (#self.m_Weapons) do
		if(self.m_Weapons[i].m_LuaGameObject ~= nil) then
			g_LogManager:Log(self.m_Weapons[i].m_LuaGameObject:GetName())
		else
			g_LogManager:Log("nil")
		end
	end
end
