class 'CItemManager'
function CItemManager:__init()
	g_EventManager:Subscribe(self, "DYNAMITE_DROPPED")
	g_EventManager:Subscribe(self, "DYNAMITE_EQUIPED")
	g_EventManager:Subscribe(self, "DYNAMITE_USED")
	self.m_Dynamite = nil
	self.m_HasDynamite = false
end

function CItemManager:LoadItems(Level)
	if (Level=="1") then
		local l_DynamiteGameObjectHandle = g_GameController:AddLuaGameObjectHandle("dynamite")
		self.m_Dynamite = CDynamiteComponent(l_DynamiteGameObjectHandle)
		g_ScriptManager:AddComponent("Dynamite_Script",self.m_Dynamite:GetLuaGameObject(),self.m_Dynamite)
		table.insert(g_GameController:GetEntities(),self.m_Dynamite)
		self.m_Dynamite:GetLuaGameObject():EnableRenderableObject(false)
	end
end

function CItemManager:DYNAMITE_DROPPED(Position)
	self.m_Dynamite:GetLuaGameObject():EnableRenderableObject(true)
	self.m_Dynamite:GetLuaGameObject():SetPosition(Position)
end

function CItemManager:DYNAMITE_EQUIPED()
	self.m_Dynamite:GetLuaGameObject():EnableRenderableObject(false)
	local l_DynamiteGuiComponent = CDynamiteGuiComponent()
	l_DynamiteGuiComponent:AddButton("dynamite_0", "dynamite", "dynamite_normal", "dynamite_highlight", "dynamite_pressed",CColor(1.0,1.0,1.0,1.0))
	g_PlayerComponent.m_Inventory:AddItem(l_DynamiteGuiComponent,1)
	self.m_HasDynamite = true
	self.m_Dynamite:UnsuscribeEvents()
	g_EventManager:Unsubscribe(self,"DYNAMITE_EQUIPED", true)
	g_GameController:RemoveEntity("dynamite")
	self.m_Dynamite = nil
end

function CItemManager:DYNAMITE_USED()
	--Lanzar cinemática y sonido
	g_PlayerComponent.m_Inventory:RemoveItem(2)
	self.m_HasDynamite = false
	--g_LayerManager:GetResource("cinematics"):GetResource("rocks_cinematic"):Play(false)
end


