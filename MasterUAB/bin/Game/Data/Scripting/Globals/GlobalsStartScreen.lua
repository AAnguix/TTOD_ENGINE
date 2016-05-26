-- Used to define global variables and functions at start screen
g_CameraXAxisRotationSpeed=0.3

g_CurrentLevel = 0

g_PlayerComponent = nil --Store the instance to the player component
g_Player = nil -- ... player RObject

g_CurrentGuiFile = "./Data/Scripting/GUI/GUIStartScreen.lua"
g_CurrentLuaMainFile = "./Data/Scripting/LuaMainStartScreen.lua"

g_Engine = CEngine.GetSingleton()
g_RenderManager = g_Engine:GetRenderManager()
g_LayerManager = g_Engine:GetLayerManager()
g_PhysXManager = g_Engine:GetPhysXManager()
g_LogManager = g_Engine:GetLogManager()
g_CameraControllerManager = g_Engine:GetCameraControllerManager()
g_GUIManager = g_Engine:GetGUIManager()
g_SceneRendererCommandManager = g_Engine:GetSceneRendererCommandManager()
g_SoundManager = g_Engine:GetSoundManager()
g_TextureManager = g_Engine:GetTextureManager()
g_DebugHelper = g_Engine:GetDebugHelper()
g_InputManager = g_Engine:GetInputManager()
g_AnimatedModelManager = g_Engine:GetAnimatedModelManager()

g_GameController = CGameController()
g_DamageCalculator = CDamageCalculator()