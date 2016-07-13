-- Used to define global variables and functions at start screen
g_CameraXAxisRotationSpeed=0.3

--g_CurrentLevel = 0

g_PlayerComponent = nil --Store the instance to the player component
g_Player = nil -- ... player RObject

g_CurrentGuiFile = "./Data/Scripting/GUI/GUIStartScreen.lua"
g_CurrentLuaMainFile = "./Data/Scripting/LuaMainStartScreen.lua"

g_Engine = CEngine.GetSingleton()
g_RenderManager = g_Engine:GetRenderManager()
g_ContextManager = g_RenderManager:GetContextManager()
g_LuabindManager = g_Engine:GetLuabindManager()

g_GameObjectManager = g_Engine:GetGameObjectManager()
g_LuaGameObjectHandleManager = g_Engine:GetLuaGameObjectHandleManager()
g_StaticMeshManager = g_Engine:GetStaticMeshManager()
g_AnimatedModelManager = g_Engine:GetAnimatedModelManager()
g_TextureManager = g_Engine:GetTextureManager()
g_EffectManager = g_Engine:GetEffectManager()
g_ParticleSystemManager = g_Engine:GetParticleSystemManager()
g_LightManager = g_Engine:GetLightManager()
g_MaterialManager = g_Engine:GetMaterialManager()

g_LayerManager = g_Engine:GetLayerManager()
g_PhysXManager = g_Engine:GetPhysXManager()
g_CameraControllerManager = g_Engine:GetCameraControllerManager()
g_GUIManager = g_Engine:GetGUIManager()
g_SoundManager = g_Engine:GetSoundManager()
g_AnimatorControllerManager = g_Engine:GetAnimatorControllerManager()
g_ScriptManager = g_Engine:GetScriptManager()


g_SceneRendererCommandManager = g_Engine:GetSceneRendererCommandManager()
g_DebugHelper = g_Engine:GetDebugHelper()
g_InputManager = g_Engine:GetInputManager()
g_LogManager = g_Engine:GetLogManager()
g_GraphicsStats = g_Engine:GetGraphicsStats()
g_Profiler = g_Engine:GetProfiler()

g_EventManager = CEventManager()
g_GameController = CGameController()
g_DamageCalculator = CDamageCalculator()

function Round(num, idp)
  local mult = 10^(idp or 0)
  return math.floor(num * mult + 0.5) / mult
end