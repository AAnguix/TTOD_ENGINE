dofile("./Data/Scripting/Weapons/DamageCalculator.lua")

class 'CGameController'
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

function CGameController:GetEntities()
	return self.m_Entities
end

function CGameController:GetEnemies()
	return self.m_Enemies
end

function CGameController:GetEnemy(EnemyName)
	return self.m_Enemies[EnemyName]
end

function CGameController:Update(ElapsedTime)
	-- for i=1, (#self.m_Entities) do
		-- self.m_Entities[i]:Update(ElapsedTime)
	-- end
end

function CGameController:LoadLevel(Level)
	self:LoadXML("Data/Level"..Level.."/game_entities.xml")
	g_AIManager:LoadLevel(Level)
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
			
			g_LogManager:Log("Loading entity "..i)
			
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
			elseif l_ElemName=="destructible_wall" then
				self:LoadDestructibleWall(l_Element)
			end 
			
			g_LogManager:Log("Entity "..i.." loaded")
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

-- function CGameController:GetLuaGameObjectHandle(XMLTreeNode)
	-- local l_GameObjectName = XMLTreeNode:GetPszProperty("game_object", "", false)
	-- local l_LuaGameObjectHandle = g_LuaGameObjectHandleManager:Get(l_GameObjectName)
	-- if l_LuaGameObjectHandle == nil then
		-- g_LogManager:Log("NULL. GameobjectHandle "..l_GameObjectName.." doesn't exists")
	-- end
	-- return l_LuaGameObjectHandle
-- end

function CGameController:AddLuaGameObjectHandle(GameObjectName)
	local l_GameObject = g_GameObjectManager:GetResource(GameObjectName)
	if l_GameObject ~= nil then
		local l_LuaGameObjectHandle = g_LuaGameObjectHandleManager:Add(l_GameObject)
		return l_LuaGameObjectHandle
	else
		g_LogManager:Log("Error. Trying to create handle with null GameObject")
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

function CGameController:GetEnemy(EnemyName)
	for i=1, (#self.m_Enemies) do
		if self.m_Enemies[i].m_LuaGameObject:GetName() == EnemyName then
			return self.m_Enemies[i]
		end
	end
end

function CGameController:PrintEnemyNames()
	for i=1, (#self.m_Enemies) do
		g_LogManager:Log(self.m_Enemies[i].m_LuaGameObject:GetName())
	end
end

dofile("./Data/Scripting/Main/LoadEntities.lua")