class 'CItem'
function CItem:__init(Cooldown)
	self.m_Cooldown = Cooldown
	self.m_CurrentCooldown = 0.0
	self.m_Gui = EItemGui()
	self.m_Gui.m_HasText = false
	self.m_Gui.m_HasButton = false
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
		g_GUIManager:AddFont(FontName, FontPath)
		self.m_Gui.m_HasText = true
	end
end

function CItem:AddButton(GuiID,ButtonID, Normal,Highlight,Pressed,GuiPosition,Color)
	if self.m_Gui.m_HasButton == false then
		self.m_Gui.m_Button = EItemButton(GuiID,ButtonID, Normal,Highlight,Pressed,GuiPosition,Color)
		g_GUIManager:AddButton(ButtonID, Normal, Highlight, Pressed)
		self.m_Gui.m_HasButton = true
	end
end

function CItem:AddCooldownButton(GuiID,ButtonID, Normal,Highlight,Pressed,GuiPosition,Color)
	if self.m_Gui.m_HasButton == true then
		self.m_Gui.m_CooldownButton = EItemButton(GuiID,ButtonID, Normal,Highlight,Pressed,GuiPosition,Color)
		g_GUIManager:AddButton(ButtonID, Normal, Highlight, Pressed)
	end
end

function CItem:Update(ElapsedTime)
	self:UpdateCooldown(ElapsedTime)
	self:UpdateGUI(ElapsedTime)
end

function CItem:UpdateGUI(ElapsedTime)
	if self.m_Gui.m_HasButton then
		local l_Pressed = false
		if self:IsInCooldown() then
			g_GUIManager:DoButton(self.m_Gui.m_CooldownButton.m_GuiID, self.m_Gui.m_CooldownButton.m_ButtonID, self.m_Gui.m_CooldownButton.m_GuiPosition, self.m_Gui.m_CooldownButton.m_Color)
		else
			g_GUIManager:DoButton(self.m_Gui.m_Button.m_GuiID, self.m_Gui.m_Button.m_ButtonID, self.m_Gui.m_Button.m_GuiPosition, self.m_Gui.m_Button.m_Color)
		end
			
		-- if l_Pressed then 
			-- self:Use() 
		-- end
	end
	
	if (self.m_Gui.m_HasText and self.m_CurrentCooldown>0.00) then
		g_GUIManager:DoText(self.m_Gui.m_Text.m_GuiID,self.m_Gui.m_Text.m_FontName,self.m_Gui.m_Text.m_Position,"",Round(self.m_CurrentCooldown,0).."")
	end
end

function CItem:UpdateCooldown(ElapsedTime)
	if (self.m_CurrentCooldown>0.00) then
		self.m_CurrentCooldown = self.m_CurrentCooldown - ElapsedTime
	end
end

function CItem:ResetCooldown()
	self.m_CurrentCooldown = self.m_Cooldown
end

function CItem:IsInCooldown()
	if self.m_CurrentCooldown > 0 then
		return true
	else
		return false
	end
end

function CItem:GetGuiData()
	return self.m_Gui
end
