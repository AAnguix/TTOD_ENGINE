function LuaMain()
	DefineLUAModules()
	InitializeLuaMain()
	g_LogManager:Log("LuaMainInGame executed.")
end

function DefineLUAModules()
	DefineFunctions()
	DefineClasses()
	LoadGamePlayScripts()
end

function DefineFunctions()
	dofile("./Data/Scripting/Globals/GlobalsInGame.lua")
	dofile("./Data/Scripting/Globals/AIGlobals.lua")
	dofile("./Data/Scripting/GUI/GUIInGame.lua")
	dofile("./Data/Scripting/CameraController.lua")
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
	if CApplication.IsGamePaused() == false then
		g_GameController:Update(ElapsedTime)
		Reload(ElapsedTime)
		CameraController(ElapsedTime)
		UpdateCinematics(ElapsedTime)
	end
	UpdateGUI(ElapsedTime)
end

function LoadGamePlayScripts()
	dofile("./Data/Scripting/Player/PlayerFMS.lua") --To fix OnIdleEnter bug
	dofile("./Data/Scripting/Player/Player.lua")
	dofile("./Data/Scripting/Weapons/Armor.lua")
	dofile("./Data/Scripting/Weapons/Weapon.lua")
	dofile("./Data/Scripting/Enemies/Enemy.lua")
	dofile("./Data/Scripting/Enemies/BasicEnemy.lua")
	dofile("./Data/Scripting/Enemies/BasicEnemyFSM.lua")
	dofile("./Data/Scripting/Enemies/RangedEnemy.lua")
	dofile("./Data/Scripting/Enemies/RangedEnemyFSM.lua")
	dofile("./Data/Scripting/Enemies/BruteEnemy.lua")
	dofile("./Data/Scripting/Enemies/BruteEnemyFSM.lua")
	dofile("./Data/Scripting/Enemies/Dragon.lua")
	dofile("./Data/Scripting/Enemies/DragonFSM.lua")
	dofile("./Data/Scripting/Elements/Pedestal.lua")
	g_LogManager:Log("Gameplayscripts loaded.")
end

