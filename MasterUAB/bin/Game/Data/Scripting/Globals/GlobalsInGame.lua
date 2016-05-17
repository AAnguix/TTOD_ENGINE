-- Used to define global variables and functions at start screen

-- g_PlayerSpeed=2.0
-- g_PlayerFastSpeed=3.0
-- g_PlayerStrafeSpeed=0.4
-- g_PlayerStrafeFastSpeed=0.8
g_CameraXAxisRotationSpeed=0.3
g_Gravity = Vect3f(0.0,-10.0,0.0)
g_WindMildEnter = false

g_Enemies = {}

g_CurrentGuiFile = "./Data/Scripting/GUI/GUIInGame.lua"
g_CurrentLuaMainFile = "./Data/Scripting/LuaMainInGame.lua"

g_ParticleSystemManager = g_Engine:GetParticleSystemManager()
gSoundManager = g_Engine:GetSoundManager()

g_IsGamePaused = false

function GetPlayer()
	player = CEngine.GetSingleton():GetLayerManager():GetPlayer()
	return player
end

