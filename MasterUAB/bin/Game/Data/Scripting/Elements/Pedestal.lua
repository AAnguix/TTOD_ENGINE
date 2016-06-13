class 'CPedestalComponent' (CLUAComponent)
function CPedestalComponent:__init(CGameObject)
CLUAComponent.__init(self,"pedestal")	
	self.m_GameObject = CGameObject
	self.m_Active = false
	self.m_InteractionRange = 1.5
	g_GUIManager:AddButton("pedestal01_button", "pedestal_button_normal", "pedestal_button_highlight", "pedestal_button_pressed")
	self.m_TimeActive = 4.0
	self.m_SoundPlayed = false
end

function CPedestalComponent:Initialize(XMLTreeNode)
	
	-- local GuiID = XMLTreeNode:GetPszProperty("gui_id", "", false)
	-- local ButtonID = XMLTreeNode:GetPszProperty("button_id", "", false)
	-- local Normal = XMLTreeNode:GetPszProperty("normal", "", false)
	-- local Highlight = XMLTreeNode:GetPszProperty("highlight", "", false)
	-- local Pressed = XMLTreeNode:GetPszProperty("pressed", "", false)
	-- self.m_Function = XMLTreeNode:GetPszProperty("function", "", false)
	--local Center = SGUIPosition(0.4,0.4,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	
	--self.m_Button = EItemButton(GuiID,ButtonID,Normal,Highlight,Pressed,GuiPosition)
	
	--g_GUIManager:AddButton(ButtonID, Normal, Highlight, Pressed)
	
	--g_LogManager:Log("Pedestal "..self.m_GameObject:GetName().." created...")
end

function CPedestalComponent:GetInteractionRange()
	return self.m_InteractionRange
end

function CPedestalComponent:IsPlayerInsideInteractionRange(PlayerPosition)

	local l_Distance = 0.0
	local l_IsPlayerInsideInteractionRange = false
	local l_Position = self.m_GameObject:GetRenderableObject():GetPosition()
	
	local l_X=(l_Position.x - PlayerPosition.x)^2
	local l_Y = (l_Position.y - PlayerPosition.y)^2
	local l_Z = (l_Position.z - PlayerPosition.z)^2
	l_Distance = math.sqrt( l_X + l_Y + l_Z)
	-- g_LogManager:Log("Player")
	-- g_LogManager:Log(PlayerPosition)
	-- g_LogManager:Log("Pedestal")
	-- g_LogManager:Log(l_Position)
	if l_Distance < self.m_InteractionRange then
		l_IsPlayerInsideInteractionRange = true
	end
	
	return l_IsPlayerInsideInteractionRange
end

function CPedestalComponent:Update(ElapsedTime)
	if self.m_Active == true then
		self:AddTime()
		if(self:GetTimer() <= self.m_TimeActive) then
			if self.m_SoundPlayed == false then
				self.m_GameObject:GetAudioSource():PlayEvent("pedestal_sound")
				self.m_SoundPlayed = true
			end
			--Show GUI message telling player that he has reach and objetive
		else
			self.m_Active = false
			self:ResetTimer()
			self:Disable()
		end
	elseif self:IsPlayerInsideInteractionRange(g_Player:GetRenderableObject():GetPosition()) then
		self:ShowGuiMessage()
	end
end

function CPedestalComponent:ShowGuiMessage()
	
	local l_Center = SGUIPosition(0.4,0.4,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_Pressed = g_GUIManager:DoButton("pedestal01_button","pedestal01_button", l_Center)
	--local l_Pressed = g_GUIManager:DoButton(self.m_Button.m_GuiID, self.m_Button.m_ButtonID, l_Center)
	if l_Pressed then
		self.m_Active = true
	end
end

function CPedestalComponent:IsActive()
	return self.m_Active
end
