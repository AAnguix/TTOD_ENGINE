--The start point of LUA in the game
function LuaMain()
	DefineLUAModules()
	InitializeLuaMain()
	g_LogManager:Log("LuaMainStartScreen executed.")
end

function DefineLUAModules()
	--If you want start screen
	-- dofile("./Data/Scripting/Utils.lua")
	-- dofile("./Data/Scripting/Main/GameController.lua")
	-- dofile("./Data/Scripting/Globals/GlobalsStartScreen.lua")
	-- dofile("./Data/Scripting/GUI/GUIStartScreen.lua")
	-- dofile("./Data/Scripting/CameraController.lua")
	-- dofile("./Data/Scripting/Reload.lua")
	-- dofile("./Data/Scripting/AntTweakBar.lua")
	
	--If you dont want start screen
		dofile("./Data/Scripting/Main/GameController.lua")
		dofile("./Data/Scripting/Globals/GlobalsStartScreen.lua")
		dofile("./Data/Scripting/GUI/GUIStartScreen.lua")
		dofile("./Data/Scripting/CameraController.lua")
		dofile("./Data/Scripting/Reload.lua")
		dofile("./Data/Scripting/AntTweakBar.lua")
		
		dofile("./Data/Scripting/Globals/GlobalsInGame.lua")
		dofile("./Data/Scripting/Globals/AIGlobals.lua")
		dofile("./Data/Scripting/GUI/GUIInGame.lua")
		--dofile("./Data/Scripting/AI.lua")
		dofile("./Data/Scripting/Triggers/PhysXTrigger.lua")
		dofile("./Data/Scripting/Cinematics/Components.lua")
		dofile("./Data/Scripting/Cinematics/CCinematicsActionManager.lua")
		dofile("./Data/Scripting/Cinematics/CCinematicsActions.lua")
		dofile("./Data/Scripting/Cinematics/CinematicsManager.lua")
		
		dofile("./Data/Scripting/LuaMainInGame.lua")
		StartLevelOne()
end

function InitializeLuaMain()
	InitializeDebugBar()
	InitializeGUI()
end

function Update(ElapsedTime)
	Reload(ElapsedTime)
	CameraController(ElapsedTime)
	UpdateGUI(ElapsedTime)
end

