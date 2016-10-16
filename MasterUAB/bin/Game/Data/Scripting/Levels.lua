
-------------------- LEVEL ONE --------------------

function StartLevelOne()
	local l_Level = "1"
	dofile("./Data/Scripting/LuaMainInGame.lua")
	g_DamageCalculator:LoadXML("Data/damage_reduction_percentages.xml")
	
	local l_LevelOne = g_Engine:AddLevel(l_Level,"StartLevelOneLua()")
	g_Engine:LoadLevel(l_Level)
	
	g_SoundManager:SetPath("./Data/Level1/Audio/SoundBanks/")
end

--This is called from C++ when the level has been loaded by the engine
function StartLevelOneLua()
	local l_Level = "1"
	LuaMain() --Load new LUA behavior (per frame)
		g_GameController:LoadXML("Data/Level"..l_Level.."/game_entities.xml")
		g_AIManager:LoadLevel(l_Level)
	
	g_GameController:LoadLevelIndependentData()
	g_LuaGuiManager:SetGui(2)
	--g_LuaGuiManager:SetGui(6)
	
	g_CameraControllerManager:SetCurrentCameraController("ThirdPersonCamera")
	g_SceneRendererCommandManager:Reload()
	
	local l_SoundEvent = SoundEvent("Play_Resting")
	--g_SoundManager:PlayEvent(l_SoundEvent)
	
	CreateLevelOneGameplayElements()
end

function CreateLevelOneGameplayElements()
	CreateNextLevelTrigger("LevelOneFinishedTrigger","LEVEL_ONE_FINISHED")
	
	local l_LuaGameObject = g_GameController:AddLuaGameObjectHandle("LightPedestalsManager")
	local l_StoneGateLuaGameObject = g_GameController:AddLuaGameObjectHandle("StoneGate")
	g_LightPedestalsManager  = CLightPedestalsManagerComponent(l_LuaGameObject,l_StoneGateLuaGameObject)
	g_ScriptManager:AddComponent("LightPedestalsManager",l_LuaGameObject,g_LightPedestalsManager)
	table.insert(g_GameController:GetEntities(),g_LightPedestalsManager)
	
	g_LightPedestalsManager:AddLight(g_LightManager:GetResource("Omni002MOTOR"),0.5)
	g_LightPedestalsManager:AddLight(g_LightManager:GetResource("Omni005MOTOR"),1.5)
	g_LightPedestalsManager:AddLight(g_LightManager:GetResource("Omni008MOTOR"),1.5)
	g_LightPedestalsManager:AddLight(g_LightManager:GetResource("Omni009MOTOR"),1.5)
	g_LightPedestalsManager:AddLight(g_LightManager:GetResource("Fspot001MOTOR"),0.8)
	g_LightPedestalsManager:AddLight(g_LightManager:GetResource("Omni010MOTOR"),1.0)
	
	local l_LightPedestalOneLuaGameObject = g_GameController:AddLuaGameObjectHandle("LightPedestalOne") --LightPedestalOne
	local l_LightPedestalTwoLuaGameObject = g_GameController:AddLuaGameObjectHandle("LightPedestalTwo") 
	local l_LightPedestalThreeLuaGameObject = g_GameController:AddLuaGameObjectHandle("LightPedestalThree") 
	local l_LightPedestalFourLuaGameObject = g_GameController:AddLuaGameObjectHandle("LightPedestalFour") 
	
	local l_RuneOneLuaGameObject = g_GameController:AddLuaGameObjectHandle("RuneOne")
	local l_RuneTwoLuaGameObject = g_GameController:AddLuaGameObjectHandle("RuneTwo")
	local l_RuneThreeLuaGameObject = g_GameController:AddLuaGameObjectHandle("RuneThree")
	local l_RuneFourLuaGameObject = g_GameController:AddLuaGameObjectHandle("RuneFour")
	
	g_LightPedestalsManager:AddLightPedestal(l_LightPedestalOneLuaGameObject,"LIGHT_PEDESTAL_ONE_ACTIVATED",l_RuneOneLuaGameObject)
	g_LightPedestalsManager:AddLightPedestal(l_LightPedestalTwoLuaGameObject,"LIGHT_PEDESTAL_TWO_ACTIVATED",l_RuneTwoLuaGameObject)
	g_LightPedestalsManager:AddLightPedestal(l_LightPedestalThreeLuaGameObject,"LIGHT_PEDESTAL_THREE_ACTIVATED",l_RuneThreeLuaGameObject)
	g_LightPedestalsManager:AddLightPedestal(l_LightPedestalFourLuaGameObject,"LIGHT_PEDESTAL_FOUR_ACTIVATED",l_RuneFourLuaGameObject)
end

-------------------- LEVEL TWO --------------------

function StartLevelTwo()
	local l_Level = "2"
	g_GameController:Destroy()
	g_EventManager:Destroy()
	g_Engine:UnloadLevel("1")
	local l_LevelTwo = g_Engine:AddLevel(l_Level,"StartLevelTwoLua()")
	l_LevelTwo:AddSoundBankInfoFile("OrcsSoundbankInfo.xml")
	g_Engine:LoadLevel(l_Level)
end

function StartLevelTwoLua()
	local l_Level = "2"
	g_GameController:LoadXML("Data/Level"..l_Level.."/game_entities.xml")
	g_ItemManager:LoadLevelTwoItems()
	g_GameController:LoadLevelIndependentData()
	
	g_CameraControllerManager:SetCurrentCameraController("ThirdPersonCamera")
	g_SceneRendererCommandManager:Reload()
	
	CreateLevelTwoGameplayElements()
end

function CreateLevelTwoGameplayElements()
	CreateNextLevelTrigger("LevelTwoFinishedTrigger","LEVEL_TWO_FINISHED")
end

-------------------- LEVEL THREE --------------------

function StartLevelThree()
	local l_Level = "3"
	g_GameController:Destroy()
	g_EventManager:Destroy()
	g_Engine:UnloadLevel("2")
	local l_LevelThree = g_Engine:AddLevel(l_Level,"StartLevelThreeLua()")
	g_Engine:LoadLevel(l_Level)
end

function StartLevelThreeLua()
	local l_Level = "3"
	g_GameController:LoadXML("Data/Level"..l_Level.."/game_entities.xml")
	g_GameController:LoadLevelIndependentData()
	g_CameraControllerManager:SetCurrentCameraController("ThirdPersonCamera")
	g_SceneRendererCommandManager:Reload()
	CreateLevelThreeGameplayElements()
end

function CreateLevelThreeGameplayElements()
	--CreateNextLevelTrigger("LevelThreeFinishedTrigger","LEVEL_THREE_FINISHED")
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