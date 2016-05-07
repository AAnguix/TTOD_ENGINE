function InitializeGUI()
	g_GUIManager:AddHealthBar("player_health_bar","player_health_bar_base","player_health_bar_top","player_health_bar_background")
	-- g_GUIManager:AddButton("menu_button","menu_button_normal","menu_button_highlight","menu_button_pressed")
end

g_IsMenuActive = false;

function UpdateGUI(ElapsedTime)
	
	--local l_HealthBarPos = SGUIPosition(50,50,512,128)
	local l_HealthBarPos = SGUIPosition(0.83,0.05,0.3,0.075,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	
	l_Pressed = g_GUIManager:DoHealthBar("player_health_bar_0","player_health_bar",l_HealthBarPos, 0.0, 100.0, g_PlayerComponent:GetHealth())
	
	-- local l_Pressed = false
	if CInputManager.GetInputManager():IsActionActive("ESC_PRESSED") then
		g_IsMenuActive = not g_IsMenuActive 
	end
	
	if g_IsMenuActive then
		Menu()
	end
end

function Menu()
	-- local l_OptionsButton = SGUIPosition(0.9,0.9,0.05,0.05,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	-- l_Pressed = g_GUIManager:DoButton("menu_button_0","menu_button",l_OptionsButton)
end