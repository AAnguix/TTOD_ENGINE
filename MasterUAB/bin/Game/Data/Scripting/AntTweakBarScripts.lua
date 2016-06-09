function OpenScripts()
	ClickOnElement("Scripts")
	local l_Scripts = g_GameController:GetScripts()
	for i=1, (#l_Scripts) do
		g_DebugHelper:RegisterBoolParameter(l_Scripts[i]:GetName(),l_Scripts[i]:GetEnabledLuaAddress(),"")
		--g_DebugHelper:RegisterExtendedButton("Script "..i,"OpenScript",g_GameController:GetScripts()[i]:GetThisLuaAddress(),"lua_component")
	end
end

-- function OpenScript(CLuaComponent)
	-- if CLuaComponent:IsEnabled() then
		-- CLuaComponent:Disable()
	-- else 
		-- CLuaComponent:Enable()
	-- end
-- end