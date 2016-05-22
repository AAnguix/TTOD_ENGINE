function InitializeGUI()
	g_GUIManager:AddButton("play_button","play_button_normal","play_button_highlight","play_button_pressed")
	g_GUIManager:AddButton("options_button","options_button_normal","options_button_highlight","options_button_pressed")
	g_GUIManager:AddButton("credits_button","credits_button_normal","credits_button_highlight","credits_button_pressed")
	g_GUIManager:AddButton("exit_button","exit_button_normal","exit_button_highlight","exit_button_pressed")	
	g_GUIManager:AddImage("background_image","background_sprite")	
	g_GUIManager:AddFont("fontTest","Data/GUI/Fonts/fontTest.fnt")
end

g_TextBoxResult = ""
g_Loading = false

xOffset = 0.5

function UpdateGUI(ElapsedTime)

	local l_Pressed = false
	
	local l_PlayPos = SGUIPosition(xOffset,0.025,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	l_Pressed = g_GUIManager:DoButton("play_button_0","play_button",l_PlayPos)
	if l_Pressed then
		g_Loading = true
	end
	
	if g_Loading then
		ShowLoadScreen()
		StartLevelOne()
	end
	
	local l_OptionsPos = SGUIPosition(xOffset,0.22,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	l_Pressed = g_GUIManager:DoButton("options_button_0","options_button",l_OptionsPos)
	if l_Pressed then
		OptionsMenu()
	end
	
	local l_CreditsPos = SGUIPosition(xOffset,0.415,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	l_Pressed = g_GUIManager:DoButton("credits_button_0","credits_button",l_CreditsPos)
	if l_Pressed then
		CreditsMenu()
	end
	
	local l_ExitPos = SGUIPosition(xOffset,0.61,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	l_Pressed = g_GUIManager:DoButton("exit_button_0","exit_button",l_ExitPos)
	if l_Pressed then
		ExitGame()
	end
	
	-- local l_TBPos = SGUIPosition(350,350,256,64)
	-- g_TextBoxResult = g_GUIManager:DoTextBox("textBox1","fontTest",l_TBPos,"textBox",g_TextBoxResult, ElapsedTime)
end

function ShowLoadScreen()
	local l_BackgroundPos = SGUIPosition(0.0,0.0,1280.0,720.0)
	--local l_BackgroundPos = SGUIPosition(0.0,0.0,1.0,1.0,CGUIManager.TOP_CENTER,CGUIManager.GUI_ABSOLUTE,CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:DoImage("background_image_0","background_image",l_BackgroundPos)
end

function StartLevelOne()
	
	g_LogManager:Log("Level one is going to be loaded.")
	g_GUIManager:Load("./Data/gui_in_game.xml")
	
	dofile("./Data/Scripting/LuaMainInGame.lua")
	LoadGamePlayScripts() --LuaMainInGame.lua
	g_Engine:LoadLevel("1")
	g_CurrentLevel = 1
	g_DamageCalculator:LoadXML("Data/damage_reduction_percentages.xml")
	g_GameController:LoadXML("Data/Level1/game_entities.xml")
	
	g_LogManager:Log("Level one loaded...")
	
	LuaMain() --Load new LUA behavior (per frame)

	g_CameraControllerManager:SetCurrentCameraController("ThirdPersonCamera")
	g_SceneRendererCommandManager:Reload()
	g_LogManager:Log("Level one loaded.")
end



function OptionsMenu()

end

function CreditsMenu()

end

function ExitGame()
	CEngine.TerminateApplication()
end