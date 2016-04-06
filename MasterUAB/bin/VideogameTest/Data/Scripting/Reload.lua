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
		CEngine.GetSingleton():GetDebugHelper():RemoveBar()
		LuaMain()
		ReloadLog()
	end
end

function ReloadScene()
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
	l_Engine:GetCameraControllerManager():Reload()
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
	dofile("./Data/Scripting/GUI.lua")
	InitializeGUI()
end

function ReloadParticles()
	g_ParticleSystemManager:Reload()
end

function ReloadCameras()
	local l_Engine = CEngine.GetSingleton()
	l_Engine:GetCameraControllerManager():Reload()
end

function ReloadLog()
	local l_Engine = CEngine.GetSingleton()
	l_Engine:GetLogManager():Reload(false)
end