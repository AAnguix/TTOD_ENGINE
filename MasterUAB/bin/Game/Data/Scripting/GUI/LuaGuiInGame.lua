class 'CLuaGuiInGame' (CLuaGui)
function CLuaGuiInGame:__init()
	CLuaGui.__init(self)
	self.m_Name = "InGameGui"
end

function CLuaGuiInGame:Initialize()

	CLuaGui.Initialize(self)
	
	self:LoadAvatars()
	
	g_GUIManager:AddHealthBar("player_health_bar","player_health_bar_base","player_health_bar_top","player_health_bar_background")
	g_GUIManager:AddImage("map_image_1270_720","map_sprite_1280_720")
	
	g_GUIManager:AddFont("felix_font", "Data\\GUI\\Fonts\\Felix.fnt")
	
	g_GUIManager:AddButton("menu_button","menu_button_normal","menu_button_highlight","menu_button_pressed")
		g_GUIManager:AddButton("pause_button","pause_button_normal","pause_button_highlight","pause_button_pressed")
		g_GUIManager:AddButton("open_audio_submenu_button","open_audio_submenu_normal","open_audio_submenu_highlight","open_audio_submenu_pressed")	
			g_GUIManager:AddSlider("modify_fx_volume_slider","modify_fx_volume_slider_base","modify_fx_volume_slider_top","modify_fx_volume_slider_handle","modify_fx_volume_slider_pressed_handle")
			g_GUIManager:AddSlider("modify_music_volume_slider","modify_music_volume_slider_base","modify_music_volume_slider_top","modify_music_volume_slider_handle","modify_music_volume_slider_pressed_handle")		
end

function CLuaGuiInGame:LoadAvatars()
	g_GUIManager:AddImage("basic_enemy_avatar_image","basic_enemy_avatar_sprite")
	g_GUIManager:AddImage("player_avatar_image","player_avatar_sprite")
	-- g_GUIManager:AddImage("ranged_enemy_avatar_image","ranged_enemy_avatar_sprite")
	-- g_GUIManager:AddImage("charge_enemy_avatar_image","charge_enemy_avatar_sprite")
end

function CLuaGuiInGame:ESC_PRESSED()
	self:CheckMenu()	
end

function CLuaGuiInGame:Update(ElapsedTime)
	
	CLuaGui.Update(self,ElapsedTime)
	local l_HealthBarPos = SGUIPosition(0.83,0.05,0.3,0.075,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:DoHealthBar("player_health_bar_0","player_health_bar",l_HealthBarPos, 0.0, g_PlayerComponent:GetMaxHealth(), g_PlayerComponent:GetHealth()) 
	
	local l_AvatarPos = SGUIPosition(0.62,0.05, 0.05, 0.05, CGUIManager.TOP_LEFT, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:DoImage("player_avatar", "player_avatar_image", l_AvatarPos)
	
	local l_TextPos = SGUIPosition(0.5, 0.8, 0.1, 0.1, CGUIManager.BOTTOM_CENTER, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:DoText("LoadingText", "freestyle_script_64_font", l_TextPos, "", "Loading...", CColor(0.0,1.0,0.0,1.0))

	self:UpdateMenu()
	
	if g_PlayerComponent:IsMapOpened() then
		self:ShowMap()
	end
end

function CLuaGuiInGame:ShowMap()
	local l_MapPosition = SGUIPosition(0.0, 0.0, 1.0, 1.0, CGUIManager.BOTTOM_LEFT, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE)
	g_GUIManager:DoImage("map_0", "map_image_1270_720", l_MapPosition)
end

function CLuaGuiInGame:UpdateMenu()
	
	local l_MenuButtonPosition = SGUIPosition(0.95,0.75,0.05,0.05,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_Pressed = g_GUIManager:DoButton("menu_button_0","menu_button",l_MenuButtonPosition)
	if l_Pressed then 
		self:CheckMenu() 
	end
	
	if g_LuaGuiManager.m_AudioSubmenuActive then
		self:AudioSubMenu()
	else
		if g_LuaGuiManager.m_MenuActive then
			self:Menu()
			g_Engine:Pause()
		else 
			g_Engine:Resume()
		end
	end
	
end

function CLuaGuiInGame:CheckMenu()

	if g_LuaGuiManager.m_AudioSubmenuActive then
		g_LuaGuiManager.m_AudioSubmenuActive = not g_LuaGuiManager.m_AudioSubmenuActive
	else  
		g_LuaGuiManager.m_MenuActive  = not g_LuaGuiManager.m_MenuActive  
	end
end

function CLuaGuiInGame:Menu()
	local l_PauseButtonPosition = SGUIPosition(0.55,0.2,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_PausePressed = g_GUIManager:DoButton("pause_button_0","pause_button",l_PauseButtonPosition)
	if l_PausePressed then
		if g_Engine:IsPaused() then
			g_Engine:Resume()
		else 
			g_Engine:Pause()
		end
	end
	
	local l_OpenAudioSubmenuButtonPosition = SGUIPosition(0.55,0.4,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_Pressed = g_GUIManager:DoButton("open_audio_submenu_button_0","open_audio_submenu_button",l_OpenAudioSubmenuButtonPosition)
	if l_Pressed then g_LuaGuiManager.m_AudioSubmenuActive = not g_LuaGuiManager.m_AudioSubmenuActive  end
end

function CLuaGuiInGame:AudioSubMenu()
	local l_ModifyMusicVolumeSliderPosition = SGUIPosition(g_LuaGuiManager.m_MUSIC_SLIDER_XOFFSET,0.5,g_LuaGuiManager.m_MUSIC_SLIDER_WIDTH,g_LuaGuiManager.m_MUSIC_SLIDER_HEIGHT,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_ModifyFxVolumeSliderPosition = SGUIPosition(g_LuaGuiManager.m_MUSIC_SLIDER_XOFFSET,0.3,g_LuaGuiManager.m_MUSIC_SLIDER_WIDTH,g_LuaGuiManager.m_MUSIC_SLIDER_HEIGHT,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	g_LuaGuiManager.m_CurrentMusicVolume = g_GUIManager:DoSlider("modify_music_volume_slider_0","modify_music_volume_slider",l_ModifyMusicVolumeSliderPosition,g_LuaGuiManager.m_MinMusicVolume,g_LuaGuiManager.m_MaxMusicVolume,g_LuaGuiManager.m_CurrentMusicVolume.temp)
	g_LuaGuiManager.m_CurrentFxVolume = g_GUIManager:DoSlider("modify_fx_volume_slider_0","modify_fx_volume_slider",l_ModifyFxVolumeSliderPosition,g_LuaGuiManager.m_MinFxVolume,g_LuaGuiManager.m_MaxFxVolume,g_LuaGuiManager.m_CurrentFxVolume.temp)
	
	local l_fxRtpc = SoundRTPC("Hit_Volume")
	local l_MusicRtpc = SoundRTPC("WolfBlood_Volume")
	g_SoundManager:SetRTPCValue(l_fxRtpc,g_LuaGuiManager.m_CurrentFxVolume.temp)
	g_SoundManager:SetRTPCValue(l_MusicRtpc,g_LuaGuiManager.m_CurrentMusicVolume.temp)
end