function DestroyLua()
	g_PlayerComponent = nil 
	g_Player = nil

	g_MagicBall = nil
	g_Dragon = nil

	g_CurrentGuiFile = nil
	g_CurrentLuaMainFile = nil

	g_Engine = nil
	g_RenderManager = nil
	g_ContextManager = nil
	g_LuabindManager = nil

	g_GameObjectManager = nil
	g_LuaGameObjectHandleManager = nil
	g_StaticMeshManager = nil
	g_AnimatedModelManager = nil
	g_TextureManager = nil
	g_EffectManager = nil
	g_ParticleSystemManager = nil
	g_LightManager = nil
	g_MaterialManager = nil

	g_LayerManager = nil
	g_PhysXManager = nil
	g_CameraControllerManager = nil
	g_GUIManager = nil
	g_SoundManager = nil
	g_AnimatorControllerManager = nil
	g_ScriptManager = nil
	
	g_SceneRendererCommandManager = nil
	g_DebugHelper = nil

	--g_LogManager = nil
	g_GraphicsStats = nil
	g_Profiler = nil

	g_EventManager = nil
	g_CinematicActionManager = nil
	g_AIManager = nil
	g_GameController:Destroy()
	g_GameController = nil
	g_DamageCalculator = CDamageCalculator()

	g_AntTweakbarReloader = nil

	g_LuaCameraController = nil
	
	g_LuaGuiManager = nil
end