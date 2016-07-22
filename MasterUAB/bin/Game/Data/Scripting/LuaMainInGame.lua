function LuaMain()
	DefineLUAModules()
	InitializeLuaMain()
	g_LogManager:Log("LuaMainInGame executed.")
end

dofile("./Data/Scripting/GameplayScripts.lua")

function DefineLUAModules()
	DefineFunctions()
	LoadGamePlayScripts()
end

function DefineFunctions()
	dofile("./Data/Scripting/Globals/GlobalsInGame.lua")
	dofile("./Data/Scripting/GUI/GUIInGame.lua")
	dofile("./Data/Scripting/CameraControllers/CameraControllerFunctions.lua")
	dofile("./Data/Scripting/CameraControllers/DebugCameraController.lua")
	dofile("./Data/Scripting/CameraControllers/ThirdPersonCameraController.lua")
	dofile("./Data/Scripting/Reload.lua")
	dofile("./Data/Scripting/AntTweakBar.lua")
end

function InitializeLuaMain()
	InitializeDebugBar()
	-- SetPlayerCharacterDefaultState()
	InitializeGUI()
	--InitializeAI()
	--InitializeRenderableObjectsScripts()
	-- local l_SoundEvent = SoundEvent("Play_Music")
	-- g_SoundManager:PlayEvent(l_SoundEvent)
end

function Update(ElapsedTime)
	if g_Engine:IsPaused() == false then
		-- g_GameController:Update(ElapsedTime)
		g_EventManager:Update()
		Reload(ElapsedTime)
		UpdateCameraController(ElapsedTime)
		--UpdateCinematics(ElapsedTime)
	end
	UpdateGUI(ElapsedTime)
end
