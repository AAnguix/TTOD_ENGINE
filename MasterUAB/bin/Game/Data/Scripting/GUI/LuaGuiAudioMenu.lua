class 'CLuaGuiAudioMenu' (CLuaGui)
function CLuaGuiAudioMenu:__init()
	CLuaGui.__init(self)
	self.m_Name = "AudioMenuGui"

	self.m_MinVolumeValue = 0.0
	self.m_MaxVolumeValue = 100.0
	
	self.m_GameVolumeSlider = SSliderResult()
	self.m_GameVolumeSlider.temp = 100.0
	self.m_MusicVolumeSlider = SSliderResult()
	self.m_MusicVolumeSlider.temp = 100.0
	self.m_FxVolumeSlider = SSliderResult()
	self.m_FxVolumeSlider.temp = 100.0
	self.m_VoicesVolumeSlider = SSliderResult()
	self.m_VoicesVolumeSlider.temp = 100.0
	
	self.AUDIO_SLIDER_WIDTH = 0.35
	self.AUDIO_SLIDER_HEIGHT = 0.075
	self.AUDIO_SLIDER_XOFFSET = 0.5
	
	self.m_Hot = {}
	self.m_Active = {}
	self.m_LastTimeSoundPlayed = 0.0
	self.m_Timer = 0.0
	self.m_PlaySoundDealy = 0.015
	self.m_MinimumValueToPlaySound = 0.1
	
	self.m_Hot["GameVolumeSlider"] = false
	self.m_Hot["MusicVolumeSlider"] = false
	self.m_Hot["FxVolumeSlider"] = false
	self.m_Hot["VoicesVolumeSlider"] = false
	
	self.m_Active["GameVolumeSlider"] = false
	self.m_Active["MusicVolumeSlider"] = false
	self.m_Active["FxVolumeSlider"] = false
	self.m_Active["VoicesVolumeSlider"] = false
end

function CLuaGuiAudioMenu:Initialize()
	CLuaGui.Initialize(self)
	g_GUIManager:AddSlider("game_volume_slider","game_volume_slider_base","game_volume_slider_top","game_volume_slider_handle","game_volume_slider_pressed_handle")
	g_GUIManager:AddSlider("modify_fx_volume_slider","modify_fx_volume_slider_base","modify_fx_volume_slider_top","modify_fx_volume_slider_handle","modify_fx_volume_slider_pressed_handle")
	g_GUIManager:AddSlider("modify_music_volume_slider","modify_music_volume_slider_base","modify_music_volume_slider_top","modify_music_volume_slider_handle","modify_music_volume_slider_pressed_handle")		
end

function CLuaGuiAudioMenu:ESC_PRESSED()
	g_LuaGuiManager:SetGui(1)
end

function CLuaGuiAudioMenu:Update(ElapsedTime)
	CLuaGui.Update(self,ElapsedTime)
	self.m_Timer = self.m_Timer + ElapsedTime
	
	local l_GameVolumeSliderPosition = SGUIPosition(self.AUDIO_SLIDER_XOFFSET,0.3,self.AUDIO_SLIDER_WIDTH,self.AUDIO_SLIDER_HEIGHT,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	self.m_GameVolumeSlider = g_GUIManager:DoSlider("game_volume_slider_0","game_volume_slider",l_GameVolumeSliderPosition,self.m_MinVolumeValue,self.m_MaxVolumeValue,self.m_VoicesVolumeSlider.temp)
	
	local l_MusicVolumeSliderPosition = SGUIPosition(self.AUDIO_SLIDER_XOFFSET,0.5,self.AUDIO_SLIDER_WIDTH,self.AUDIO_SLIDER_HEIGHT,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	self.m_MusicVolumeSlider = g_GUIManager:DoSlider("modify_music_volume_slider_0","modify_music_volume_slider",l_MusicVolumeSliderPosition,self.m_MinVolumeValue,self.m_MaxVolumeValue,self.m_MusicVolumeSlider.temp)
	
	local l_FxVolumeSliderPosition = SGUIPosition(self.AUDIO_SLIDER_XOFFSET,0.3,self.AUDIO_SLIDER_WIDTH,self.AUDIO_SLIDER_HEIGHT,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	self.m_FxVolumeSlider = g_GUIManager:DoSlider("modify_fx_volume_slider_0","modify_fx_volume_slider",l_FxVolumeSliderPosition,self.m_MinVolumeValue,self.m_MaxVolumeValue,self.m_FxVolumeSlider.temp)
	
	local l_VoicesVolumeSliderPosition = SGUIPosition(self.AUDIO_SLIDER_XOFFSET,0.3,self.AUDIO_SLIDER_WIDTH,self.AUDIO_SLIDER_HEIGHT,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	self.m_VoicesVolumeSlider = g_GUIManager:DoSlider("voices_volume_slider_0","voices_volume_slider",l_VoicesVolumeSliderPosition,self.m_MinVolumeValue,self.m_MaxVolumeValue,self.m_VoicesVolumeSlider.temp)
	
	self:HandleSliderSound("GameVolumeSlider",self.m_GameVolumeSlider,"Play_Scroll2")
	self:HandleSliderSound("MusicVolumeSlider",self.m_MusicVolumeSlider,"Play_Scroll3")
	self:HandleSliderSound("FxVolumeSlider",self.m_FxVolumeSlider,"Play_Scroll2")
	self:HandleSliderSound("VoicesVolumeSlider",self.m_VoicesVolumeSlider,"Play_Scroll3")
	
	local l_GameRtpc = SoundRTPC("GameVolume")
	local l_FxRtpc = SoundRTPC("FxVolume")
	local l_MusicRtpc = SoundRTPC("MusicVolume")
	local l_VoicesRtpc = SoundRTPC("VoicesVolume")
	g_SoundManager:SetRTPCValue(l_GameRtpc,self.m_GameVolumeSlider.temp)
	g_SoundManager:SetRTPCValue(l_FxRtpc,self.m_FxVolumeSlider.temp)
	g_SoundManager:SetRTPCValue(l_MusicRtpc,self.m_MusicVolumeSlider.temp)
	g_SoundManager:SetRTPCValue(l_VoicesRtpc,self.m_VoicesVolumeSlider.temp)
end

function CLuaGuiAudioMenu:HandleSliderSound(SliderName, SliderResult, Sound)
	-- if((not self.m_Active[SliderName])and(not self.m_Hot[SliderName]) and (SliderResult.hot)) then
		-- g_SoundManager:PlayEvent(SoundEvent(Sound))	
	-- end
	
	if( (self.m_Active[SliderName]) 
	and (math.abs(self.m_Timer-self.m_LastTimeSoundPlayed)>self.m_PlaySoundDealy) 
	and (math.abs(SliderResult.real - SliderResult.temp)>self.m_MinimumValueToPlaySound))
	then
		g_SoundManager:PlayEvent(SoundEvent(Sound))	
		self.m_LastTimeSoundPlayed = self.m_Timer
		self.m_Timer=0.0
	elseif((not self.m_Active[SliderName]) and (SliderResult.active)) then
		g_SoundManager:PlayEvent(SoundEvent(Sound))	
	end
	self.m_Hot[SliderName] = SliderResult.hot
	self.m_Active[SliderName] = SliderResult.active
end



