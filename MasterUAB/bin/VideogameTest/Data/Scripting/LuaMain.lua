function LuaMain()
	DefineLUAModules()
	Main()
end

function DefineLUAModules()
	DefineFunctions()
	DefineClasses()
end

function DefineFunctions()
	dofile("./Data/Scripting/Globals.lua")
	dofile("./Data/Scripting/AIGlobals.lua")
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

function Main()
	InitializeDebugBar()
	-- SetPlayerCharacterDefaultState()
	InitializeAI()
	InitializeRenderableObjectsScripts()
	CinematicsManager()
end


