class 'CLuaGuiOptions' (CLuaGui)
function CLuaGuiOptions:__init()
	CLuaGui.__init(self)
	self.m_Name = "OptionsGui"
end

function CLuaGuiOptions:Initialize()
	
end

function CLuaGuiOptions:ESC_PRESSED()
	g_LuaGuiManager:SetGui(1)
end

function CLuaGuiOptions:Update(ElapsedTime)
	
	
end



