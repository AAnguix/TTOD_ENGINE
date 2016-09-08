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
	
	StartBossLevel()
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

function StartBossLevel()
	dofile("./Data/Scripting/Globals/GlobalsBossLevel.lua")
	dofile("./Data/Scripting/Elements/StatuesManager.lua")

	CreateMagicBall()
	CreateStatues()
end

function CreateMagicBall()
	local l_MagicBallLuaGameObject = g_GameController:AddLuaGameObjectHandle("MagicBall")
	g_MagicBall=CMagicBallComponent(l_MagicBallLuaGameObject)
	g_ScriptManager:AddComponent(l_MagicBallLuaGameObject:GetName(),l_MagicBallLuaGameObject,g_MagicBall)
	table.insert(g_GameController:GetEntities(),g_MagicBall)
end

function CreateStatues()
	local l_LuaGameObject = g_GameController:AddLuaGameObjectHandle("StatuesManager")
	g_StatuesManager  = CStatuesManagerComponent(l_LuaGameObject)
	g_ScriptManager:AddComponent("StatuesManager",l_LuaGameObject,g_StatuesManager)
	table.insert(g_GameController:GetEntities(),g_StatuesManager)
	
	local l_StatueOneLuaGameObject = g_GameController:AddLuaGameObjectHandle("StatueOne")
	local l_StatueTwoLuaGameObject = g_GameController:AddLuaGameObjectHandle("StatueTwo")
	local l_StatueThreeLuaGameObject = g_GameController:AddLuaGameObjectHandle("StatueThree")
	g_StatuesManager:AddStatue(l_StatueOneLuaGameObject,10.0)
	g_StatuesManager:AddStatue(l_StatueTwoLuaGameObject,10.0)
	g_StatuesManager:AddStatue(l_StatueThreeLuaGameObject,10.0)
end