g_PlayerComponent = nil --Store the instance to the player component
g_DragonComponent = nil
g_Player = nil -- ... player LuaGameObjectHandle

g_MagicBall = nil

g_CurrentLuaMainFile = "./Data/Scripting/LuaMainStartScreen.lua"

g_Engine = CEngine.GetSingleton()
g_RenderManager = g_Engine:GetRenderManager()
g_ContextManager = g_RenderManager:GetContextManager()
g_LuabindManager = g_Engine:GetLuabindManager()

-- GameObject must be accesed by LuaHandler
--g_GameObjectManager = g_Engine:GetGameObjectManager()
g_LuaGameObjectHandleManager = g_Engine:GetLuaGameObjectHandleManager()
g_StaticMeshManager = g_Engine:GetStaticMeshManager()
g_AnimatedModelManager = g_Engine:GetAnimatedModelManager()
g_TextureManager = g_Engine:GetTextureManager()
g_EffectManager = g_Engine:GetEffectManager()
g_ParticleSystemManager = g_Engine:GetParticleSystemManager()
g_LightManager = g_Engine:GetLightManager()
g_MaterialManager = g_Engine:GetMaterialManager()
g_RenderableObjectTechniqueManager = g_Engine:GetRenderableObjectTechniqueManager()

g_LayerManager = g_Engine:GetLayerManager()
g_PhysXManager = g_Engine:GetPhysXManager()
g_CameraControllerManager = g_Engine:GetCameraControllerManager()
g_GUIManager = g_Engine:GetGUIManager()
g_SoundManager = g_Engine:GetSoundManager()
g_AnimatorControllerManager = g_Engine:GetAnimatorControllerManager()
g_ScriptManager = g_Engine:GetScriptManager()


g_SceneRendererCommandManager = g_Engine:GetSceneRendererCommandManager()
g_DebugHelper = g_Engine:GetDebugHelper()

g_LogManager = g_Engine:GetLogManager()
g_GraphicsStats = g_Engine:GetGraphicsStats()
g_Profiler = g_Engine:GetProfiler()

g_CinematicActionManager = CCinematicsActionManager()
g_AIManager = CAIManager()
g_EventManager = CEventManager()
g_GameController = CGameController()
g_DamageCalculator = CDamageCalculator()

dofile("./Data/Scripting/Main/ItemManager.lua")
g_ItemManager = CItemManager()

dofile("./Data/Scripting/AntTweakbarReloader.lua")
g_AntTweakbarReloader = CAntTweakBarReloader()

dofile("./Data/Scripting/CameraControllers/LuaCameraController.lua")
g_LuaCameraController = CLuaCameraController()

dofile("./Data/Scripting/GUI/LuaGuiManager.lua")
g_LuaGuiManager = CLuaGuiManager()

g_ShowHealthBarManager = nil

function Round(num, idp)
  local mult = 10^(idp or 0)
  return math.floor(num * mult + 0.5) / mult
end