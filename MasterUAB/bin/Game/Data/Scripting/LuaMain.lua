--The start point of LUA in the game
function LuaMain()
	DefineLUAModules()
	InitializeLuaMain()
	g_LogManager:Log("LuaMainStartScreen executed.")
end

dofile("./Data/Scripting/CinematicsScripts.lua")

function DefineLUAModules()
	--If you want start screen
	dofile("./Data/Scripting/Utils.lua")
	dofile("./Data/Scripting/Main/EventManager.lua")
	dofile("./Data/Scripting/Main/GameController.lua")
	dofile("./Data/Scripting/Main/AIManager.lua")
	LoadCinematicsScripts()
	dofile("./Data/Scripting/Globals/GlobalsStartScreen.lua")
	--dofile("./Data/Scripting/GUI/GUIStartScreen.lua")
	dofile("./Data/Scripting/CameraControllers/FixedCameraController.lua")
	dofile("./Data/Scripting/AntTweakBar.lua")
	dofile("./Data/Scripting/Levels.lua")
	--If you dont want start screen
		-- dofile("./Data/Scripting/Main/GameController.lua")
		-- dofile("./Data/Scripting/Globals/GlobalsStartScreen.lua")
		-- dofile("./Data/Scripting/GUI/GUIStartScreen.lua")
		-- dofile("./Data/Scripting/CameraControllers/DebugCameraController.lua")
		-- dofile("./Data/Scripting/Reload.lua")
		-- dofile("./Data/Scripting/AntTweakBar.lua")
		
		-- dofile("./Data/Scripting/Globals/GlobalsInGame.lua")
		-- dofile("./Data/Scripting/Globals/AIGlobals.lua")
		-- dofile("./Data/Scripting/GUI/GUIInGame.lua")
		-- --dofile("./Data/Scripting/AI.lua")
		-- dofile("./Data/Scripting/Triggers/PhysXTrigger.lua")
		
		-- dofile("./Data/Scripting/LuaMainInGame.lua")
		-- StartLevelOne()
end

function InitializeLuaMain()
	InitializeDebugBar()
	g_LuaGuiManager:SetGui(1)
	g_CinematicActionManager:LoadXML("./Data/Scripting/Cinematics_actions.xml")
end

function Update(ElapsedTime)
	g_LuaGuiManager:Update()
	g_EventManager:Update()
	--UpdateCinematics(ElapsedTime)
end

--Prevent input handling in the start screen
function InputActionCallback(Action)
	if Action == 20 then 
		g_EventManager:FireEvent("ESC_PRESSED")
	end
end

function InputStateCallback(State)
end

function InputRangesCallback(xAxis,yAxis)
end

function ExitGame()
	CEngine.TerminateApplication()
end
