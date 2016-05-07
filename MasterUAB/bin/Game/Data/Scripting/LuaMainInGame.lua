function LuaMain()
	DefineLUAModules()
	InitializeLuaMain()
end

function DefineLUAModules()
	DefineFunctions()
	DefineClasses()
end

function DefineFunctions()
	dofile("./Data/Scripting/Globals/GlobalsInGame.lua")
	dofile("./Data/Scripting/Globals/AIGlobals.lua")
	dofile("./Data/Scripting/GUI/GUIInGame.lua")
	dofile("./Data/Scripting/CameraController.lua")
	dofile("./Data/Scripting/CharacterController.lua")
	--dofile("./Data/Scripting/AI.lua")
	dofile("./Data/Scripting/Reload.lua")
	dofile("./Data/Scripting/AntTweakBar.lua")
	dofile("./Data/Scripting/Triggers/PhysXTrigger.lua")
end

function DefineClasses()
	dofile("./Data/Scripting/Cinematics/Components.lua")
	dofile("./Data/Scripting/Cinematics/CCinematicsActionManager.lua")
	dofile("./Data/Scripting/Cinematics/CCinematicsActions.lua")
	dofile("./Data/Scripting/Cinematics/CinematicsManager.lua")
end

function InitializeLuaMain()
	InitializeDebugBar()
	-- SetPlayerCharacterDefaultState()
	InitializeGUI()
	--InitializeAI()
	InitializeRenderableObjectsScripts()
	CinematicsManager()
end

function Update(ElapsedTime)
	g_GameController:Update(ElapsedTime)
	Reload(ElapsedTime)
	CameraController(ElapsedTime)
	UpdateGUI(ElapsedTime)
	UpdateCinematics(ElapsedTime)
end

