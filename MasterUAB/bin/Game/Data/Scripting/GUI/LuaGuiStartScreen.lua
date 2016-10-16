class 'CLuaGuiStartScreen' (CLuaGui)
function CLuaGuiStartScreen:__init()
	CLuaGui.__init(self)
	self.m_Name = "StartScreenGui"
	self.m_Loading = false
	
	self.m_Active = {}
	self.m_Hot = {}
	self.m_xOffset = 0.5
	self.m_FirstY = 0.22
	self.m_YOffset = 0.15
	self:InitializeButtonStates()
	
	self.m_PlayButtonHot = false
	self.m_OptionsButtonHot = false
	self.m_OptionsButtonActive = false
	self.m_CreditsButtonHot = false
	self.m_ExitButtonHot = false
	
	self.m_SpriteWidth = 0.1
	self.m_SpriteHeight = 0.05
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
	g_LogManager:Log("initializando gui start screen")
	g_GUIManager:AddButton("play_button","play_sprite","play_sprite","play_sprite")
	g_GUIManager:AddButton("options_button","options_sprite","options_sprite","options_sprite")
	g_GUIManager:AddButton("credits_button","credits_sprite","credits_sprite","credits_sprite")
	g_GUIManager:AddButton("exit_button","exit_sprite","exit_sprite","exit_sprite")	
	g_GUIManager:AddImage("background_image","background_sprite")
	g_GUIManager:AddImage("main_menu_image","main_menu_sprite")
	--g_GUIManager:AddImage("basic_enemy_avatar_image","basic_enemy_avatar_sprite")
	g_GUIManager:AddFont("freestyle_script_64_font","Data/GUI/Fonts/FreestyleScript64.fnt")
	g_GUIManager:AddFont("freestyle_script_48_font","Data/GUI/Fonts/FreestyleScript48.fnt")
	g_GUIManager:AddFont("calibri_font", "Data\\GUI\\Fonts\\calibri.fnt")
	g_LogManager:Log("Start screen gui initialized")
end

function CLuaGuiStartScreen:ESC_PRESSED()
	ExitGame()	
end

function CLuaGuiStartScreen:Update(ElapsedTime)
	
	--self.m_Loading = false
	
	if (not self.m_Loading) then
		CLuaGui.Update(self,ElapsedTime)
		-- local l_Pos = SGUIPosition(0.2, 0.2, 0.2, 0.2, CGUIManager.BOTTOM_LEFT, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE)
		-- g_GUIManager:DoImage("main_menu_0", "health_bar_image", l_Pos)
		
		-- local l_Pos = SGUIPosition(0.5, 0.5, 0.12, 0.2, CGUIManager.BOTTOM_LEFT, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE)
		-- g_GUIManager:DoImage("orc_avatar_image_0", "orc_avatar_image", l_Pos)
		
		self:PlayButton()
		self:OptionsButton()
		self:CreditsButton()
		self:ExitButton()
	else 
		StartLevelOne() 
		self.m_Loading = false
	end
end

-------------------- BUTTONS --------------------
function CLuaGuiStartScreen:PlayButton()
	local l_PlayPos = SGUIPosition(self.m_xOffset,self.m_FirstY,self.m_SpriteWidth,self.m_SpriteHeight,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_ButtonState = g_GUIManager:DoSButton("play_button_0","play_button",l_PlayPos,CColor(1.0,1.0,1.0,1.0))
	self:HandleSoundEvents("Play",l_ButtonState)
	if l_ButtonState.pressed then
		g_SoundManager:PlayEvent(SoundEvent(g_LuaGuiManager.m_ButtonPressedSound))
		self.m_Loading = true
	end
end

function CLuaGuiStartScreen:OptionsButton()
	local l_OptionsPos = SGUIPosition(self.m_xOffset,self.m_FirstY+self.m_YOffset,self.m_SpriteWidth,self.m_SpriteHeight,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_ButtonState = g_GUIManager:DoSButton("options_button_0","options_button",l_OptionsPos,CColor(1.0,1.0,1.0,1.0))
	self:HandleSoundEvents("Options",l_ButtonState)
	if l_ButtonState.pressed then
		g_SoundManager:PlayEvent(SoundEvent(g_LuaGuiManager.m_ButtonPressedSound))
		self:AudioMenu()
	end
end

function CLuaGuiStartScreen:CreditsButton()
	local l_CreditsPos = SGUIPosition(self.m_xOffset,self.m_FirstY+(2*self.m_YOffset),self.m_SpriteWidth,self.m_SpriteHeight,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_ButtonState = g_GUIManager:DoSButton("credits_button_0","credits_button",l_CreditsPos,CColor(1.0,1.0,1.0,1.0))
	self:HandleSoundEvents("Credits",l_ButtonState)
	if l_ButtonState.pressed then
		g_SoundManager:PlayEvent(SoundEvent(g_LuaGuiManager.m_ButtonPressedSound))
		self:CreditsMenu()
	end
end

function CLuaGuiStartScreen:ExitButton()
	local l_ExitPos = SGUIPosition(self.m_xOffset,self.m_FirstY+(3*self.m_YOffset),self.m_SpriteWidth,self.m_SpriteHeight,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
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

