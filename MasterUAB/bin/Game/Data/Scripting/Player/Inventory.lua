class 'CInventory'
function CInventory:__init()
	self.m_Slots={}
end

function CInventory:Update(ElapsedTime)
	for i=1, (#self.m_Slots) do
		self.m_Slots[i].m_Item:Update(ElapsedTime)
	end
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

function CInventory:DeleteItem(Index)
	self.m_Slots[Index] = nil
end

function CInventory:ClearInventory()
	for i=1, (#self.m_Slots) do
		self.m_Slots[i] = nil
	end
end

