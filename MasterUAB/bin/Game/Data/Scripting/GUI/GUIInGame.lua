function InitializeGUI()
	g_GUIManager:AddHealthBar("player_health_bar","player_health_bar_base","player_health_bar_top","player_health_bar_background")
	
	g_GUIManager:AddFont("felix_font", "Data\\GUI\\Fonts\\Felix.fnt")
	g_GUIManager:AddFont("calibri_font", "Data\\GUI\\Fonts\\calibri.fnt")
	
	
	g_GUIManager:AddButton("menu_button","menu_button_normal","menu_button_highlight","menu_button_pressed")
		g_GUIManager:AddButton("pause_button","pause_button_normal","pause_button_highlight","pause_button_pressed")
		g_GUIManager:AddButton("open_audio_submenu_button","open_audio_submenu_normal","open_audio_submenu_highlight","open_audio_submenu_pressed")	
			g_GUIManager:AddSlider("modify_fx_volume_slider","modify_fx_volume_slider_base","modify_fx_volume_slider_top","modify_fx_volume_slider_handle","modify_fx_volume_slider_pressed_handle")
			g_GUIManager:AddSlider("modify_music_volume_slider","modify_music_volume_slider_base","modify_music_volume_slider_top","modify_music_volume_slider_handle","modify_music_volume_slider_pressed_handle")
end

g_IsMenuActive = false
g_IsAudioSubmenuActive = false

g_MinMusicVolume = 0.0
g_MaxMusicVolume = 100.0
g_CurrentMusicVolume = SSliderResult()
g_CurrentMusicVolume.temp = 100.0

g_MinFxVolume = 0.0
g_MaxFxVolume = 100.0
g_CurrentFxVolume = SSliderResult()
g_CurrentFxVolume.temp = 100.0

MUSIC_SLIDER_WIDTH = 0.35
MUSIC_SLIDER_HEIGHT = 0.075
MUSIC_SLIDER_XOFFSET = 0.5

GRAPHICS_STATS_X_DISPLACEMENT = 0.97
GRAPHICS_STATS_Y_DISPLACEMENT = 0.08
GRAPHICS_STATS_Y_OFFSET = 0.025

g_ShowGraphicsStats = true

function UpdateGUI(ElapsedTime)
	
	--local l_HealthBarPos = SGUIPosition(50,50,512,128)
	local l_HealthBarPos = SGUIPosition(0.83,0.05,0.3,0.075,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:DoHealthBar("player_health_bar_0","player_health_bar",l_HealthBarPos, 0.0, g_PlayerComponent:GetMaxHealth(), g_PlayerComponent:GetHealth()) 
	
	ShowGraphicsStats()
	-- local l_Pos = SGUIPosition(0.1,0.1,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	-- g_GUIManager:DoText("textoDePrueba","felix_font",l_Pos,"","Game is paused!")
	--void DoText(const std::string& GuiID, const std::string& Font, const SGUIPosition& Position, const std::string& Sprite, const std::string& Text);
	
	UpdateMenu()
end

function ShowGraphicsStats()
	
	if g_ShowGraphicsStats then
		local l_FPSPos = SGUIPosition(GRAPHICS_STATS_X_DISPLACEMENT,GRAPHICS_STATS_Y_DISPLACEMENT,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoText("FramesPerSecond","calibri_font",l_FPSPos,"","Fps:"..g_GraphicsStats:GetFps())
		
		local l_MillisecondsPerFrameTextPos = SGUIPosition(GRAPHICS_STATS_X_DISPLACEMENT,GRAPHICS_STATS_Y_DISPLACEMENT+GRAPHICS_STATS_Y_OFFSET,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoText("MillisecondsPerFrameText","calibri_font",l_MillisecondsPerFrameTextPos,"","Millisecs/frame")
		
		local l_MillisecondsPerFramePos = SGUIPosition(GRAPHICS_STATS_X_DISPLACEMENT,GRAPHICS_STATS_Y_DISPLACEMENT+(GRAPHICS_STATS_Y_OFFSET*2),0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoText("MillisecondsPerFrame","calibri_font",l_MillisecondsPerFramePos,"","Frame "..Round(g_GraphicsStats:GetMilliSecondsPerFrame(),2))
		
		local l_MinMillisecondsPerFramePos = SGUIPosition(GRAPHICS_STATS_X_DISPLACEMENT,GRAPHICS_STATS_Y_DISPLACEMENT+(GRAPHICS_STATS_Y_OFFSET*3),0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoText("MinMillisecondsPerFrame","calibri_font",l_MinMillisecondsPerFramePos,"","Min "..Round(g_GraphicsStats:GetMinMillisecondsPerFrame(),2))
		
		local l_AvgMillisecondsPerFramePos = SGUIPosition(GRAPHICS_STATS_X_DISPLACEMENT,GRAPHICS_STATS_Y_DISPLACEMENT+(GRAPHICS_STATS_Y_OFFSET*4),0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoText("AvgMillisecondsPerFrame","calibri_font",l_AvgMillisecondsPerFramePos,"","Avg "..Round(g_GraphicsStats:GetAvgMillisecondsPerFrame(),2))
		
		local l_MaxMillisecondsPerFramePos = SGUIPosition(GRAPHICS_STATS_X_DISPLACEMENT,GRAPHICS_STATS_Y_DISPLACEMENT+(GRAPHICS_STATS_Y_OFFSET*5),0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoText("MaxMillisecondsPerFrame","calibri_font",l_MaxMillisecondsPerFramePos,"","Max "..Round(g_GraphicsStats:GetMaxMillisecondsPerFrame(),2))
	end
end

function CheckMenu()
	if g_IsAudioSubmenuActive then
		g_IsAudioSubmenuActive = not g_IsAudioSubmenuActive
	else  
		g_IsMenuActive = not g_IsMenuActive 
	end
end

function UpdateMenu()
	
	local l_MenuButtonPosition = SGUIPosition(0.95,0.85,0.05,0.05,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_Pressed = g_GUIManager:DoButton("menu_button_0","menu_button",l_MenuButtonPosition)
	if l_Pressed then 
		CheckMenu() 
	end
	
	if CInputManager.GetInputManager():IsActionActive("ESC_PRESSED") then
		CheckMenu()
	end
	
	if g_IsAudioSubmenuActive then
		AudioSubMenu()
	elseif g_IsMenuActive then
		Menu()
	end
	
end

function Menu()
	local l_PauseButtonPosition = SGUIPosition(0.55,0.2,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_PausePressed = g_GUIManager:DoButton("pause_button_0","pause_button",l_PauseButtonPosition)
	if l_PausePressed then
		CApplication.Pause()
	end
	
	local l_OpenAudioSubmenuButtonPosition = SGUIPosition(0.55,0.4,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_Pressed = g_GUIManager:DoButton("open_audio_submenu_button_0","open_audio_submenu_button",l_OpenAudioSubmenuButtonPosition)
	if l_Pressed then g_IsAudioSubmenuActive = not g_IsAudioSubmenuActive  end
end

function AudioSubMenu()
	local l_ModifyMusicVolumeSliderPosition = SGUIPosition(MUSIC_SLIDER_XOFFSET,0.5,MUSIC_SLIDER_WIDTH,MUSIC_SLIDER_HEIGHT,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_ModifyFxVolumeSliderPosition = SGUIPosition(MUSIC_SLIDER_XOFFSET,0.3,MUSIC_SLIDER_WIDTH,MUSIC_SLIDER_HEIGHT,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	g_CurrentMusicVolume = g_GUIManager:DoSlider("modify_music_volume_slider_0","modify_music_volume_slider",l_ModifyMusicVolumeSliderPosition,g_MinMusicVolume,g_MaxMusicVolume,g_CurrentMusicVolume.temp)
	g_CurrentFxVolume = g_GUIManager:DoSlider("modify_fx_volume_slider_0","modify_fx_volume_slider",l_ModifyFxVolumeSliderPosition,g_MinFxVolume,g_MaxFxVolume,g_CurrentFxVolume.temp)
	
	local l_fxRtpc = SoundRTPC("Hit_Volume")
	local l_MusicRtpc = SoundRTPC("WolfBlood_Volume")
	g_SoundManager:SetRTPCValue(l_fxRtpc,g_CurrentFxVolume.temp)
	g_SoundManager:SetRTPCValue(l_MusicRtpc,g_CurrentMusicVolume.temp)
end