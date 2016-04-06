function InitializeGUI()
	g_GUIManager:AddButton("play_button","play_button_normal","play_button_highlight","play_button_pressed")
	g_GUIManager:AddButton("options_button","options_button_normal","options_button_highlight","options_button_pressed")
	-- g_GUIManager:AddButton("credits_button","credits_button_normal","credits_button_highlight","credits_button_pressed")
	-- g_GUIManager:AddButton("exit_button","exit_button_normal","exit_button_highlight","exit_button_pressed")	
end

function UpdateGUI()

	local l_Pressed = false
	local l_PlayPos = SGUIPosition(0.1,0.1,0.05,0.05,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	l_Pressed = g_GUIManager:DoButton("play_button_0","play_button",l_PlayPos)
	if l_Pressed then
		g_GUIManager:Load("./Data/gui_in_game.xml")
		dofile("./Data/Scripting/GUIInGame.lua")
		InitializeGUI()
		--g_Engine:LoadLevel("1")
	end
	
	local l_OptionsPos = SGUIPosition(0.3,0.3,0.05,0.05,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	l_Pressed = g_GUIManager:DoButton("options_button_0","options_button",l_OptionsPos)
	if l_Pressed then
		-- dofile("./Data/Scripting/GUIInGame.lua")
		-- InitializeGUI()
	end
end