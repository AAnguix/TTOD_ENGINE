function StartLevelOne()
	
	dofile("./Data/Scripting/LuaMainInGame.lua")
	g_DamageCalculator:LoadXML("Data/damage_reduction_percentages.xml")
	
	g_GameController:LoadLevel("1")
	g_GameController:LoadLevelIndependentData()
	g_LuaGuiManager:SetGui(2)
	
	--g_CameraControllerManager:SetCurrentCameraController("DebugCamera")
	g_CameraControllerManager:SetCurrentCameraController("ThirdPersonCamera")
	g_SceneRendererCommandManager:Reload()
	
	local l_SoundEvent = SoundEvent("Play_WolfBlood")
	g_SoundManager:PlayEvent(l_SoundEvent)
end

function StartLevelTwo()
	g_GameController:Destroy()
	g_Engine:UnloadLevel("1")
	g_Engine:AddLevel("2")
	g_Engine:LoadLevel("2")

	g_GameController:LoadXML("Data/Level2/game_entities.xml")
	InitializeGUI()
	-- g_Engine:LoadLevel("2")
end

function StartLevelThree()
	g_GameController:Destroy()
	g_Engine:UnloadLevel("2")
	g_Engine:AddLevel("3")
	g_Engine:LoadLevel("3")

	g_GameController:LoadXML("Data/Level3/game_entities.xml")
	InitializeGUI()
end