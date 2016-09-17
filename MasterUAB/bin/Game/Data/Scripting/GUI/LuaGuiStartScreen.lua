class 'CLuaGuiStartScreen' (CLuaGui)
function CLuaGuiStartScreen:__init()
	CLuaGui.__init(self)
	self.m_Name = "StartScreenGui"
	self.m_Loading = false
	
	self.m_Active = {}
	self.m_Hot = {}
	self.m_xOffset = 0.5
	self:InitializeButtonStates()
	
	self.m_PlayButtonHot = false
	self.m_OptionsButtonHot = false
	self.m_OptionsButtonActive = false
	self.m_CreditsButtonHot = false
	self.m_ExitButtonHot = false
end

function CLuaGuiStartScreen:InitializeButtonStates()
	self.m_Active["Play"] = false
	self.m_Active["Options"] = false
	self.m_Active["Credits"] = false
	self.m_Active["Exit"] = false
	self.m_Hot["Play"] = false
	self.m_Hot["Options"] = false
	self.m_Hot["Credits"] = false
	self.m_Hot["Exit"] = false
end

function CLuaGuiStartScreen:Initialize()
	CLuaGui.Initialize(self)

	g_GUIManager:AddButton("play_button","play_button_normal","play_button_highlight","play_button_pressed")
	g_GUIManager:AddButton("options_button","options_button_normal","options_button_highlight","options_button_pressed")
	g_GUIManager:AddButton("credits_button","credits_button_normal","credits_button_highlight","credits_button_pressed")
	g_GUIManager:AddButton("exit_button","exit_button_normal","exit_button_highlight","exit_button_pressed")	
	g_GUIManager:AddImage("background_image","background_sprite")	
	g_GUIManager:AddFont("freestyle_script_64_font","Data/GUI/Fonts/FreestyleScript64.fnt")
	g_GUIManager:AddFont("freestyle_script_48_font","Data/GUI/Fonts/FreestyleScript48.fnt")
	g_GUIManager:AddFont("calibri_font", "Data\\GUI\\Fonts\\calibri.fnt")
	g_LogManager:Log("Start screen gui initialized")
end

function CLuaGuiStartScreen:ESC_PRESSED()
	ExitGame()	
end

function CLuaGuiStartScreen:Update(ElapsedTime)
	
	self.m_Loading = true
	
	if (not self.m_Loading) then
		CLuaGui.Update(self,ElapsedTime)
		self:PlayButton()
		self:OptionsButton()
		self:CreditsButton()
		self:ExitButton()
	else 
		StartLevelOne() 
	end
end

-------------------- BUTTONS --------------------
function CLuaGuiStartScreen:PlayButton()
	local l_PlayPos = SGUIPosition(self.m_xOffset,0.025,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_ButtonState = g_GUIManager:DoSButton("play_button_0","play_button",l_PlayPos,CColor(1.0,1.0,1.0,1.0))
	self:HandleSoundEvents("Play",l_ButtonState)
	if l_ButtonState.pressed then
		g_SoundManager:PlayEvent(SoundEvent(g_LuaGuiManager.m_ButtonPressedSound))
		self.m_Loading = true
	end
end

function CLuaGuiStartScreen:OptionsButton()
	local l_OptionsPos = SGUIPosition(self.m_xOffset,0.22,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_ButtonState = g_GUIManager:DoSButton("options_button_0","options_button",l_OptionsPos,CColor(1.0,1.0,1.0,1.0))
	self:HandleSoundEvents("Options",l_ButtonState)
	if l_ButtonState.pressed then
		g_SoundManager:PlayEvent(SoundEvent(g_LuaGuiManager.m_ButtonPressedSound))
		self:AudioMenu()
	end
end

function CLuaGuiStartScreen:CreditsButton()
	local l_CreditsPos = SGUIPosition(self.m_xOffset,0.415,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_ButtonState = g_GUIManager:DoSButton("credits_button_0","credits_button",l_CreditsPos,CColor(1.0,1.0,1.0,1.0))
	self:HandleSoundEvents("Credits",l_ButtonState)
	if l_ButtonState.pressed then
		g_SoundManager:PlayEvent(SoundEvent(g_LuaGuiManager.m_ButtonPressedSound))
		self:CreditsMenu()
	end
end

function CLuaGuiStartScreen:ExitButton()
	local l_ExitPos = SGUIPosition(self.m_xOffset,0.61,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_ButtonState = g_GUIManager:DoSButton("exit_button_0","exit_button",l_ExitPos,CColor(1.0,1.0,1.0,1.0))
	self:HandleSoundEvents("Exit",l_ButtonState)
	if l_ButtonState.pressed then
		g_SoundManager:PlayEvent(SoundEvent(g_LuaGuiManager.m_ButtonPressedSound))
		ExitGame()
	end
end

function CLuaGuiStartScreen:HandleSoundEvents(ButtonName, ButtonState)
	if((not self.m_Active[ButtonName])and(not self.m_Hot[ButtonName]) and (ButtonState.hot)) then
		g_SoundManager:PlayEvent(SoundEvent(g_LuaGuiManager.m_HotElementSound))	
	end
	self.m_Hot[ButtonName] = ButtonState.hot
	self.m_Active[ButtonName] = ButtonState.active
end


function CLuaGuiStartScreen:CreditsMenu()g_LuaGuiManager:SetGui(3) end
function CLuaGuiStartScreen:OptionsMenu() g_LuaGuiManager:SetGui(4) end
function CLuaGuiStartScreen:AudioMenu() g_LuaGuiManager:SetGui(5) end

