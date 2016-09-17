-- DRAW QUADS
class 'CCinematicsActionDrawSlide' (CCinematicsAction)
function CCinematicsActionDrawSlide:__init(XMLTreeNode)
	CCinematicsAction.__init(self, XMLTreeNode)
	
	self.m_Text = XMLTreeNode:GetPszProperty("text","",false)
	self.m_TextName = XMLTreeNode:GetPszProperty("text_name","",false)
	self.m_FontName = XMLTreeNode:GetPszProperty("font_name","",false)
	self.m_TextX = XMLTreeNode:GetFloatProperty("text_x",0.0,false)
	self.m_TextY = XMLTreeNode:GetFloatProperty("text_y",0.0,false)
	self.m_TextWidth = XMLTreeNode:GetFloatProperty("text_width",0.0,false)
	self.m_TextHeight = XMLTreeNode:GetFloatProperty("text_height",0.0,false)
	self.m_TextColor = CColor(XMLTreeNode:GetVect4fProperty("text_color",Vect4f(0.0,0.0,0.0,0.0),false))
	self.m_TextPos = SGUIPosition(self.m_TextX, self.m_TextY, self.m_TextWidth, self.m_TextHeight, CGUIManager.BOTTOM_CENTER, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE_WIDTH)
	
	self.m_GuiImageID =  XMLTreeNode:GetPszProperty("gui_image_id","",false)
	self.m_ImageID =  XMLTreeNode:GetPszProperty("image_id","",false)
	self.m_ImagePos = SGUIPosition(0.0, 0.0, 1.0, 1.0, CGUIManager.BOTTOM_LEFT, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE)
	self.m_Sprite =  XMLTreeNode:GetPszProperty("sprite","",false)
	self.m_Sound = XMLTreeNode:GetPszProperty("sound","",false)
	g_GUIManager:AddImage(self.m_ImageID,self.m_Sprite)
end

function CCinematicsActionDrawSlide:Execute()
	if self.m_Sound~="" then
		g_SoundManager:PlayEvent(SoundEvent(self.m_Sound))
	end
end

function CCinematicsActionDrawSlide:Update()
	g_GUIManager:DoText(self.m_TextName, self.m_FontName, self.m_TextPos, "", self.m_Text, self.m_TextColor)
	g_GUIManager:DoImage(self.m_GuiImageID, self.m_ImageID , self.m_ImagePos)
end

-- DRAW TEXT
class 'CCinematicsActionDrawText' (CCinematicsAction)
function CCinematicsActionDrawText:__init(XMLTreeNode)
	CCinematicsAction.__init(self, XMLTreeNode)
	
	self.m_Text = XMLTreeNode:GetPszProperty("text","",false)
	
	self.m_TextName = XMLTreeNode:GetPszProperty("text_name","",false)
	self.m_FontName = XMLTreeNode:GetPszProperty("font_name","",false)
	self.m_TextX = XMLTreeNode:GetFloatProperty("text_x",0.0,false)
	self.m_TextY = XMLTreeNode:GetFloatProperty("text_y",0.0,false)
	self.m_TextWidth = XMLTreeNode:GetFloatProperty("text_width",0.0,false)
	self.m_TextHeight = XMLTreeNode:GetFloatProperty("text_height",0.0,false)
	self.m_TextColor = CColor(XMLTreeNode:GetVect4fProperty("text_color",Vect4f(0.0,0.0,0.0,0.0),false))
	self.m_TextPos = SGUIPosition(self.m_TextX, self.m_TextY, self.m_TextWidth, self.m_TextHeight, CGUIManager.BOTTOM_CENTER, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE_WIDTH)
	self.m_Sound = XMLTreeNode:GetPszProperty("sound","",false)
end

function CCinematicsActionDrawText:Execute()
	if self.m_Sound~="" then
		g_SoundManager:PlayEvent(SoundEvent(self.m_Sound))
	end
end

function CCinematicsActionDrawText:Update()
	g_GUIManager:DoText(self.m_TextName, self.m_FontName, self.m_TextPos, "", self.m_Text, self.m_TextColor)
end







