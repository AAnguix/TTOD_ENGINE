function Action0()	g_EventManager:FireEvent("PLAYER_ATTACKS") end
function Action1()	g_EventManager:FireEvent("PLAYER_BLOCKS") end
function Action2()	g_EventManager:FireEvent("PLAYER_DRINKS_BEER") end
function Action3()	g_EventManager:FireEvent("PLAYER_OPENS_MAP") end
function Action4()	g_EventManager:FireEvent("PLAYER_INTERACTS") end

function Action5() end
function Action6() end
function Action7() end
function Action8() end

function Action9()	g_EventManager:FireEvent("LOCK_CHARACTER") end
function Action10()	g_EventManager:FireEvent("BLOCK_CAMERA") end
function Action11()	g_EventManager:FireEvent("CENTER_CAMERA") end
function Action12()	g_EventManager:FireEvent("CHANGE_CAMERA") end

function Action13() end
function Action14() end
function Action15() end
function Action16() end
function Action17() end
function Action18() end
function Action19() end

function Action20()	g_EventManager:FireEvent("ESC_PRESSED") end

function Action21()	g_EventManager:FireEvent("ON_ALT1_PRESSED") end
function Action22()	g_EventManager:FireEvent("ON_ALT2_PRESSED") end
function Action23()	g_EventManager:FireEvent("ON_ALT3_PRESSED") end
function Action24()	g_EventManager:FireEvent("ON_ALT4_PRESSED") end
function Action25()	g_EventManager:FireEvent("ON_ALT5_PRESSED") end
function Action26()	g_EventManager:FireEvent("ON_ALT6_PRESSED") end
function Action27()	g_EventManager:FireEvent("ON_ALT7_PRESSED") end
function Action28()	g_EventManager:FireEvent("ON_ALT8_PRESSED") end
function Action29()	g_EventManager:FireEvent("ON_ALT9_PRESSED") end
function Action30()	g_EventManager:FireEvent("RELOAD_LUA") end

function State0() g_EventManager:FireEvent("PLAYER_WALKING_FORWARD") 

local l_Pos = SGUIPosition(0.5,0.4,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
g_GUIManager:DoText("W","freestyle_script_64_font",l_Pos,"","W")
		
end
function State1() g_EventManager:FireEvent("PLAYER_WALKING_LEFT") 
local l_Pos = SGUIPosition(0.53,0.4,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
g_GUIManager:DoText("A","freestyle_script_64_font",l_Pos,"","A")
end
function State2() g_EventManager:FireEvent("PLAYER_WALKING_BACKWARDS") 
local l_Pos = SGUIPosition(0.56,0.4,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
g_GUIManager:DoText("S","freestyle_script_64_font",l_Pos,"","S")
end
function State3() g_EventManager:FireEvent("PLAYER_WALKING_RIGHT") 
local l_Pos = SGUIPosition(0.59,0.4,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
g_GUIManager:DoText("D","freestyle_script_64_font",l_Pos,"","D")
end

function InputRangesCallback(xAxis,yAxis)
	local l_CameraController = g_CameraControllerManager:GetCurrentCameraController()
	
	if l_CameraController:GetType() == CCameraController.THIRD_PERSON then
		l_CameraController:AddLookAtPitch(yAxis * g_Engine:GetElapsedTime())
		l_CameraController:AddYaw( xAxis * g_Engine:GetElapsedTime())
	end
	if l_CameraController:GetType() == CCameraController.DEBUG then
		l_CameraController:AddYaw(xAxis * g_Engine:GetElapsedTime() * 0.5)
		l_CameraController:AddPitch(yAxis * g_Engine:GetElapsedTime() * 0.5)
	end
end