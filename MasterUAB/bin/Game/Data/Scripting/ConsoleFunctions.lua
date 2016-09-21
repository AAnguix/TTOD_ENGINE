function Console(ElapsedTime)
	local l_Pos = SGUIPosition(0.2,0.85,0.25,0.05,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:CreateConsole("Console", "freestyle_script_48_font", l_Pos, "console", ElapsedTime)
end

function ReloadLua()
	g_AntTweakbarReloader:ReloadLua()
end

function RP()
	g_LightPedestalsManager:Reset()
end

