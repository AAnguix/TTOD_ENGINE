-- Used to define global variables and functions at start screen

g_PlayerSpeed=2.0
g_PlayerFastSpeed=3.0
g_PlayerStrafeSpeed=0.4
g_PlayerStrafeFastSpeed=0.8
g_CameraXAxisRotationSpeed=0.3
g_Gravity = Vect3f(0.0,-10.0,0.0)
g_WindMildEnter = false

g_GuiFile = "./Data/Scripting/GUIInGame.lua"

--g_Engine = CEngine.GetSingleton()
--g_LayerManager = g_Engine:GetLayerManager()
--g_PhysXManager = g_Engine:GetPhysXManager()
--g_LogManager = g_Engine:GetLogManager()
--g_CameraControllerManager = g_Engine:GetCameraControllerManager()
--g_GUIManager = g_Engine:GetGUIManager()
g_ParticleSystemManager = g_Engine:GetParticleSystemManager()
gSoundManager = g_Engine:GetSoundManager()

function GetPlayer()
	player = CEngine.GetSingleton():GetLayerManager():GetPlayer()
	return player
end

