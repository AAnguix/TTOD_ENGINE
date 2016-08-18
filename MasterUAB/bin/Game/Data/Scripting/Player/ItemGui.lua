class 'EItemButton'
function EItemButton:__init(GuiID,ButtonID,Normal,Highlight,Pressed,Color)
	self.m_GuiID = GuiID
	self.m_ButtonID = ButtonID
	self.m_Normal = Normal
	self.m_Highlight = Highlight
	self.m_Pressed = Pressed
	self.m_Color = Color
end

class 'EItemText'
function EItemText:__init(GuiID,FontName,FontPath,Sprite,Text)
	self.m_GuiID = GuiID
	self.m_FontName = FontName
	self.m_FontPath = FontPath
	self.m_Sprite = Sprite
	self.m_Text = Text
end

class 'EItemGui'
function EItemGui:__init()
	self.m_Text = nil
	self.m_Button = nil
	self.m_CooldownButton = nil
	self.m_HasButton = false
	self.m_HasText = false
end