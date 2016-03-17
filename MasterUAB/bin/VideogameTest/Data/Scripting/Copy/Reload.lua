function Reload()
	if CInputManager.get_input_manager():is_action_active("ON_ALT1_PRESSED") then
		ReloadScene()
	end
	
	if CInputManager.get_input_manager():is_action_active("ON_ALT2_PRESSED") then
		ReloadEffects()
	end
	
	if CInputManager.get_input_manager():is_action_active("ON_ALT3_PRESSED") then
		ReloadRenderableObjectTechniques()
	end
	
	if CInputManager.get_input_manager():is_action_active("ON_ALT4_PRESSED") then
		ReloadMaterials()
	end
	
	if CInputManager.get_input_manager():is_action_active("ON_ALT5_PRESSED") then
		ReloadAnimatedModels()
	end
	
	if CInputManager.get_input_manager():is_action_active("ON_ALT6_PRESSED") then
		ReloadStaticMeshes()
	end
	
	if CInputManager.get_input_manager():is_action_active("ON_ALT7_PRESSED") then
		ReloadLayers()
	end
	
	if CInputManager.get_input_manager():is_action_active("ON_ALT8_PRESSED") then
		ReloadLights()
	end
	
	if CInputManager.get_input_manager():is_action_active("ON_ALT9_PRESSED") then
		ReloadSceneRendererCommands()
	end
end

function ReloadScene() --Alt+1
	local l_Engine = CEngine.get_singleton()
	l_Engine:get_effect_manager():reload_file()
	l_Engine:get_renderable_object_technique_manager():reload()
	l_Engine:get_material_manager():reload()
	l_Engine:get_animated_model_manager():reload()
	l_Engine:get_staticmesh_manager():reload()
	l_Engine:get_layer_manager():reload()
	l_Engine:get_light_manager():reload()
	l_Engine:get_scene_renderer_command_manager():reload()
	--CEngine.get_singleton()
end

function ReloadEffects() --Alt+2
	local l_Engine = CEngine.get_singleton()
	l_Engine:get_effect_manager():reload_file()
	l_Engine:get_renderable_object_technique_manager():reload()
	l_Engine:get_material_manager():reload()
	l_Engine:get_animated_model_manager():reload()
	l_Engine:get_staticmesh_manager():reload()
	l_Engine:get_layer_manager():reload()
	l_Engine:get_scene_renderer_command_manager():reload()
end

function ReloadRenderableObjectTechniques() --Alt+3
	local l_Engine = CEngine.get_singleton()
	l_Engine:get_renderable_object_technique_manager():reload()
	l_Engine:get_material_manager():reload()
	l_Engine:get_animated_model_manager():reload()
	l_Engine:get_staticmesh_manager():reload()
	l_Engine:get_layer_manager():reload()
	l_Engine:get_scene_renderer_command_manager():reload()
end

function ReloadMaterials() --Alt+4
	local l_Engine = CEngine.get_singleton()
	l_Engine:get_material_manager():reload()
	l_Engine:get_animated_model_manager():reload()
	l_Engine:get_staticmesh_manager():reload()
	l_Engine:get_layer_manager():reload()
	l_Engine:get_scene_renderer_command_manager():reload()
end

function ReloadAnimatedModels() --Alt+5
	local l_Engine = CEngine.get_singleton()
	l_Engine:get_animated_model_manager():reload()
	l_Engine:get_layer_manager():reload()
	l_Engine:get_scene_renderer_command_manager():reload()
end

function ReloadStaticMeshes() --Alt+6
	local l_Engine = CEngine.get_singleton()
	l_Engine:get_staticmesh_manager():reload()
	l_Engine:get_layer_manager():reload()
	l_Engine:get_scene_renderer_command_manager():reload()
end

function ReloadLayers() --Alt+7
	local l_Engine = CEngine.get_singleton()
	l_Engine:get_layer_manager():reload()
	l_Engine:get_scene_renderer_command_manager():reload()
end

function ReloadLights() --Alt+8
	local l_Engine = CEngine.get_singleton()
	l_Engine:get_light_manager():reload()
end

function ReloadSceneRendererCommands() --Alt+9
	local l_Engine = CEngine.get_singleton()
	l_Engine:get_scene_renderer_command_manager():reload()
end