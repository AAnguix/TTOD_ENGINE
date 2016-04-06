function InitializeGUI()
	g_GUIManager:AddHealthBar("player_health_bar","player_health_bar_base","player_health_bar_top","player_health_bar_background")
	-- g_GUIManager:AddButton("menu_button","menu_button_normal","menu_button_highlight","menu_button_pressed")
end

g_PlayerH = 100.0

function UpdateGUI()

	-- local l_Pressed = false
	-- local l_MenuButtonPos = SGUIPosition(0.9,0.9,0.05,0.05,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	-- l_Pressed = g_GUIManager:DoButton("menu_button_0","menu_button",l_MenuButtonPos)
	
	local l_HealthBarPos = SGUIPosition(50,50,512,128)
	l_Pressed = g_GUIManager:DoHealthBar("player_health_bar_0","player_health_bar",l_HealthBarPos, 0.0, 100.0, g_PlayerLUA:GetHealth())
	
end