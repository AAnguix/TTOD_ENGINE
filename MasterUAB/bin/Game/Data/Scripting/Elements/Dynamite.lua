class 'CDynamiteComponent' (CActivableElement)
function CDynamiteComponent:__init(CLuaGameObject)
	CActivableElement.__init(self, CLuaGameObject, 3.0, "Dynamite", "DYNAMITE_EQUIPED")
end

function CDynamiteComponent:Update(ElapsedTime)
	CActivableElement.Update(self,ElapsedTime)
	if(CActivableElement.IsActivable(self)) then -- and g_ItemManager.m_HasDynamite==false
		local l_Pos = SGUIPosition(0.4,0.7,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoText("GetDynamiteText","felix_font",l_Pos,"","Press [E]")
	end
end