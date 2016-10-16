class 'CLuaGuiInGame' (CLuaGui)
function CLuaGuiInGame:__init()
	CLuaGui.__init(self)
	self.m_Name = "InGameGui"
	
	self.m_OptionsMenuActive = false
	self.m_AudioMenuActive = false
	
	self.m_HBBackgroundX = 0.07
	self.m_HBX = self.m_HBBackgroundX + 0.1
	self.m_AvatarX = self.m_HBBackgroundX - 0.04
	
	self.m_HBBackgroundY = 0.07
	self.m_HBY = self.m_HBBackgroundY + 0.01
	self.m_AvatarY = self.m_HBBackgroundY - 0.034
	
	self.m_HBBackgroundWidth = 0.2
	self.m_HBWidth = self.m_HBBackgroundWidth*0.8
	
	self.m_HBBackgrounHeight = 0.025
	self.m_HBHeight = self.m_HBBackgrounHeight/1.65 --0.015
	
	self.m_AvatarWidth = 0.07
	self.m_AvatarHeight = self.m_AvatarWidth
end

function CLuaGuiInGame:Initialize()

	CLuaGui.Initialize(self)
	
	self:LoadAvatars()
	
	g_GUIManager:AddHealthBar("player_health_bar","healthbar_sprite_base","healthbar_sprite_top")
	g_GUIManager:AddImage("healthbar_image_background","healthbar_sprite_background")
		
	g_GUIManager:AddImage("map_image_1270_720","map_sprite_1280_720")
	g_GUIManager:AddImage("dead_black_texture_image","dead_black_texture_sprite")
	g_GUIManager:AddFont("felix_font", "Data\\GUI\\Fonts\\Felix.fnt")
	
	g_GUIManager:AddButton("resume_button","resume_sprite","resume_sprite","resume_sprite")
	g_GUIManager:AddButton("options_button","options_sprite","options_sprite","options_sprite")	
	g_GUIManager:AddButton("exit_button","exit_sprite","exit_sprite","exit_sprite")	
	
	g_GUIManager:AddButton("restart_game_button","restart_game_button_normal","restart_game_button_highlight","restart_game_button_pressed")
end

function CLuaGuiInGame:LoadAvatars()
	g_GUIManager:AddImage("basic_enemy_avatar_image","basic_enemy_avatar_sprite")
	-- g_GUIManager:AddImage("ranged_enemy_avatar_image","ranged_enemy_avatar_sprite")
	-- g_GUIManager:AddImage("charge_enemy_avatar_image","charge_enemy_avatar_sprite")
end

function CLuaGuiInGame:ESC_PRESSED()
	if g_PlayerComponent:IsMapOpened() == false then
		self:CheckMenu()	
		g_SoundManager:PlayEvent(SoundEvent("Play_Button"))	
	end
end

function CLuaGuiInGame:Update(ElapsedTime)
	
	
	local l_Position = SGUIPosition(0.5,0.0,0.05,0.05,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:DoText("angle","calibri_font",l_Position,"","Yaaaaaw: "..Round(g_PlayerComponent:GetLuaGameObject():GetYaw(),2))
	
	CLuaGui.Update(self,ElapsedTime)
	--Console(ElapsedTime)
	if g_PlayerComponent.m_Dead == false then
		self:PlayerAliveGui()
	else
		local l_Alpha = 1.0 - ((g_PlayerComponent.m_CountdownToExtintionTimer)/(g_PlayerComponent.m_CountdownToExtintion))
		local l_BlackTexturePos = SGUIPosition(0.0, 0.0, 1.0, 1.0, CGUIManager.BOTTOM_LEFT, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE)
		g_GUIManager:DoImage("dead_black_texture_image_0", "dead_black_texture_image", l_BlackTexturePos, CColor(1.0,1.0,1.0,l_Alpha))
	end

	self:UpdateMenu()
end

function CLuaGuiInGame:UpdateMenu()
	
	if self.m_AudioMenuActive then
		self:AudioSubMenu()
	else
		if self.m_OptionsMenuActive then
			self:Menu()
			g_Engine:Pause()
		else 
			g_Engine:Resume()
		end
	end
	
end

function CLuaGuiInGame:CheckMenu()
	
	if((not self.m_OptionsMenuActive)and(not self.m_AudioMenuActive)) then
		self.m_OptionsMenuActive = true
	else
		if self.m_AudioMenuActive then
			self.m_AudioMenuActive = false
			self.m_OptionsMenuActive = true	
		elseif self.m_OptionsMenuActive then
			self.m_OptionsMenuActive = false
		end
	end
end

function CLuaGuiInGame:Menu()
	local l_PauseButtonPosition = SGUIPosition(0.55,0.25,0.13,0.06,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_PausePressed = g_GUIManager:DoButton("resume_button","resume_button",l_PauseButtonPosition)
	if l_PausePressed then
		if g_Engine:IsPaused() then
			g_Engine:Resume()
		else 
			g_Engine:Pause()
		end
	end
	
	local l_OptionsPosition = SGUIPosition(0.55,0.45,0.13,0.06,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_Pressed = g_GUIManager:DoButton("options_button","options_button",l_OptionsPosition)
	if l_Pressed then self.m_OptionsMenuActive = not self.m_OptionsMenuActive  end
end

function CLuaGuiInGame:AudioSubMenu()
	g_LuaGuiManager:SetGui(5)
end

function CLuaGuiInGame:ShowMap()
	local l_MapPosition = SGUIPosition(0.0, 0.0, 1.0, 1.0, CGUIManager.BOTTOM_LEFT, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE)
	g_GUIManager:DoImage("map_0", "map_image_1270_720", l_MapPosition)
end

function CLuaGuiInGame:PlayerAliveGui()
	local l_HealthBarBackgroundPos = SGUIPosition(self.m_HBBackgroundX,self.m_HBBackgroundY,self.m_HBBackgroundWidth,self.m_HBBackgrounHeight,CGUIManager.TOP_LEFT, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:DoImage("healthbar_image_background", "healthbar_image_background", l_HealthBarBackgroundPos)
	local l_HealthBarPos = SGUIPosition(self.m_HBX,self.m_HBY,self.m_HBWidth,self.m_HBHeight,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:DoHealthBar("player_health_bar_0","player_health_bar",l_HealthBarPos, 0.0, g_PlayerComponent:GetMaxHealth(), g_PlayerComponent:GetHealth()) 
	
	local l_AvatarPos = SGUIPosition(self.m_AvatarX,self.m_AvatarY,self.m_AvatarWidth,self.m_AvatarHeight,CGUIManager.TOP_LEFT, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:DoImage("player_avatar", "basic_enemy_avatar_image", l_AvatarPos)

	if g_PlayerComponent:IsMapOpened() then
		self:ShowMap()
	end
end

function CLuaGuiInGame:PLAYER_IS_DEAD()
	local l_ExitButtonPosition = SGUIPosition(0.35,0.5,0.2,0.05,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_ExitPressed = g_GUIManager:DoButton("exit_button_player_dead","exit_button",l_ExitButtonPosition)	
	
	local l_RestartButtonPosition = SGUIPosition(0.65,0.5,0.2,0.05,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_RestartPressed = g_GUIManager:DoButton("restart_game_button_player_dead","restart_game_button",l_RestartButtonPosition)

	if(l_ExitPressed) then
		ExitGame()
	elseif(l_RestartPressed) then
		g_GameController:RestartLastCheckPoint()
	end
end