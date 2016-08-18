class 'CDestructibleWall' (CActivableElement)
function CDestructibleWall:__init(CLuaGameObject)
	CActivableElement.__init(self, CLuaGameObject, 3.0, "DestructibleWall", "DYNAMITE_USED")
	--g_EventManager:Subscribe( self, "PLAYER_INTERACTS" )
	local l_AudioSourceName = self.m_LuaGameObject:GetName().."_AudioSource"
	g_SoundManager:AddComponent(l_AudioSourceName, self.m_LuaGameObject)
	--self.m_LuaGameObject:AddSound("DynamiteSound","Play_DynamiteSound")
end

--Event
-- function CDestructibleWall:PLAYER_INTERACTS()
	-- if(CActivableElement.IsActivable(self)) then
		-- self:ActivateDynamite()
	-- end
-- end

function CDestructibleWall:Update(ElapsedTime)
	CActivableElement.Update(self,ElapsedTime)
	if(CActivableElement.IsActivable(self) and g_ItemManager.m_HasDynamite) then
		local l_Pos = SGUIPosition(0.4,0.7,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoText("ActivateDynamiteText","felix_font",l_Pos,"","Activate dynamite [E]")
	end
end