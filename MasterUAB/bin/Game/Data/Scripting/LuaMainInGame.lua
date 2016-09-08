function LuaMain()
	DefineFunctions()
	dofile("./Data/Scripting/GameplayScripts.lua")
	LoadGamePlayScripts()
	InitializeLuaMain()
	g_LogManager:Log("LuaMainInGame executed.")
end

function DefineFunctions()
	dofile("./Data/Scripting/Globals/GlobalsInGame.lua")
	dofile("./Data/Scripting/AntTweakBar.lua")
end

function InitializeLuaMain()
	InitializeDebugBar()
	g_CinematicActionManager:LoadXML("./Data/Scripting/Cinematic_tutorial.xml")
	-- local l_SoundEvent = SoundEvent("Play_Music")
	-- g_SoundManager:PlayEvent(l_SoundEvent)
end

function Update(ElapsedTime)
	if g_Engine:IsPaused() == false then
		-- g_GameController:Update(ElapsedTime)
		g_EventManager:Update()
		g_LuaCameraController:Update(ElapsedTime)
		UpdateCinematics(ElapsedTime)
	end
	g_LuaGuiManager:Update()
	--g_LogManager:Log("LuiGui updated")
end

dofile("./Data/Scripting/InputInGame.lua")