-- Used to define global variables and functions at start screen
g_CameraXAxisRotationSpeed=0.3

g_GuiFile = "./Data/Scripting/GUIStartScreen.lua"

g_Engine = CEngine.GetSingleton()
g_LayerManager = g_Engine:GetLayerManager()
g_PhysXManager = g_Engine:GetPhysXManager()
g_LogManager = g_Engine:GetLogManager()
g_CameraControllerManager = g_Engine:GetCameraControllerManager()
g_GUIManager = g_Engine:GetGUIManager()
g_SceneRendererCommandManager = g_Engine:GetSceneRendererCommandManager()