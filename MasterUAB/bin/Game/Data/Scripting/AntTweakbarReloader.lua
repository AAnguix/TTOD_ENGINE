class 'CAntTweakBarReloader'
function CAntTweakBarReloader:__init()
	self:SubscribeEvents()
end

function CAntTweakBarReloader:SubscribeEvents()
	g_EventManager:Subscribe(self, "ON_ALT1_PRESSED")
	g_EventManager:Subscribe(self, "ON_ALT2_PRESSED")
	g_EventManager:Subscribe(self, "ON_ALT3_PRESSED")
	g_EventManager:Subscribe(self, "ON_ALT4_PRESSED")
	g_EventManager:Subscribe(self, "ON_ALT5_PRESSED")
	g_EventManager:Subscribe(self, "ON_ALT6_PRESSED")
	g_EventManager:Subscribe(self, "ON_ALT7_PRESSED")
	g_EventManager:Subscribe(self, "ON_ALT8_PRESSED")
	g_EventManager:Subscribe(self, "ON_ALT9_PRESSED")
	g_EventManager:Subscribe(self, "RELOAD_LUA")
end

function CAntTweakBarReloader:ON_ALT1_PRESSED() self:ReloadScene() end
function CAntTweakBarReloader:ON_ALT2_PRESSED() self:ReloadEffects()() end
function CAntTweakBarReloader:ON_ALT3_PRESSED() self:ReloadAnimatedModels() end
function CAntTweakBarReloader:ON_ALT4_PRESSED() self:ReloadStaticMeshes() end
function CAntTweakBarReloader:ON_ALT5_PRESSED() self:ReloadLayers() end
function CAntTweakBarReloader:ON_ALT6_PRESSED() self:ReloadLights() end
function CAntTweakBarReloader:ON_ALT7_PRESSED() self:ReloadMaterials() end
function CAntTweakBarReloader:ON_ALT8_PRESSED() self:ReloadSceneRendererCommands() end
function CAntTweakBarReloader:ON_ALT9_PRESSED() self:ReloadGUI() end
function CAntTweakBarReloader:RELOAD_LUA() self:ReloadLua() end

function CAntTweakBarReloader:ReloadLua()
	CEngine.GetSingleton():GetDebugHelper():RemoveBar()
	self:ReloadLog()
	dofile(g_CurrentLuaMainFile)
	LuaMain()
	g_ScriptManager:RemoveComponents()
	-- g_PhysXManager:RemoveComponents()
	g_SoundManager:RemoveComponents()
	g_AnimatorControllerManager:RemoveComponents()
	g_GameController:Reload()
	dofile("./Data/Scripting/GUI/LuaGuiManager.lua")
	g_LuaGuiManager:Reload()
end

function CAntTweakBarReloader:ReloadScene()
	local l_Engine = CEngine.GetSingleton()
	if g_CurrentLevel ~= 0 then
		g_ScriptManager:RemoveComponents()
		g_PhysXManager:Reload()
		g_SoundManager:Reload()
		--l_Engine:GetEffectManager():ReloadFile()
		--l_Engine:GetRenderableObjectTechniqueManager():Reload()
		g_AnimatorControllerManager:RemoveComponents()
		g_MaterialManager:Reload()
		--g_MaterialManager:Load("./Data/effects_materials.xml")
		--g_MaterialManager:Load("./Data/gui_materials.xml")
		g_StaticMeshManager:Reload()
		g_ParticleSystemManager:Reload()
		g_AnimatedModelManager:Reload()
		g_LayerManager:Reload()
		g_LightManager:Reload()
		g_GUIManager:Reload()
		InitializeGUI()
		g_GameController:Reload()
		g_SceneRendererCommandManager:Reload()
		--g_CameraControllerManager:Reload()
		--g_GUIManager:Reload()
	end
end

function CAntTweakBarReloader:ReloadEffects() 
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

function CAntTweakBarReloader:ReloadEffectTechnique(EffectTechniqueName)
	local l_Technique=CEngine.GetSingleton():GetEffectManager():GetResource(EffectTechniqueName)
	l_Technique:Refresh()
end

function CAntTweakBarReloader:ReloadStaticMesh(CStaticMesh) 
	CStaticMesh:Reload() 
end

function CAntTweakBarReloader:ReloadRenderableObjectTechniques() 
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

function CAntTweakBarReloader:ReloadMaterials()
	local l_Engine = CEngine.GetSingleton()
	l_Engine:GetPhysXManager():Reload()
	l_Engine:GetMaterialManager():Reload()
	l_Engine:GetAnimatedModelManager():Reload()
	l_Engine:GetStaticMeshManager():Reload()
	l_Engine:GetLayerManager():Reload()
	l_Engine:GetLightManager():Reload()
	l_Engine:GetSceneRendererCommandManager():Reload()
end

function CAntTweakBarReloader:ReloadAnimatedModels() 
	local l_Engine = CEngine.GetSingleton()
	l_Engine:GetPhysXManager():Reload()
	l_Engine:GetAnimatedModelManager():Reload()
	l_Engine:GetLayerManager():Reload()
	l_Engine:GetLightManager():Reload()
	l_Engine:GetSceneRendererCommandManager():Reload()
end

function CAntTweakBarReloader:ReloadStaticMeshes() 
	local l_Engine = CEngine.GetSingleton()
	l_Engine:GetPhysXManager():Reload()
	l_Engine:GetStaticMeshManager():Reload()
	l_Engine:GetLayerManager():Reload()
	l_Engine:GetLightManager():Reload()
	l_Engine:GetSceneRendererCommandManager():Reload()
end

function CAntTweakBarReloader:ReloadLayers() 
	local l_Engine = CEngine.GetSingleton()
	l_Engine:GetPhysXManager():Reload()
	l_Engine:GetLayerManager():Reload()
	l_Engine:GetLightManager():Reload()
	l_Engine:GetSceneRendererCommandManager():Reload()
end

function CAntTweakBarReloader:ReloadLights()
	local l_Engine = CEngine.GetSingleton()
	l_Engine:GetLightManager():Reload()
	l_Engine:GetSceneRendererCommandManager():Reload()
end

function CAntTweakBarReloader:ReloadSceneRendererCommands() 
	local l_Engine = CEngine.GetSingleton()
	l_Engine:GetSceneRendererCommandManager():Reload()
end

function CAntTweakBarReloader:ReloadGUI()
	g_GUIManager:Reload()
	dofile(g_CurrentGuiFile) --Reload GUI___.lua
	InitializeGUI()
end

function CAntTweakBarReloader:ReloadCameras()
	g_CameraControllerManager:Reload()
end

function CAntTweakBarReloader:ReloadParticles()
	g_ParticleSystemManager:Reload()
end

function CAntTweakBarReloader:ReloadCameras()
	local l_Engine = CEngine.GetSingleton()
	l_Engine:GetCameraControllerManager():Reload()
end

function CAntTweakBarReloader:ReloadLog()
	g_LogManager:Reload(false)
end