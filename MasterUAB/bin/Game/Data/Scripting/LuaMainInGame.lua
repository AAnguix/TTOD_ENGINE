function LuaMain()
	DefineLUAModules()
	InitializeLuaMain()
	g_LogManager:Log("LuaMainInGame executed.")
end

dofile("./Data/Scripting/GameplayScripts.lua")

function DefineLUAModules()
	DefineFunctions()
	DefineClasses()
	LoadGamePlayScripts()
end

function DefineFunctions()
	dofile("./Data/Scripting/Globals/GlobalsInGame.lua")
	dofile("./Data/Scripting/Triggers/PhysXTrigger.lua")
	dofile("./Data/Scripting/Globals/AIGlobals.lua")
	dofile("./Data/Scripting/GUI/GUIInGame.lua")
	dofile("./Data/Scripting/CameraControllers/CameraControllerFunctions.lua")
	dofile("./Data/Scripting/CameraControllers/DebugCameraController.lua")
	dofile("./Data/Scripting/CameraControllers/ThirdPersonCameraController.lua")
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
	g_DamageCalculator:LoadXML("Data/damage_reduction_percentages.xml")
	InitializeDebugBar()
	-- SetPlayerCharacterDefaultState()
	InitializeGUI()
	--InitializeAI()
	--InitializeRenderableObjectsScripts()
	CinematicsManager()
	-- local l_SoundEvent = SoundEvent("Play_Music")
	-- g_SoundManager:PlayEvent(l_SoundEvent)
end

function Update(ElapsedTime)
	if g_Engine:IsPaused() == false then
		-- g_GameController:Update(ElapsedTime)
		g_EventManager:Update()
		Reload(ElapsedTime)
		UpdateCameraController(ElapsedTime)
		UpdateCinematics(ElapsedTime)
	end
	UpdateGUI(ElapsedTime)
end
