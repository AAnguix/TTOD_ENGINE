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
	local l_Light=g_LightManager:GetResource(LightName)

	g_DebugHelper:RegisterBoolParameter("Active", l_Light:GetActiveLuaAddress(), "")
	g_DebugHelper:RegisterFloatParameter("Intensity", l_Light:GetIntensityLuaAddress(), "min=0.0 max=10.0 step=0.01") --"step=0.1 precision=1")
	g_DebugHelper:RegisterVect3fParameter("Pos", l_Light:GetPositionLuaAddress(), "")
	
	-- g_DebugHelper:RegisterFloatParameter("Yaw", l_Light:GetYawLuaAddress(),"step=0.01")
	-- g_DebugHelper:RegisterFloatParameter("Pitch", l_Light:GetPitchLuaAddress(),"step=0.01")
	-- g_DebugHelper:RegisterFloatParameter("Roll", l_Light:GetRollLuaAddress(),"step=0.01")
	
	g_DebugHelper:RegisterColorParameter("Color", l_Light:GetColorLuaAddress(), "")
	g_DebugHelper:RegisterFloatParameter("Start Attenuation", l_Light:GetStartRangeAttenuationLuaAddress(), "min=0.0 max=20.0 step=0.01")
	g_DebugHelper:RegisterFloatParameter("End Attenuation", l_Light:GetEndRangeAttenuationLuaAddress(), "min=0.0 max=20.0 step=0.01")
	
	if l_Light:GetType() == CLight.directional then
		g_DebugHelper:RegisterVect3fParameter("Direction", l_Light:GetDirectionAddress(),"")
	end
	
	if l_Light:GetType() == CLight.spot then
		g_DebugHelper:RegisterVect3fParameter("Direction", l_Light:GetDirectionAddress(),"")
		g_DebugHelper:RegisterFloatParameter("Angle", l_Light:GetAngleLuaAddress(),"min=-360.0 max=360.0 step=0.01")
		g_DebugHelper:RegisterFloatParameter("FallOff", l_Light:GetFallOffLuaAddress(),"min=-50.0 max=50.0 step=0.01")
	end
end

function AddLightButton(Light)
	local l_DebugHelper=CEngine.GetSingleton():GetDebugHelper()
	l_DebugHelper:RegisterButton(Light:GetName(),"AddLightParameters('"..Light:GetName().."')")
end
