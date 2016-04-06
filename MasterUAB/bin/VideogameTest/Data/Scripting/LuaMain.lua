function LuaMain()
	DefineLUAModules()
	Initialize()
end

function DefineLUAModules()
	DefineFunctions()
	DefineClasses()
end

function DefineFunctions()
	dofile("./Data/Scripting/PlayerLUA.lua")
	dofile("./Data/Scripting/Globals.lua")
	dofile("./Data/Scripting/AIGlobals.lua")
	--dofile("./Data/Scripting/GUI.lua")
	dofile("./Data/Scripting/GUIStartScreen.lua")
	dofile("./Data/Scripting/CameraController.lua")
	dofile("./Data/Scripting/EnemyController.lua")
	dofile("./Data/Scripting/CharacterController.lua")
	dofile("./Data/Scripting/AI.lua")
	dofile("./Data/Scripting/Reload.lua")
	dofile("./Data/Scripting/AntTweakBar.lua")
	dofile("./Data/Scripting/PhysXTrigger.lua")
end

function DefineClasses()
	dofile("./Data/Scripting/Components.lua")
	dofile("./Data/Scripting/CCinematicsActionManager.lua")
	dofile("./Data/Scripting/CCinematicsActions.lua")
	dofile("./Data/Scripting/CinematicsManager.lua")
end

function Initialize()
	InitializeDebugBar()
	-- SetPlayerCharacterDefaultState()
	InitializeGUI()
	InitializeAI()
	InitializeRenderableObjectsScripts()
	CinematicsManager()
end

function Update(ElapsedTime)
	Reload(ElapsedTime)
	CameraController(ElapsedTime)
	UpdateGUI(ElapsedTime)
	UpdateCinematics(ElapsedTime)
end

