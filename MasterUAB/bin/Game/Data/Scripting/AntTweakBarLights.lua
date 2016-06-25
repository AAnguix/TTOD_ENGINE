function OpenLights()
	ClickOnElement("Lights","InitializeDebugBar()")
	local l_DebugHelper = CEngine.GetSingleton():GetDebugHelper()
	
	l_DebugHelper:RegisterButton("Export Lights[XML]","WriteLightsInfoToXml()")
	
	local l_LightManager=CEngine.GetSingleton():GetLightManager()
	
	l_DebugHelper:RegisterFloatParameter("Fog Start", l_LightManager:GetFogStartAddress(),"min=0.0 step=0.01 group=\"Fog Properties\"")
	l_DebugHelper:RegisterFloatParameter("Fog End", l_LightManager:GetFogEndAddress(),"min=0.0 step=0.01 group=\"Fog Properties\"")
	l_DebugHelper:RegisterFloatParameter("Density", l_LightManager:GetFogDensityAddress(),"min=0.0 step=0.01 group=\"Fog Properties\"")
	l_DebugHelper:RegisterFloatParameter("FogColor.x", l_LightManager:GetFogColorAddress(0),"min=0.0 max=1.0 step=0.01 group=\"Fog Color\"")
	l_DebugHelper:RegisterFloatParameter("FogColor.y", l_LightManager:GetFogColorAddress(1),"min=0.0 max=1.0 step=0.01 group=\"Fog Color\"")
	l_DebugHelper:RegisterFloatParameter("FogColor.z", l_LightManager:GetFogColorAddress(2),"min=0.0 max=1.0 step=0.01 group=\"Fog Color\"")
	l_DebugHelper:InsertGroupIntoGroup("\"Fog Properties\"","Fog")
	l_DebugHelper:InsertGroupIntoGroup("\"Fog Color\"","Fog")
	l_DebugHelper:Define(" Lights/Fog opened=false ")
	
	l_DebugHelper:RegisterButton("Add Spot","AddLight('SPOT')","group=\"Add Lights\"")
	l_DebugHelper:RegisterButton("Add Directional","AddLight('DIRECTIONAL')","group=\"Add Lights\"")
	l_DebugHelper:RegisterButton("Add Omni","AddLight('OMNI')","group=\"Add Lights\"")
	l_DebugHelper:InsertGroupIntoGroup("\"Add Lights\"","Illumination")
	
	local l_Lights = l_LightManager:GetResourcesVector():size()
	if l_Lights >=1 then
		for i=0,l_Lights-1 do
			local l_Light=l_LightManager:GetResourceById(i)
			AddLightButton(l_Light)
		end
	end
	l_DebugHelper:RegisterFloatParameter("AmbientLight.x", l_LightManager:GetAmbientLightAddress(0),"min=0.0 max=1.0 step=0.001 group=\"Ambient Light\"")
	l_DebugHelper:RegisterFloatParameter("AmbientLight.y", l_LightManager:GetAmbientLightAddress(1),"min=0.0 max=1.0 step=0.001 group=\"Ambient Light\"")
	l_DebugHelper:RegisterFloatParameter("AmbientLight.z", l_LightManager:GetAmbientLightAddress(2),"min=0.0 max=1.0 step=0.001 group=\"Ambient Light\"")
	
	l_DebugHelper:InsertGroupIntoGroup("\"Ambient Light\"","Illumination")
	l_DebugHelper:InsertGroupIntoGroup("\"Lights\"","Illumination")
	l_DebugHelper:Define(" Lights/Illumination opened=false ")
 end
 
 function WriteLightsInfoToXml()
	local l_LightManager=g_Engine:GetLightManager()
	local l_LigthsFilename = "Data/Level"..g_Engine:GetCurrentLevel().."/a_lights.xml"
	local l_Writer = CTTODXMLWriter()
	l_Writer:StartFile(l_LigthsFilename)
		l_Writer:StartElement("lights", true)
			WriteLightsParameters(l_Writer)
			local l_Lights = g_LightManager:GetResourcesVector():size()
			for i=0,l_Lights-1 do
				local l_Light=l_LightManager:GetResourceById(i)
				l_Writer:StartElement("ligth", true)
					l_Writer:WriteStringProperty("name", l_Light:GetName())
					l_Writer:WriteBoolProperty("active", l_Light:GetActive())
					local l_Type = "OMNI"
					if (l_Light:GetType() == CLight.DIRECTIONAL) then
						l_Type = "DIRECTIONAL"
					elseif (l_Light:GetType() == CLight.SPOT) then
						l_Type = "SPOT"
					end
					l_Writer:WriteStringProperty("type", l_Type)
					l_Writer:WriteVect3fProperty("position", l_Light:GetPosition())
					if (l_Type==DIRECTIONAL) then
						l_Writer:WriteFloatProperty("direction", l_Light:GetDirection())
					end
					l_Writer:WriteVect4fProperty("color", l_Light:GetColor())
					l_Writer:WriteFloatProperty("att_start_range", l_Light:GetStartRangeAttenuation())
					l_Writer:WriteFloatProperty("att_end_range", l_Light:GetEndRangeAttenuation())
					l_Writer:WriteFloatProperty("intensity", l_Light:GetIntensity())
				l_Writer:EndElement()	
			end
		l_Writer:EndElement()	
	l_Writer:EndFile()
end

function WriteLightsParameters(Writer)
	local l_Engine = CEngine.GetSingleton()
	local l_LightManager = l_Engine:GetLightManager()
	Writer:WriteVect4fProperty("ambient", l_LightManager:GetAmbientLight())	
	Writer:WriteVect4fProperty("fog_color", l_LightManager:GetFogColor())
	Writer:WriteFloatProperty("fog_start", l_LightManager:GetFogParameters().x)
	Writer:WriteFloatProperty("fog_end", l_LightManager:GetFogParameters().y)
	Writer:WriteFloatProperty("fog_density", l_LightManager:GetFogParameters().z)
end
 
function AddLightParameters(LightName)
	ClickOnElement(LightName,"OpenLights()")
	local l_Light=g_LightManager:GetResource(LightName)

	g_DebugHelper:RegisterBoolParameter("Active", l_Light:GetActiveLuaAddress(), "")
	g_DebugHelper:RegisterFloatParameter("Intensity", l_Light:GetIntensityLuaAddress(), "min=0.0 max=10.0 step=0.01") --"step=0.1 precision=1")
	g_DebugHelper:RegisterFloatParameter("Pos.x", l_Light:GetPositionLuaAddress(0), "step=0.01 group=\"Position\"")
	g_DebugHelper:RegisterFloatParameter("Pos.y", l_Light:GetPositionLuaAddress(1), "step=0.01 group=\"Position\"")
	g_DebugHelper:RegisterFloatParameter("Pos.z", l_Light:GetPositionLuaAddress(2), "step=0.01 group=\"Position\"")
	
	-- g_DebugHelper:RegisterFloatParameter("Yaw", l_Light:GetYawLuaAddress(),"step=0.01")
	-- g_DebugHelper:RegisterFloatParameter("Pitch", l_Light:GetPitchLuaAddress(),"step=0.01")
	-- g_DebugHelper:RegisterFloatParameter("Roll", l_Light:GetRollLuaAddress(),"step=0.01")
	
	g_DebugHelper:RegisterColorParameter("Color", l_Light:GetColorLuaAddress(), "")
	g_DebugHelper:RegisterFloatParameter("Start Attenuation", l_Light:GetStartRangeAttenuationLuaAddress(), "min=0.0 max=20.0 step=0.01")
	g_DebugHelper:RegisterFloatParameter("End Attenuation", l_Light:GetEndRangeAttenuationLuaAddress(), "min=0.0 max=20.0 step=0.01")
	
	if l_Light:GetType() == CLight.DIRECTIONAL then
		g_DebugHelper:RegisterVect3fParameter("Direction", l_Light:GetDirectionAddress(),"")
	end
	
	if l_Light:GetType() == CLight.SPOT then
		g_DebugHelper:RegisterVect3fParameter("Direction", l_Light:GetDirectionAddress(),"")
		g_DebugHelper:RegisterFloatParameter("Angle", l_Light:GetAngleLuaAddress(),"min=-360.0 max=360.0 step=0.01")
		g_DebugHelper:RegisterFloatParameter("FallOff", l_Light:GetFallOffLuaAddress(),"min=-50.0 max=50.0 step=0.01")
	end
	g_DebugHelper:RegisterButton("Remove","RemoveLight('"..l_Light:GetName().."')")
end

function AddLight(LightType)
	g_LightManager:AddLight(CLight.GetLightTypeByName(LightType))
	OpenLights()
end

function AddLightButton(Light)
	g_DebugHelper:RegisterButton(Light:GetName(),"AddLightParameters('"..Light:GetName().."')","group=\"Lights\"")
end

function RemoveLight(LightName)
	g_LightManager:RemoveResource(LightName)
	OpenLights()
end