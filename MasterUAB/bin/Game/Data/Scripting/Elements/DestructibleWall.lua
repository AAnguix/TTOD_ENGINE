class 'CDestructibleWall' (CActivableElement)
function CDestructibleWall:__init(CLuaGameObject)
	CActivableElement.__init(self, CLuaGameObject, 1.5, "DestructibleWall")
	g_EventManager:Subscribe( self, "EPressed" )
	local l_GameObject = self.m_LuaGameObject:GetGameObject()
	local l_AudioSourceName = self.m_LuaGameObject:GetName().."_AudioSource"
	g_SoundManager:AddComponent(l_AudioSourceName, l_GameObject)
	--self.m_LuaGameObject:AddSound("DynamiteSound","Play_DynamiteSound")
end

--Event
function CDestructibleWall:EPressed()
	if(CActivableElement.IsActivable(self)) then
		self:ActivateDynamite()
	end
end

function CDestructibleWall:Update(ElapsedTime)
	CActivableElement.Update(self,ElapsedTime)
	local l_PlayerHasDynamite = true --g_PlayerComponent:...
	if(CActivableElement.IsActivable(self) and l_PlayerHasDynamite) then
		local l_Pos = SGUIPosition(0.4,0.7,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoText("DynamiteText","felix_font",l_Pos,"","Activate dynamite [E]")
	end
end

function CDestructibleWall:ActivateDynamite()
	--Lanzar cinematica de las rocas
	g_LogManager:Log("BUMMMM")
	g_EventManager:FireEvent("DynamiteActivated")
end