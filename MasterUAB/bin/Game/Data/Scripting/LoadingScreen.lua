g_MinPercentageLoaded = 0.0
g_MaxPercentageLoaded = 100.0
g_CurrentPercentageLoaded = SSliderResult()
g_CurrentPercentageLoaded.temp = 0.0

function UpdateGUI(ElapsedTime)
	local l_Pos = SGUIPosition(0.0, 0.0, 1.0, 1.0, CGUIManager.TOP_CENTER, CGUIManager.GUI_ABSOLUTE, CGUIManager.GUI_RELATIVE_WIDTH)
	--local l_BackgroundPos = SGUIPosition(0.0,0.0,1280.0,720.0)
	g_GUIManager:DoImage("background_image_0","background_image",l_Pos)
	
	local l_PercentageLoadedPos = SGUIPosition(0.5,0.2,0.35,0.075,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	local l_PercentageLoaded = g_Engine:GetCurrentLevel():GetPercentageLoaded()
	g_CurrentPercentageLoaded = g_GUIManager:DoSlider("level_percentage_loaded_slider_0","level_percentage_loaded_slider",l_PercentageLoadedPos,g_MinPercentageLoaded,g_MaxPercentageLoaded,l_PercentageLoaded)
end