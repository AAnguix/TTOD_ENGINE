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
	-- local l_SoundEvent = SoundEvent("Play_Music")
	-- g_SoundManager:PlayEvent(l_SoundEvent)
end

function Update(ElapsedTime)
	if g_Engine:IsPaused() == false then
		-- g_GameController:Update(ElapsedTime)
		g_EventManager:Update()
		--g_LogManager:Log("LuiEvents updated")
		g_LuaCameraController:Update(ElapsedTime)
		--UpdateCinematics(ElapsedTime)
	end
	g_LuaGuiManager:Update()
	--g_LogManager:Log("LuiGui updated")
end

function InputActionCallback(Action)
	if Action == 0 then 
		g_EventManager:FireEvent("PLAYER_ATTACKS")
	elseif Action == 1 then
		g_EventManager:FireEvent("PLAYER_BLOCKS")
	elseif Action == 2 then
		g_EventManager:FireEvent("PLAYER_DRINKS_BEER")
	elseif Action == 3 then
		g_EventManager:FireEvent("PLAYER_OPENS_MAP")
	elseif Action == 4 then
		g_EventManager:FireEvent("PLAYER_INTERACTS")
	
	elseif Action == 9 then
		g_EventManager:FireEvent("LOCK_CHARACTER")
	elseif Action == 10 then
		g_EventManager:FireEvent("BLOCK_CAMERA")
	elseif Action == 11 then
		g_EventManager:FireEvent("CENTER_CAMERA")
	elseif Action == 12 then
		g_EventManager:FireEvent("CHANGE_CAMERA")
	elseif Action == 13 then
		g_EventManager:FireEvent("ON_ALT8_PRESSED")
	elseif Action == 20 then 
		g_EventManager:FireEvent("ESC_PRESSED")
	
	elseif Action == 21 then 
		g_EventManager:FireEvent("ON_ALT1_PRESSED")
	elseif Action == 22 then 
		g_EventManager:FireEvent("ON_ALT2_PRESSED")
	elseif Action == 23 then 
		g_EventManager:FireEvent("ON_ALT3_PRESSED")
	elseif Action == 24 then 
		g_EventManager:FireEvent("ON_ALT4_PRESSED")
	elseif Action == 25 then 
		g_EventManager:FireEvent("ON_ALT5_PRESSED")
	elseif Action == 26 then 
		g_EventManager:FireEvent("ON_ALT6_PRESSED")
	elseif Action == 27 then 
		g_EventManager:FireEvent("ON_ALT7_PRESSED")
	elseif Action == 28 then 
		g_EventManager:FireEvent("ON_ALT8_PRESSED")
	elseif Action == 29 then 
		g_EventManager:FireEvent("ON_ALT9_PRESSED")
	elseif Action == 30 then 
		g_EventManager:FireEvent("RELOAD_LUA")
	end
	
end

function InputStateCallback(State)
	if State == 0 then
		g_EventManager:FireEvent("PLAYER_WALKING_FORWARD")
	elseif State == 1 then 
		g_EventManager:FireEvent("PLAYER_WALKING_LEFT")
	elseif State == 2 then 
		g_EventManager:FireEvent("PLAYER_WALKING_BACKWARDS")
	elseif State == 3 then 
		g_EventManager:FireEvent("PLAYER_WALKING_RIGHT")
	end
end

function InputRangesCallback(xAxis,yAxis)
	local l_CameraController = g_CameraControllerManager:GetCurrentCameraController()
	
	if l_CameraController:GetType() == CCameraController.THIRD_PERSON then
		l_CameraController:AddLookAtPitch(yAxis * g_Engine:GetElapsedTime())
		l_CameraController:AddYaw( xAxis * g_Engine:GetElapsedTime())
	end
	if l_CameraController:GetType() == CCameraController.DEBUG then
		l_CameraController:AddYaw(xAxis * ElapsedTime * 0.3)
		l_CameraController:AddPitch(yAxis * ElapsedTime * 0.3)
	end
end