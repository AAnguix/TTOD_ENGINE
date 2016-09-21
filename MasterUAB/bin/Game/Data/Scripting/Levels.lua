
-------------------- LEVEL ONE --------------------

function StartLevelOnee()
	local l_Level = "1"
	dofile("./Data/Scripting/LuaMainInGame.lua")
	g_DamageCalculator:LoadXML("Data/damage_reduction_percentages.xml")
	
	--g_GameController:LoadLevel(l_Level)
	g_Engine:AddLevel(l_Level,"StartLevelOneLua()")
	g_Engine:LoadLevel(l_Level)
end

--This is called from C++ when the level has been loaded by the engine
function StartLevelOneLua()
	local l_Level = "1"
	LuaMain() --Load new LUA behavior (per frame)
		g_GameController:LoadXML("Data/Level"..l_Level.."/game_entities.xml")
		g_AIManager:LoadLevel(l_Level)
		g_ItemManager:LoadItems(l_Level)
	
	g_GameController:LoadLevelIndependentData()
	g_LuaGuiManager:SetGui(2)
	
	g_CameraControllerManager:SetCurrentCameraController("ThirdPersonCamera")
	g_SceneRendererCommandManager:Reload()
	
	local l_SoundEvent = SoundEvent("Play_Resting")
	g_SoundManager:PlayEvent(l_SoundEvent)
	
	CreateLevelOneGameplayElements()
end

function CreateLevelOneGameplayElements()
	
	CreateNextLevelTrigger("LevelOneFinishedTrigger","LEVEL_ONE_FINISHED")
	
	local l_LuaGameObject = g_GameController:AddLuaGameObjectHandle("LightPedestalsManager")
	local l_StoneGateLuaGameObject = g_GameController:AddLuaGameObjectHandle("StoneGate")
	g_LightPedestalsManager  = CLightPedestalsManagerComponent(l_LuaGameObject,l_StoneGateLuaGameObject)
	g_ScriptManager:AddComponent("LightPedestalsManager",l_LuaGameObject,g_LightPedestalsManager)
	table.insert(g_GameController:GetEntities(),g_LightPedestalsManager)
	
	g_LightPedestalsManager:AddLight(g_LightManager:GetResource("FDirect001"),0.02)
	--...
	
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

-------------------- LEVEL TWO --------------------

function StartLevelTwo()

	local l_Level = "2"
	
	g_GameController:Destroy()
	g_EventManager:Destroy()
	g_Engine:UnloadLevel("1")
	g_Engine:AddLevel(l_Level,"StartLevelTwoLua()")
	g_Engine:LoadLevel(l_Level)
end

function StartLevelTwoLua()
	local l_Level = "2"
	g_GameController:LoadXML("Data/Level"..l_Level.."/game_entities.xml")
	--g_AIManager:LoadLevel(l_Level)
	--g_ItemManager:LoadItems(l_Level)
	
	g_GameController:LoadLevelIndependentData()
	--g_LuaGuiManager:SetGui(2)
	
	g_CameraControllerManager:SetCurrentCameraController("ThirdPersonCamera")
	g_SceneRendererCommandManager:Reload()
	
	--local l_SoundEvent = SoundEvent("Play_Resting")
	--g_SoundManager:PlayEvent(l_SoundEvent)
	
	--CreateLevelOneGameplayElements()
end

-------------------- LEVEL THREE --------------------

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

-------------------- LEVEL FOUR --------------------

function StartLevelFour()
	dofile("./Data/Scripting/Globals/GlobalsBossLevel.lua")
	dofile("./Data/Scripting/Elements/StatuesManager.lua")

	CreateMagicBall()
	--CreateStatues()
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

function GameFinished()

end

-------------------- COMMON --------------------

function CreateNextLevelTrigger(LevelFinishedTriggerLuaGameObjectName, LevelFinishedEventName) --LevelOneFinishedTrigger, LEVEL_ONE_FINISHED
	local l_NextLevelTriggerLuaGameObject = g_GameController:AddLuaGameObjectHandle(LevelFinishedTriggerLuaGameObjectName)
	local l_NextLevelTrigger = CTriggerComponent(l_NextLevelTriggerLuaGameObject,LevelFinishedEventName)
	g_ScriptManager:AddComponent(LevelFinishedTriggerLuaGameObjectName,l_NextLevelTriggerLuaGameObject,l_NextLevelTrigger)
	table.insert(g_GameController:GetEntities(),l_NextLevelTrigger)
end