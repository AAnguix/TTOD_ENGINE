-- function LuaMain()
	-- local l_Lights=CEngine.get_singleton():get_light_manager():get_resources_vector()
	-- local l_DebugHelper=CEngine.get_singleton():get_debug_helper()
	-- for i=1, l_Lights:size() do
		-- local l_Light=l_Lights:get_resource_by_id(i)
		-- l_DebugHelper:register_button("Lights","Light "..l_Light:get_name(), "RegisterLight('"..l_Light:get_name().."')")
	-- end
-- end

-- function RegisterLights()
	-- local l_LightManager=CEngine.get_singleton():get_light_manager()
	-- local l_Lights = l_LightManager:get_resources_vector():size()
	-- for i=1, l_Lights do
		-- local l_Light=l_LightManager:get_resource_by_id(i)
		-- RegisterLight(l_Light)
	-- end
-- end

-- function RegisterLight(Light)
	-- local l_DebugHelper=CEngine.get_singleton():get_debug_helper()
	-- l_DebugHelper:register_float_parameter("Lights","color", Light:GetColorAddress(), "step=0.1 precision=1")
	-- l_DebugHelper:register_float_parameter("Lights","start range attenuation", Light:GetStartRangeAttenuation(),"")
	-- l_DebugHelper:register_float_parameter("Lights","end range attenuation", Light:GetEndRangeAttenuation(),"")
-- end

function OpenMaterials()
local l_DebugHelper=CEngine.get_singleton():get_debug_helper()
	l_DebugHelper:create_bar("Material")
end

function InitializeDebugBar()
	local l_DebugHelper=CEngine.get_singleton():get_debug_helper()
	l_DebugHelper:create_bar("Debug Bar")
	l_DebugHelper:register_button("Debug Bar","Scene","ReloadScene()")
	l_DebugHelper:register_button("Debug Bar","Effects","OpenEffects()")
	l_DebugHelper:register_button("Debug Bar","RO. Techniques","OpenRenderableObjectTechniques()")
	l_DebugHelper:register_button("Debug Bar","Materials","OpenMaterials()")
	l_DebugHelper:register_button("Debug Bar","Animated Models","OpenAnimatedModels()")
	l_DebugHelper:register_button("Debug Bar","Static Meshes","OpenStaticMeshes()")
	l_DebugHelper:register_button("Debug Bar","Layers","OpenLayers()")
	l_DebugHelper:register_button("Debug Bar","Lights","OpenLights()")
	l_DebugHelper:register_button("Debug Bar","Commands","OpenSceneRendererCommands()")
end

function OpenEffects()
	local l_DebugHelper=CEngine.get_singleton():get_debug_helper()
	l_DebugHelper:create_bar("EFFECTS")
	-- -- local l_EffectManager=CEngine.get_singleton():get_effect_manager():get_resources_vector()
	-- -- local l_Effects = l_EffectManager:get_resources_vector():size()
	-- -- for i=1, l_Effects do
		-- -- local l_Effect=l_EffectManager:get_resource_by_id(i)
		-- -- RegisterEffect(l_Effect)
	-- -- end
	
	-- -- l_DebugHelper:register_button("EFFECTS","Close", "RemoveBar(EFFECTS)")
end

-- function RegisterEffect(Effect)
	-- local l_DebugHelper=CEngine.get_singleton():get_debug_helper()
	-- l_DebugHelper:register_button("EFFECTS","Efecto1", "")
-- end

function OpenRenderableObjectTechniques()

end



function OpenAnimatedModels()

end

function OpenStaticMeshes()

end

function OpenLayers()

end

function OpenLights()

end

function OpenSceneRendererCommands()

end

function RemoveBar(BarName)
	local l_DebugHelper=CEngine.get_singleton():get_debug_helper()
	l_DebugHelper:remove_bar(BarName)
end
