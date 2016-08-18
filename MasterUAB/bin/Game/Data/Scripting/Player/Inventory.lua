class 'CInventory'
function CInventory:__init()
	self.m_Slots={}
	
	self.m_ButtonsStartX = 0.95
	self.m_ButtonsStartY = 0.95
	
	self.m_ButtonXOffset = 0.0
	self.m_ButtonYOffset = 0.15
	
	self.m_TextsStartX = 0.968
	self.m_TextsStartY = 0.785
	
	self.m_TextXOffset = 0.0
	self.m_TextYOffset = 0.10
	
	self.m_ElementWidth = 0.08
	self.m_ElementHeight = 0.07
	
	self.m_ButtonsAnchor = CGUIManager.TOP_CENTER
	self.m_ButtonsAnchorCoordsType = CGUIManager.GUI_RELATIVE
	self.m_ButtonsSizeCoordsTyp = CGUIManager.GUI_RELATIVE_WIDTH
	
	self.m_TextsAnchor = CGUIManager.TOP_CENTER
	self.m_TextsAnchorCoordsType = CGUIManager.GUI_RELATIVE
	self.m_TextsSizeCoordsType = CGUIManager.GUI_RELATIVE_WIDTH

	self.m_ButtonPosition = nil
	self.m_TextPosition = nil
end

function CInventory:GetButtonPosition() return self.m_ButtonPosition end
function CInventory:GetTextPosition() return self.m_TextPosition end

function CInventory:Update(ElapsedTime)
	for i=1, (#self.m_Slots) do
		self:CalculateNextButtonPosition(i)
		self:CalculateNextTextPosition(i)
		self.m_Slots[i].m_Item:Update(ElapsedTime)
	end
end

function CInventory:CalculateNextButtonPosition(Index)
	self.m_ButtonPosition = SGUIPosition(self.m_ButtonsStartX, self.m_ButtonsStartY - (self.m_ButtonYOffset*Index), self.m_ElementWidth, self.m_ElementHeight, CGUIManager.TOP_CENTER, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE_WIDTH)
end
function CInventory:CalculateNextTextPosition(Index)
	self.m_TextPosition = SGUIPosition(self.m_TextsStartX, self.m_TextsStartY - (self.m_TextYOffset*Index), self.m_ElementWidth, self.m_ElementHeight, CGUIManager.TOP_CENTER, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE_WIDTH)
end

function CInventory:AddItem(CItem,Units)
	table.insert(self.m_Slots,CSlot(CItem,Units))
end

function CInventory:UseItem(Index)
	if self.m_Slots[Index].m_Item:Use() then
		local l_Units = self.m_Slots[Index].m_Units
		self.m_Slots[Index].m_Units = l_Units - 1
	end
end

function CInventory:RemoveItem(Index)
	self.m_Slots[Index] = nil
end

function CInventory:ClearInventory()
	for i=1, (#self.m_Slots) do
		self.m_Slots[i] = nil
	end
end

