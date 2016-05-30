function OpenLights()
	ClickOnElement("Lights")
	local l_LightManager=CEngine.GetSingleton():GetLightManager()
	local l_Lights = l_LightManager:GetResourcesVector():size()
	for i=0,l_Lights-1 do
		local l_Light=l_LightManager:GetResourceById(i)
		AddLightButton(l_Light)
	end
	
	CEngine.GetSingleton():GetDebugHelper():RegisterFloatParameter("Fog Start", l_LightManager:GetFogStartAddress(),"min=0.0 max=100.0 step=0.01")
	CEngine.GetSingleton():GetDebugHelper():RegisterFloatParameter("Fog End", l_LightManager:GetFogEndAddress(),"min=0.0 max=100.0 step=0.01")
	CEngine.GetSingleton():GetDebugHelper():RegisterFloatParameter("Density", l_LightManager:GetFogDensityAddress(),"min=0.0 max=0.5 step=0.001")
	--CEngine.GetSingleton():GetDebugHelper():RegisterVect4fParameter("Fog parameters", l_LightManager:GetFogParametersAddress(),"")
	CEngine.GetSingleton():GetDebugHelper():RegisterVect4fParameter("Fog color", l_LightManager:GetFogColorAddress(),"")
 end
 
function AddLightParameters(LightName)
	ClickOnElement(LightName)
	local l_DebugHelper=CEngine.GetSingleton():GetDebugHelper()
	local l_Light=CEngine.GetSingleton():GetLightManager():GetResource(LightName)

	l_DebugHelper:RegisterBoolParameter("Active", l_Light:GetActiveLuaAddress(), "")
	l_DebugHelper:RegisterFloatParameter("Intensity", l_Light:GetIntensityLuaAddress(), "min=0.0 max=10.0 step=0.01") --"step=0.1 precision=1")
	l_DebugHelper:RegisterVect3fParameter("Pos", l_Light:GetPositionLuaAddress(), "")
	l_DebugHelper:RegisterColorParameter("Color", l_Light:GetColorLuaAddress(), "")
	l_DebugHelper:RegisterFloatParameter("Start Attenuation", l_Light:GetStartRangeAttenuationLuaAddress(), "min=0.0 max=20.0 step=0.01")
	l_DebugHelper:RegisterFloatParameter("End Attenuation", l_Light:GetEndRangeAttenuationLuaAddress(), "min=0.0 max=20.0 step=0.01")
	
	if l_Light:get_type() == CLight.directional then
		l_DebugHelper:RegisterVect3fParameter("Direction", l_Light:GetDirectionAddress(),"")
	end
	
	if l_Light:get_type() == CLight.spot then
		l_DebugHelper:RegisterFloatParameter("Angle", l_Light:GetAngleAddress(),"")
		l_DebugHelper:RegisterFloatParameter("FallOff", l_Light:GetFallOffAddress(),"")
	end
end

function AddLightButton(Light)
	local l_DebugHelper=CEngine.GetSingleton():GetDebugHelper()
	l_DebugHelper:RegisterButton(Light:GetName(),"AddLightParameters('"..Light:GetName().."')")
end
