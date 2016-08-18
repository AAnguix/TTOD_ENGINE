class 'CDynamiteGuiComponent' (CItem)
function CDynamiteGuiComponent:__init()
	CItem.__init(self, 1.0)
end

function CDynamiteGuiComponent:Use()
	-- if (CItem.Use(self)) then
		-- g_EventManager:FireEvent("DYNAMITE_USED")
		-- return true
	-- else 
		-- return false
	-- end
end