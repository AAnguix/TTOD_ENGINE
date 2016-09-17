class 'CLuaGuiAudioMenu' (CLuaGui)
function CLuaGuiAudioMenu:__init()
	CLuaGui.__init(self)
	self.m_Name = "AudioMenuGui"

	self.m_MinMusicVolume = 0.0
	self.m_MaxMusicVolume = 100.0
	self.m_MusicVolumeSlider = SSliderResult()
	self.m_MusicVolumeSlider.temp = 100.0
	self.m_MinFxVolume = 0.0
	self.m_MaxFxVolume = 100.0
	self.m_FxVolumeSlider = SSliderResult()
	self.m_FxVolumeSlider.temp = 100.0
	
	self.MUSIC_SLIDER_WIDTH = 0.35
	self.MUSIC_SLIDER_HEIGHT = 0.075
	self.MUSIC_SLIDER_XOFFSET = 0.5
	
	self.m_Hot = {}
	self.m_Active = {}
	self.m_LastTimeSoundPlayed = 0.0
	self.m_Timer = 0.0
	self.m_PlaySoundDealy = 0.015
	self.m_MinimumValueToPlaySound = 0.1
	
	self.m_Hot["MusicVolumeSlider"] = false
	self.m_Hot["FxVolumeSlider"] = false
	self.m_Active["MusicVolumeSlider"] = false
	self.m_Active["FxVolumeSlider"] = false
end

function CLuaGuiAudioMenu:Initialize()
	CLuaGui.Initialize(self)
	g_GUIManager:AddSlider("modify_fx_volume_slider","modify_fx_volume_slider_base","modify_fx_volume_slider_top","modify_fx_volume_slider_handle","modify_fx_volume_slider_pressed_handle")
	g_GUIManager:AddSlider("modify_music_volume_slider","modify_music_volume_slider_base","modify_music_volume_slider_top","modify_music_volume_slider_handle","modify_music_volume_slider_pressed_handle")		
end

function CLuaGuiAudioMenu:ESC_PRESSED()
	g_LuaGuiManager:SetGui(1)
end

function CLuaGuiAudioMenu:Update(ElapsedTime)
	CLuaGui.Update(self,ElapsedTime)
	self.m_Timer = self.m_Timer + ElapsedTime
	
	local l_ModifyMusicVolumeSliderPosition = SGUIPosition(self.MUSIC_SLIDER_XOFFSET,0.5,self.MUSIC_SLIDER_WIDTH,self.MUSIC_SLIDER_HEIGHT,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_ModifyFxVolumeSliderPosition = SGUIPosition(self.MUSIC_SLIDER_XOFFSET,0.3,self.MUSIC_SLIDER_WIDTH,self.MUSIC_SLIDER_HEIGHT,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	self.m_MusicVolumeSlider = g_GUIManager:DoSlider("modify_music_volume_slider_0","modify_music_volume_slider",l_ModifyMusicVolumeSliderPosition,self.m_MinMusicVolume,self.m_MaxMusicVolume,self.m_MusicVolumeSlider.temp)
	self.m_FxVolumeSlider = g_GUIManager:DoSlider("modify_fx_volume_slider_0","modify_fx_volume_slider",l_ModifyFxVolumeSliderPosition,self.m_MinFxVolume,self.m_MaxFxVolume,self.m_FxVolumeSlider.temp)
	
	self:HandleSliderSound("MusicVolumeSlider",self.m_MusicVolumeSlider,"Play_Scroll3")
	self:HandleSliderSound("FxVolumeSlider",self.m_FxVolumeSlider,"Play_Scroll2")
	
	-- local l_fxRtpc = SoundRTPC("Hit_Volume")
	-- local l_MusicRtpc = SoundRTPC("WolfBlood_Volume")
	-- g_SoundManager:SetRTPCValue(l_fxRtpc,self.m_FxVolumeSlider.temp)
	-- g_SoundManager:SetRTPCValue(l_MusicRtpc,self.m_MusicVolumeSlider.temp)
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



