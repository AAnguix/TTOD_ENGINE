class 'CItemManager'
function CItemManager:__init()
	self.m_Dynamite = nil
	self.m_HasDynamite = false
	self.m_DynamiteDroped = false
	g_EventManager:Subscribe(self, "DYNAMITE_EQUIPED")
	g_EventManager:Subscribe(self, "DYNAMITE_USED")
	g_EventManager:Subscribe(self, "DYNAMITE_DROPPED")
end

function CItemManager:LoadLevelTwoItems()
	local l_DynamiteGameObjectHandle = g_GameController:AddLuaGameObjectHandle("dynamite")
	self.m_Dynamite = CDynamiteComponent(l_DynamiteGameObjectHandle)
	g_ScriptManager:AddComponent("Dynamite_Script",self.m_Dynamite:GetLuaGameObject(),self.m_Dynamite)
	table.insert(g_GameController:GetEntities(),self.m_Dynamite)
	self.m_Dynamite:GetLuaGameObject():EnableRenderableObject(false)
end

function CItemManager:DYNAMITE_DROPPED(Position)
	self.m_Dynamite:GetLuaGameObject():EnableRenderableObject(true)
	self.m_Dynamite:GetLuaGameObject():SetPosition(Position)
end

function CItemManager:DYNAMITE_EQUIPED()
	g_LogManager:Log("DINAMITA EQUIPADA")
	self.m_Dynamite:GetLuaGameObject():EnableRenderableObject(false)
	local l_DynamiteGuiComponent = CDynamiteGuiComponent()
	l_DynamiteGuiComponent:AddButton("dynamite_0", "dynamite", "dynamite_normal", "dynamite_highlight", "dynamite_pressed",CColor(1.0,1.0,1.0,1.0))
	g_PlayerComponent.m_Inventory:AddItem(l_DynamiteGuiComponent,1)
	self.m_HasDynamite = true
	g_GameController:RemoveElementFromTable(g_GameController:GetEntities(),"dynamite")
	self.m_Dynamite = nil
end

function CItemManager:DYNAMITE_USED()
	--Lanzar cinemática y sonido
	g_PlayerComponent.m_Inventory:RemoveItem(2)
	self.m_HasDynamite = false
	--g_LayerManager:GetResource("cinematics"):GetResource("rocks_cinematic"):Play(false)
end


