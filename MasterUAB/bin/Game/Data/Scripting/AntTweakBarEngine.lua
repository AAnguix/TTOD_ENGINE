function ShowEngineParameters()
	ClickOnElement("Engine")
	g_DebugHelper:RegisterFloatParameter("Time scale", g_Engine:GetTimeScaleLuaAddress(),"min=0.05 max=1.0 step=0.001")
	g_DebugHelper:RegisterBoolParameter("Paused", g_Engine:GetPausedLuaAddress(),"")
end