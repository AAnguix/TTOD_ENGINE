class 'CItem'
function CItem:__init(Cooldown)
	self.m_Cooldown = Cooldown
	self.m_ElapsedTime = 0.0
	self.m_Gui = EItemGui()
	self.m_Gui.m_HasText = false
	self.m_Gui.m_HasButton = false
	g_LogManager:Log("Item!")
end

function CItem:Use()
	if self:IsInCooldown() == false then
		self:ResetCooldown()
		return true
	else
		return false
	end
end

function CItem:AddText(GuiID,FontName,FontPath,GuiPosition,Sprite,Text)
	if self.m_Gui.m_HasText == false then
		self.m_Gui.m_Text = EItemText(GuiID,FontName,FontPath,GuiPosition,Sprite,Text)
		-- self.m_Gui.m_Text.m_GuiID = GuiID
		-- self.m_Gui.m_Text.m_FontName = FontName
		-- self.m_Gui.m_Text.m_FontPath = FontPath
		-- self.m_Gui.m_Text.m_Position = GuiPosition
		-- self.m_Gui.m_Text.m_Sprite = Sprite
		-- self.m_Gui.m_Text.m_Text = Text
		g_GUIManager:AddFont(FontName, FontPath)
		self.m_Gui.m_HasText = true
	end
end

function CItem:AddButton(GuiID,ButtonID, Normal,Highlight,Pressed,GuiPosition)
	if self.m_Gui.m_HasButton == false then
		self.m_Gui.m_Button = EItemButton(GuiID,ButtonID, Normal,Highlight,Pressed,GuiPosition)
		-- self.m_Gui.m_Button.m_GuiID = GuiID
		-- self.m_Gui.m_Button.m_ButtonID = ButtonID
		-- self.m_Gui.m_Button.m_Normal = Normal
		-- self.m_Gui.m_Button.m_Highlight = Highlight
		-- self.m_Gui.m_Button.m_Pressed = Pressed
		-- self.m_Gui.m_Button.m_GuiPosition = GuiPosition
       
		g_GUIManager:AddButton(ButtonID, Normal, Highlight, Pressed)
		self.m_Gui.m_HasButton = true
	end
end

function CItem:Update(ElapsedTime)
	self:UpdateCooldown(ElapsedTime)
	self:UpdateGUI(ElapsedTime)
end

function CItem:UpdateGUI(ElapsedTime)
	if self.m_Gui.m_HasButton then
		g_GUIManager:DoButton(self.m_Gui.m_Button.m_GuiID, self.m_Gui.m_Button.m_ButtonID, self.m_Gui.m_Button.m_GuiPosition)
	end
	if self.m_Gui.m_HasText then
		g_GUIManager:DoText(self.m_Gui.m_Text.m_GuiID, self.m_Gui.m_Text.m_FontName, self.m_Gui.m_Text.m_Position, self.m_Gui.m_Text.m_Sprite, "pollas enormes" ) --self.m_Gui.m_Text.m_Text
		--local l_T = "probando text"
		--l_T = g_GUIManager:DoTextBox(self.m_Gui.m_Text.m_GuiID, self.m_Gui.m_Text.m_FontName, self.m_Gui.m_Text.m_Position, self.m_Gui.m_Text.m_Sprite, l_T, 0.0)
	end
end

function CItem:UpdateCooldown(ElapsedTime)
	self.m_ElapsedTime = self.m_ElapsedTime + ElapsedTime
end

function CItem:ResetCooldown()
	self.m_Cooldown = 0.0
end

function CItem:IsInCooldown()
	if self.m_ElapsedTime <= self.m_Cooldown then
		return true
	else
		return false
	end
end

function CItem:GetGuiData()
	return self.m_Gui
end
