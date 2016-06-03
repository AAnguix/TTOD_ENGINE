function Reload()
	if CInputManager.GetInputManager():IsActionActive("ON_ALT1_PRESSED") then
		ReloadScene()
	end
	
	if CInputManager.GetInputManager():IsActionActive("ON_ALT2_PRESSED") then
		ReloadEffects()
	end
	
	if CInputManager.GetInputManager():IsActionActive("ON_ALT3_PRESSED") then
		ReloadAnimatedModels()
	end
	
	if CInputManager.GetInputManager():IsActionActive("ON_ALT4_PRESSED") then
		ReloadStaticMeshes()
	end
	
	if CInputManager.GetInputManager():IsActionActive("ON_ALT5_PRESSED") then
		ReloadLayers()
	end
	
	if CInputManager.GetInputManager():IsActionActive("ON_ALT6_PRESSED") then
		ReloadLights()
	end
	
	if CInputManager.GetInputManager():IsActionActive("ON_ALT7_PRESSED") then
		ReloadMaterials()
	end
	
	if CInputManager.GetInputManager():IsActionActive("ON_ALT8_PRESSED") then
		ReloadSceneRendererCommands()
	end
	
	if CInputManager.GetInputManager():IsActionActive("ON_ALT9_PRESSED") then
		ReloadGUI()
	end
	
	if CInputManager.GetInputManager():IsActionActive("RELOAD_LUA") then
		ReloadLua()
	end
end

function ReloadLua()
	CEngine.GetSingleton():GetDebugHelper():RemoveBar()
	ReloadLog()
	dofile(g_CurrentLuaMainFile)
	LuaMain()
	g_ScriptManager:RemoveComponents()
	g_PhysXManager:RemoveComponents()
	g_SoundManager:RemoveComponents()
	g_AnimatorControllerManager:RemoveComponents()
	g_GameController:Reload()
end

function ReloadScene()
	local l_Engine = CEngine.GetSingleton()
	if g_CurrentLevel ~= 0 then 
		g_PhysXManager:Reload()
		l_Engine:GetEffectManager():ReloadFile()
		l_Engine:GetRenderableObjectTechniqueManager():Reload()
		l_Engine:GetMaterialManager():Reload()
		l_Engine:GetAnimatedModelManager():Reload()
		l_Engine:GetStaticMeshManager():Reload()
		g_LayerManager:Reload()
		l_Engine:GetLightManager():Reload()
		g_SceneRendererCommandManager:Reload()
		g_CameraControllerManager:Reload()
		g_GUIManager:Reload()
	end
end

function ReloadEffects() 
	local l_Engine = CEngine.GetSingleton()
	l_Engine:GetPhysXManager():Reload()
	l_Engine:GetEffectManager():ReloadFile()
	l_Engine:GetRenderableObjectTechniqueManager():Reload()
	l_Engine:GetMaterialManager():Reload()
	l_Engine:GetAnimatedModelManager():Reload()
	l_Engine:GetStaticMeshManager():Reload()
	l_Engine:GetLayerManager():Reload()
	l_Engine:GetLightManager():Reload()
	l_Engine:GetSceneRendererCommandManager():Reload()
end

function ReloadEffectTechnique(EffectTechniqueName)
	local l_Technique=CEngine.GetSingleton():GetEffectManager():GetResource(EffectTechniqueName)
	l_Technique:Refresh()
end

function ReloadRenderableObjectTechniques() 
	local l_Engine = CEngine.GetSingleton()
	l_Engine:GetPhysXManager():Reload()
	l_Engine:GetRenderableObjectTechniqueManager():Reload()
	l_Engine:GetMaterialManager():Reload()
	l_Engine:GetAnimatedModelManager():Reload()
	l_Engine:GetStaticMeshManager():Reload()
	l_Engine:GetLayerManager():Reload()
	l_Engine:GetLightManager():Reload()
	l_Engine:GetSceneRendererCommandManager():Reload()
end

function ReloadMaterials()
	local l_Engine = CEngine.GetSingleton()
	l_Engine:GetPhysXManager():Reload()
	l_Engine:GetMaterialManager():Reload()
	l_Engine:GetAnimatedModelManager():Reload()
	l_Engine:GetStaticMeshManager():Reload()
	l_Engine:GetLayerManager():Reload()
	l_Engine:GetLightManager():Reload()
	l_Engine:GetSceneRendererCommandManager():Reload()
end

function ReloadAnimatedModels() 
	local l_Engine = CEngine.GetSingleton()
	l_Engine:GetPhysXManager():Reload()
	l_Engine:GetAnimatedModelManager():Reload()
	l_Engine:GetLayerManager():Reload()
	l_Engine:GetLightManager():Reload()
	l_Engine:GetSceneRendererCommandManager():Reload()
end

function ReloadStaticMeshes() 
	local l_Engine = CEngine.GetSingleton()
	l_Engine:GetPhysXManager():Reload()
	l_Engine:GetStaticMeshManager():Reload()
	l_Engine:GetLayerManager():Reload()
	l_Engine:GetLightManager():Reload()
	l_Engine:GetSceneRendererCommandManager():Reload()
end

function ReloadLayers() 
	local l_Engine = CEngine.GetSingleton()
	l_Engine:GetPhysXManager():Reload()
	l_Engine:GetLayerManager():Reload()
	l_Engine:GetLightManager():Reload()
	l_Engine:GetSceneRendererCommandManager():Reload()
end

function ReloadLights()
	local l_Engine = CEngine.GetSingleton()
	l_Engine:GetLightManager():Reload()
	l_Engine:GetSceneRendererCommandManager():Reload()
end

function ReloadSceneRendererCommands() 
	local l_Engine = CEngine.GetSingleton()
	l_Engine:GetSceneRendererCommandManager():Reload()
end

function ReloadGUI()
	g_GUIManager:Reload()
	dofile(g_CurrentGuiFile) --Reload GUI___.lua
	InitializeGUI()
end

function ReloadCameras()
	g_CameraControllerManager:Reload()
end

function ReloadParticles()
	g_ParticleSystemManager:Reload()
end

function ReloadCameras()
	local l_Engine = CEngine.GetSingleton()
	l_Engine:GetCameraControllerManager():Reload()
end

function ReloadLog()
	g_LogManager:Reload(false)
end