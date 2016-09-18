function StartLevelOne()
	
	dofile("./Data/Scripting/LuaMainInGame.lua")
	g_DamageCalculator:LoadXML("Data/damage_reduction_percentages.xml")
	
	g_GameController:LoadLevel("1")
	g_GameController:LoadLevelIndependentData()
	g_LuaGuiManager:SetGui(2)
	
	--g_CameraControllerManager:SetCurrentCameraController("DebugCamera")
	g_CameraControllerManager:SetCurrentCameraController("ThirdPersonCamera")
	g_SceneRendererCommandManager:Reload()
	
	local l_SoundEvent = SoundEvent("Play_Resting")
	g_SoundManager:PlayEvent(l_SoundEvent)
	
	--StartBossLevel()
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
	
	local l_LuaGameObject = g_GameController:AddLuaGameObjectHandle("StoneGatePedestal")
	local l_StoneGatePedestal  = CStoneGatePedestalComponent(l_LuaGameObject,"STONE_GATE_PEDESTAL_ACTIVATED")
	g_ScriptManager:AddComponent("StoneGatePedestal",l_LuaGameObject,l_StoneGatePedestal)
	table.insert(g_GameController:GetEntities(),l_StoneGatePedestal)
	
	InitializeGUI()
end

function StartBossLevel()
	dofile("./Data/Scripting/Globals/GlobalsBossLevel.lua")
	dofile("./Data/Scripting/Elements/StatuesManager.lua")

	CreateMagicBall()
	--CreateStatues()
	CreateLightPedestals()
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
	g_StatuesManager:AddStatue(l_StatueOneLuaGameObject,5.0)
	g_StatuesManager:AddStatue(l_StatueTwoLuaGameObject,70.0)
	g_StatuesManager:AddStatue(l_StatueThreeLuaGameObject,80.0)
end

function CreateLightPedestals()
	local l_LuaGameObject = g_GameController:AddLuaGameObjectHandle("LightPedestalsManager")
	g_LightPedestalsManager  = CLightPedestalsManagerComponent(l_LuaGameObject)
	g_ScriptManager:AddComponent("LightPedestalsManager",l_LuaGameObject,g_LightPedestalsManager)
	table.insert(g_GameController:GetEntities(),g_LightPedestalsManager)
	
	local l_LightPedestalOneLuaGameObject = g_GameController:AddLuaGameObjectHandle("LightPedestalOne") --LightPedestalOne
	local l_LightPedestalTwoLuaGameObject = g_GameController:AddLuaGameObjectHandle("LightPedestalTwo") 
	local l_LightPedestalThreeLuaGameObject = g_GameController:AddLuaGameObjectHandle("LightPedestalThree")  
	local l_RuneOneLuaGameObject = g_GameController:AddLuaGameObjectHandle("RuneOne")
	local l_RuneTwoLuaGameObject = g_GameController:AddLuaGameObjectHandle("RuneTwo")
	local l_RuneThreeLuaGameObject = g_GameController:AddLuaGameObjectHandle("RuneThree")
	
	g_LightPedestalsManager:AddLightPedestal(l_LightPedestalOneLuaGameObject,"LIGHT_PEDESTAL_ONE_ACTIVATED",l_RuneOneLuaGameObject)
	g_LightPedestalsManager:AddLightPedestal(l_LightPedestalTwoLuaGameObject,"LIGHT_PEDESTAL_TWO_ACTIVATED",l_RuneTwoLuaGameObject)
	g_LightPedestalsManager:AddLightPedestal(l_LightPedestalThreeLuaGameObject,"LIGHT_PEDESTAL_THREE_ACTIVATED",l_RuneThreeLuaGameObject)
end