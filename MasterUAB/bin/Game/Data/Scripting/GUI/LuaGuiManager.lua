
dofile("./Data/Scripting/GUI/LuaGui.lua")
dofile("./Data/Scripting/GUI/LuaGuiStartScreen.lua")
dofile("./Data/Scripting/GUI/LuaGuiInGame.lua")
dofile("./Data/Scripting/GUI/LuaGuiCredits.lua")

class 'CLuaGuiManager'
function CLuaGuiManager:__init()
	
	self:SubscribeEvents()
	self.m_State = 1 --StartScreen, InGame, etc.
	self.m_GUIs = {}
	self.m_CurrentGui = nil
	
	self.m_AudioSubmenuActive = false
	self.m_MenuActive = false
	
	self.m_MinMusicVolume = 0.0
	self.m_MaxMusicVolume = 100.0
	self.m_CurrentMusicVolume = SSliderResult()
	self.m_CurrentMusicVolume.temp = 100.0

	self.m_MinFxVolume = 0.0
	self.m_MaxFxVolume = 100.0
	self.m_CurrentFxVolume = SSliderResult()
	self.m_CurrentFxVolume.temp = 100.0

	self.m_MUSIC_SLIDER_WIDTH = 0.35
	self.m_MUSIC_SLIDER_HEIGHT = 0.075
	self.m_MUSIC_SLIDER_XOFFSET = 0.5

	self.m_GRAPHICS_STATS_X_DISPLACEMENT = 0.97
	self.m_GRAPHICS_STATS_Y_DISPLACEMENT = 0.08
	self.m_GRAPHICS_STATS_Y_OFFSET = 0.025
	
	self.m_CREDITS_AUTHORS_XOFFSET = 0.4
	self.m_CREDITS_AUTHORS_YOFFSET = 0.2
	
	self.m_ShowGraphicsStats = true
	
	self:AddGui(CLuaGuiStartScreen())
	self:AddGui(CLuaGuiInGame())
	self:AddGui(CLuaGuiCredits())
end

--Manager subscribes events, and call apropiate method
--Prevents multiple responses to one callback
function CLuaGuiManager:ESC_PRESSED()
	self.m_CurrentGui:ESC_PRESSED()
	-- if (self.m_State==1) then
		-- ExitGame()
	-- elseif (self.m_State==2) then
		-- self.m_CurrentGui:CheckMenu()	
	-- elseif (self.m_State==3) then
		-- self:SetGui(1)
	-- end
end

function CLuaGuiManager:SubscribeEvents()
	g_EventManager:Subscribe(self, "ESC_PRESSED")
end

function CLuaGuiManager:AddGui(GUI)
	table.insert(self.m_GUIs,GUI)
end

function CLuaGuiManager:SetGui(Index)
	self.m_CurrentGui=self.m_GUIs[Index]
	self.m_State = Index
	g_LogManager:Log("Gui "..Index.." set")
	self.m_CurrentGui:Initialize()
end

-- function CLuaGuiManager:Initialize()
	-- self.m_CurrentGui:Initialize()
-- end
function CLuaGuiManager:Update(ElapsedTime)
	self.m_CurrentGui:Update(ElapsedTime)
	-- g_LogManager:Log("GUI: "..self.m_CurrentGui:GetName())
	self:ShowGraphicsStats()
	self:ShowProfilerStats()
end

function CLuaGuiManager:ShowGraphicsStats()
	if self.m_ShowGraphicsStats then
		local l_FPSPos = SGUIPosition(self.m_GRAPHICS_STATS_X_DISPLACEMENT,self.m_GRAPHICS_STATS_Y_DISPLACEMENT,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoText("FramesPerSecond","calibri_font",l_FPSPos,"","Fps:"..g_GraphicsStats:GetFps())
		
		local l_MillisecondsPerFrameTextPos = SGUIPosition(self.m_GRAPHICS_STATS_X_DISPLACEMENT,self.m_GRAPHICS_STATS_Y_DISPLACEMENT+self.m_GRAPHICS_STATS_Y_OFFSET,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoText("MillisecondsPerFrameText","calibri_font",l_MillisecondsPerFrameTextPos,"","Millisecs/frame")
		
		local l_MillisecondsPerFramePos = SGUIPosition(self.m_GRAPHICS_STATS_X_DISPLACEMENT,self.m_GRAPHICS_STATS_Y_DISPLACEMENT+(self.m_GRAPHICS_STATS_Y_OFFSET*2),0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoText("MillisecondsPerFrame","calibri_font",l_MillisecondsPerFramePos,"","Frame "..Round(g_GraphicsStats:GetMilliSecondsPerFrame(),2))
		
		local l_MinMillisecondsPerFramePos = SGUIPosition(self.m_GRAPHICS_STATS_X_DISPLACEMENT,self.m_GRAPHICS_STATS_Y_DISPLACEMENT+(self.m_GRAPHICS_STATS_Y_OFFSET*3),0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoText("MinMillisecondsPerFrame","calibri_font",l_MinMillisecondsPerFramePos,"","Min "..Round(g_GraphicsStats:GetMinMillisecondsPerFrame(),2))
		
		local l_AvgMillisecondsPerFramePos = SGUIPosition(self.m_GRAPHICS_STATS_X_DISPLACEMENT,self.m_GRAPHICS_STATS_Y_DISPLACEMENT+(self.m_GRAPHICS_STATS_Y_OFFSET*4),0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoText("AvgMillisecondsPerFrame","calibri_font",l_AvgMillisecondsPerFramePos,"","Avg "..Round(g_GraphicsStats:GetAvgMillisecondsPerFrame(),2))
		
		local l_MaxMillisecondsPerFramePos = SGUIPosition(self.m_GRAPHICS_STATS_X_DISPLACEMENT,self.m_GRAPHICS_STATS_Y_DISPLACEMENT+(self.m_GRAPHICS_STATS_Y_OFFSET*5),0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoText("MaxMillisecondsPerFrame","calibri_font",l_MaxMillisecondsPerFramePos,"","Max "..Round(g_GraphicsStats:GetMaxMillisecondsPerFrame(),2))
	end
end

function CLuaGuiManager:ShowProfilerStats()
	local l_TimeSinceStartPosition = SGUIPosition(self.m_GRAPHICS_STATS_X_DISPLACEMENT,self.m_GRAPHICS_STATS_Y_DISPLACEMENT+(self.m_GRAPHICS_STATS_Y_OFFSET*6),0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:DoText("TimeSinceStart","calibri_font",l_TimeSinceStartPosition,"","Time "..Round(g_Engine:GetRealTimeSinceStartup(),2))
end

function CLuaGuiManager:IsInStartScreen()	return m_State == 1 end
function CLuaGuiManager:IsInGame()	return m_State == 2 end
function CLuaGuiManager:IsInCredits()	return m_State == 3 end

