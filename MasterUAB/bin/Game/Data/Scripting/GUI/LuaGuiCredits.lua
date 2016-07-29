class 'CLuaGuiCredits' (CLuaGui)
function CLuaGuiCredits:__init()
	CLuaGui.__init(self)
	self.m_Name = "CreditsGui"
end

function CLuaGuiCredits:Initialize()
	g_GUIManager:AddFont("nirmala_48_font", "Data\\GUI\\Fonts\\Nirmala48.fnt")
	g_GUIManager:AddImage("background_image", "background_sprite")
end

function CLuaGuiCredits:ESC_PRESSED()
	g_LuaGuiManager:SetGui(1)
end

function CLuaGuiCredits:Update(ElapsedTime)
	local l_BackgroundImagePos = SGUIPosition(0.0,0.0,1.0,1.0,CGUIManager.TOP_LEFT,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE)
	g_GUIManager:DoImage("CreditsBackgroundImage","background_image",l_BackgroundImagePos)
	
	local l_Pos = SGUIPosition(g_LuaGuiManager.m_CREDITS_AUTHORS_XOFFSET,g_LuaGuiManager.m_CREDITS_AUTHORS_YOFFSET,0.0,0.0,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:DoText("CreditsText","nirmala_48_font",l_Pos,"","CREDITS")
	
	local l_Pos = SGUIPosition(g_LuaGuiManager.m_CREDITS_AUTHORS_XOFFSET,g_LuaGuiManager.m_CREDITS_AUTHORS_YOFFSET+0.1,0.0,0.0,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:DoText("Programmers","nirmala_48_font",l_Pos,"","PROGRAMMERS")
	
	local l_Pos = SGUIPosition(g_LuaGuiManager.m_CREDITS_AUTHORS_XOFFSET,g_LuaGuiManager.m_CREDITS_AUTHORS_YOFFSET+0.2,0.0,0.0,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:DoText("Programmer1","nirmala_48_font",l_Pos,"","Arturo Anguix Lopez")
	
	local l_Pos = SGUIPosition(g_LuaGuiManager.m_CREDITS_AUTHORS_XOFFSET,g_LuaGuiManager.m_CREDITS_AUTHORS_YOFFSET+0.3,0.0,0.0,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:DoText("Programmer2","nirmala_48_font",l_Pos,"","Jesus Rojas")
	
	local l_Pos = SGUIPosition(g_LuaGuiManager.m_CREDITS_AUTHORS_XOFFSET,g_LuaGuiManager.m_CREDITS_AUTHORS_YOFFSET+0.4,0.0,0.0,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:DoText("Artist","nirmala_48_font",l_Pos,"","ARTISTS")
	
	local l_Pos = SGUIPosition(g_LuaGuiManager.m_CREDITS_AUTHORS_XOFFSET,g_LuaGuiManager.m_CREDITS_AUTHORS_YOFFSET+0.5,0.0,0.0,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:DoText("Artist1","nirmala_48_font",l_Pos,"","Aina Gomar")
	
	local l_Pos = SGUIPosition(g_LuaGuiManager.m_CREDITS_AUTHORS_XOFFSET,g_LuaGuiManager.m_CREDITS_AUTHORS_YOFFSET+0.6,0.0,0.0,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:DoText("Artist2","nirmala_48_font",l_Pos,"","Laura Muchada")
end



