function AddBackButton()
	CEngine.GetSingleton():GetDebugHelper():RegisterButton("..","BackButtonPressed()")
end

function BackButtonPressed()
	CEngine.GetSingleton():GetDebugHelper():RemoveBar()
	CEngine.GetSingleton():GetDebugHelper():ResetButtons()
	InitializeDebugBar()
end

function ClickOnElement(NewBarName)
	CEngine.GetSingleton():GetDebugHelper():RemoveBar()
	CEngine.GetSingleton():GetDebugHelper():ResetButtons()
	CEngine.GetSingleton():GetDebugHelper():CreateBar(NewBarName)
	AddBackButton()
end

function InitializeDebugBar()
	local l_DebugHelper=CEngine.GetSingleton():GetDebugHelper()
	l_DebugHelper:CreateBar("Debug Bar")
	l_DebugHelper:RegisterButton("[1]Scene","ReloadScene()")
	l_DebugHelper:RegisterButton("[2]Effects","OpenEffects()")
	-- l_DebugHelper:RegisterButton("[3]ROTechniques","OpenRenderableObjectTechniques()")
	--l_DebugHelper:RegisterButton("[4]Materials","OpenMaterials()")
	l_DebugHelper:RegisterButton("[3]AnimatedModels","OpenAnimatedModels()")
	l_DebugHelper:RegisterButton("[4]StaticMeshes","OpenStaticMeshes()")
	l_DebugHelper:RegisterButton("[5]Layers","OpenLayers()")
	l_DebugHelper:RegisterButton("[6]Lights","OpenLights()")
	-- l_DebugHelper:RegisterButton("[9]Commands","OpenSceneRendererCommands()")
	l_DebugHelper:RegisterButton("[7]Materials","OpenMaterials()")
	l_DebugHelper:RegisterButton("[8]Commands","ReloadSceneRendererCommands()")
	l_DebugHelper:RegisterButton("[9]GUI","ReloadGUI()")
	l_DebugHelper:RegisterButton("Particles","OpenParticles()")
	l_DebugHelper:RegisterButton("Cameras","ReloadCameras()")
end

function OpenEffects()
	ClickOnElement("Effects")
	local l_DebugHelper=CEngine.GetSingleton():GetDebugHelper()
	local l_Effects=CEngine.GetSingleton():GetEffectManager():GetLUAEffects()
	for l_Effect in l_Effects do
		l_DebugHelper:RegisterButton(l_Effect:GetName(),"ReloadEffectTechnique('"..l_Effect:GetName().."')")
	end
end

function OpenRenderableObjectTechniques()
	ClickOnElement("ROTechniques")
end

------------------------------ MATERIALS -------------------------------------------------------------------------
dofile("./Data/Scripting/AntTweakBarMaterials.lua")

function OpenAnimatedModels()
	ClickOnElement("Animated Models")
	local l_DebugHelper=CEngine.GetSingleton():GetDebugHelper()
	local l_Models=CEngine.GetSingleton():GetAnimatedModelManager():GetLUAAnimatedModels()
	for l_Model in l_Models do
		l_DebugHelper:RegisterButton(l_Model:GetName(),"")
	end
end

function OpenStaticMeshes()
	ClickOnElement("Static Meshes")
	local l_DebugHelper=CEngine.GetSingleton():GetDebugHelper()
	local l_SMeshes=CEngine.GetSingleton():GetStaticMeshManager():GetLUAStaticMeshes()
	for l_SMesh in l_SMeshes do
		l_DebugHelper:RegisterButton(l_SMesh:GetName(),"")
	end
end

function OpenLayers()
	ClickOnElement("Layers")
	--CEngine.GetSingleton():GetDebugHelper():RegisterButton("Reload All","ReloadLayers()")
	local l_LayerManager=CEngine.GetSingleton():GetLayerManager()
	local l_ROManagers = l_LayerManager:GetResourcesVector():size()
	for i=0,l_ROManagers-1 do
		local l_ROMan=l_LayerManager:GetResourceById(i)
		AddROManagerButton(l_ROMan)
	end
end

function AddROManagerButton(ROManager)
	local l_DebugHelper=CEngine.GetSingleton():GetDebugHelper()
	l_DebugHelper:RegisterButton(ROManager:GetName(),"OpenRenderableObjectManager('"..ROManager:GetName().."')")
end

function OpenRenderableObjectManager(ROManagerName)
	ClickOnElement(ROManagerName)
	CEngine.GetSingleton():GetDebugHelper():RegisterButton("Reload","ReloadRenderableObjectManager('"..ROManagerName.."')")
	local l_ROManager=CEngine.GetSingleton():GetLayerManager():GetResource(ROManagerName)	
	local l_RObjects = l_ROManager:GetResourcesVector():size()
	for i=0,l_RObjects-1 do
		local l_RObject=l_ROManager:GetResourceById(i)
		AddRenderableObjectButton(l_RObject)
	end
end

function AddRenderableObjectButton(RenderableObject)
	local l_DebugHelper=CEngine.GetSingleton():GetDebugHelper()
	local l_Name = RenderableObject:GetName()
	l_DebugHelper:RegisterButton(l_Name,"ReloadRenderableObject()")
end

function ReloadRenderableObjectManager(Name)
	--CEngine.GetSingleton():GetLayerManager():GetResource(Name):Reload()
end

function ReloadRenderableObject()

end

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
 
 function OpenSceneRendererCommands()
	ClickOnElement("Commands")
 end
 
function AddLightParameters(LightName)
	ClickOnElement(LightName)
	local l_DebugHelper=CEngine.GetSingleton():GetDebugHelper()
	local l_Light=CEngine.GetSingleton():GetLightManager():GetResource(LightName)

	l_DebugHelper:RegisterBoolParameter("Active", l_Light:GetActiveAddress(), "")
	l_DebugHelper:RegisterFloatParameter("Intensity", l_Light:GetIntensityAddress(), "min=0.0 max=10.0 step=0.01") --"step=0.1 precision=1")
	l_DebugHelper:RegisterPositionOrientationParameter("Pos", l_Light:GetPositionAddress(), "")
	l_DebugHelper:RegisterColorParameter("Color", l_Light:GetColorAddress(), "")
	l_DebugHelper:RegisterFloatParameter("Start Attenuation", l_Light:GetStartRangeAttenuationAddress(), "")
	l_DebugHelper:RegisterFloatParameter("End Attenuation", l_Light:GetEndRangeAttenuationAddress(), "")
	
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

------------------------------ PARTICLES -------------------------------------------------------------------------

function OpenParticles()
	ClickOnElement("Particles")
	local l_ParticleManager=CEngine.GetSingleton():GetLayerManager():GetResource("particles")
	local l_Particles = l_ParticleManager:GetResourcesVector():size()
	local l_DebugHelper=CEngine.GetSingleton():GetDebugHelper()
	
	for i=0,l_Particles-1 do
		local l_Particle=l_ParticleManager:GetResourceById(i)
		l_DebugHelper:RegisterButton(l_Particle:GetName(),"OpenParticle('"..l_Particle:GetName().."')")
	end
end

function OpenParticle(ParticleName)
	ClickOnElement(ParticleName)
	local l_ParticleSystem = CEngine.GetSingleton():GetLayerManager():GetResource("particles"):GetResource(ParticleName)
	local l_DebugHelper=CEngine.GetSingleton():GetDebugHelper()
	
	local l_Type = l_ParticleSystem:GetType()
	local l_Material = l_Type:GetMaterial()
	l_DebugHelper:RegisterExtendedButton("Size C.P","OpenSizeControlPoints",l_Type:GetThisLuaAddress(),"partycle_system_type")
	l_DebugHelper:RegisterExtendedButton("Color C.P","OpenColorControlPoints",l_Type:GetThisLuaAddress(),"partycle_system_type")
	
	l_DebugHelper:RegisterButton("Material ("..l_Material:GetName()..")","OpenMaterial('"..l_Material:GetName().."')")
	
	l_DebugHelper:RegisterVect3fParameter("EmissionBoxHalfSize",l_ParticleSystem:GetEmissionBoxHalfSizeLuaAddress(),"")
	l_DebugHelper:RegisterFloatParameter("Yaw",l_ParticleSystem:GetYawLuaAddress(),"min=0.0 max=3.14 step=0.01")
	l_DebugHelper:RegisterFloatParameter("Pitch",l_ParticleSystem:GetPitchLuaAddress(),"min=0.0 max=3.14 step=0.01")
	l_DebugHelper:RegisterFloatParameter("Roll",l_ParticleSystem:GetRollLuaAddress(),"min=0.0 max=3.14 step=0.01")
	
	l_DebugHelper:RegisterInt32Parameter("Num.Frames",l_Type:GetNumFramesLuaAddress(),"min=0 max=100 step=1")
	l_DebugHelper:RegisterFloatParameter("Time/frame",l_Type:GetTimePerFrameLuaAddress(),"min=0.0 max=5.0 step=0.01")
	l_DebugHelper:RegisterBoolParameter("Loop",l_Type:GetLoopLuaAddress(),"")
	l_DebugHelper:RegisterBoolParameter("Emit absolute",l_Type:GetEmitAbsoluteLuaAddress(),"")
	
	l_DebugHelper:RegisterFloatParameter("Starting direction angle",l_Type:GetStartingDirectionAngleLuaAddress(),"min=0 max=100 step=1")
	l_DebugHelper:RegisterFloatParameter("Starting acceleration angle ",l_Type:GetStartingAccelerationAngleLuaAddress(),"min=0 max=100 step=1")
	l_DebugHelper:RegisterVect2fParameter("Size",l_Type:GetSizeLuaAddress(),"")
	
	l_DebugHelper:RegisterVect2fParameter("Emit rate",l_ParticleSystem:GetType():GetEmitRateLuaAddress(),"")
	l_DebugHelper:RegisterVect2fParameter("Awake time",l_ParticleSystem:GetType():GetAwakeTimeLuaAddress(),"")
	l_DebugHelper:RegisterVect2fParameter("Sleep time",l_ParticleSystem:GetType():GetSleepTimeLuaAddress(),"")
	l_DebugHelper:RegisterVect2fParameter("Life",l_ParticleSystem:GetType():GetLifeLuaAddress(),"")
	
	l_DebugHelper:RegisterVect2fParameter("Starting angle",l_ParticleSystem:GetType():GetStartingAngleLuaAddress(),"")
	l_DebugHelper:RegisterVect2fParameter("Starting angular speed",l_ParticleSystem:GetType():GetStartingAngularSpeedLuaAddress(),"")
	l_DebugHelper:RegisterVect2fParameter("Angular acceleration",l_ParticleSystem:GetType():GetAngularAccelerationLuaAddress(),"")
	
	l_DebugHelper:RegisterVect3fParameter("Starting speed 1",l_ParticleSystem:GetType():GetStartingSpeed1LuaAddress(),"")
	l_DebugHelper:RegisterVect3fParameter("Starting speed 2",l_ParticleSystem:GetType():GetStartingSpeed2LuaAddress(),"")
	l_DebugHelper:RegisterVect3fParameter("Starting Acceleration 1",l_ParticleSystem:GetType():GetStartingAcceleration1LuaAddress(),"")
	l_DebugHelper:RegisterVect3fParameter("Starting Acceleration 2",l_ParticleSystem:GetType():GetStartingAcceleration2LuaAddress(),"")
	l_DebugHelper:RegisterColorParameter("Color1",l_ParticleSystem:GetType():GetColor1LuaAddress(),"")
	l_DebugHelper:RegisterColorParameter("Color2",l_ParticleSystem:GetType():GetColor2LuaAddress(),"")

	l_DebugHelper:RegisterButton("Export[XML]","WriteParticleInfoToXml('"..l_Type:GetName().."')")
	
end

function OpenSizeControlPoints(ParticleSystemType)
	ClickOnElement("Size Control Points")
	local l_ControlPointsSize=ParticleSystemType:GetLUAControlPointsSize()
	for l_ControlPointSize in l_ControlPointsSize do
		-- g_DebugHelper:RegisterVect2fParameter("Size",l_ControlPointSize:GetSizeLuaAddress(),"")
		-- g_DebugHelper:RegisterVect2fParameter("Time",l_ControlPointSize:GetTimeLuaAddress(),"")
		l_DebugHelper:RegisterExtendedButton("Add C.P","AddControlPointSize",ParticleSystemType,"partycle_system_type")
	end
end

function AddControlPointSize(ParticleSystemType)
	ParticleSystemType:AddControlPointSize(Vect2f(0.0,0.0),Vect2f(0.0,0.0))
	OpenSizeControlPoints(ParticleSystemType)
end

function OpenColorControlPoints(ParticleSystemType)
	ClickOnElement("Color Control Points")
	local l_ControlPointsColor = ParticleSystemType:GetLUAControlPointsColor()
	for l_ControlPointColor in l_ControlPointsColor do
		-- g_DebugHelper:RegisterVect2fParameter("Size",l_ControlPointSize:GetSizeLuaAddress(),"")
		-- g_DebugHelper:RegisterVect2fParameter("Time",l_ControlPointSize:GetTimeLuaAddress(),"")
		-- g_DebugHelper:RegisterExtendedButton("Add C.P","AddControlPointColor",ParticleSystemType,"partycle_system_type")
	end
end

function AddControlPointColor(ParticleSystemType)
	ParticleSystemType:AddControlPointColor(Vect2f(0.0,0.0),CColor(0.0,0.0,0.0,1.0),CColor(0.0,0.0,0.0,1.0))
	OpenColorControlPoints(ParticleSystemType)
end

function WriteParticleInfoToXml(Type)
	
	local Filename = "Data/Level"..g_CurrentLevel.."/particles_systems_WRITTING.xml"
	--local Filename = "Data/Level1/particles_systems.xml"
	
	local l_Writer = CTTODXMLWriter()
	l_Writer:StartFile(Filename)
	l_Writer:StartElement("particle_systems", false)
	local l_ParticleSystems=g_ParticleSystemManager:GetLUAParticles()
	for l_ParticleSystem in l_ParticleSystems do
		WriteParticleSystemInfo(l_Writer, l_ParticleSystem)
	end
	l_Writer:EndElement()
	l_Writer:EndFile()
	
	-- local l_XMLTreeNode=CXMLTreeNode()
	-- local l_Created = l_XMLTreeNode:StartNewFile(Filename)
		-- if l_Created then
			-- l_XMLTreeNode:StartElement("particle_systems")
				-- local l_ParticleSystems=g_ParticleSystemManager:GetLUAParticles()
				-- for l_ParticleSystem in l_ParticleSystems do
					-- WriteParticleSystemInfo(l_XMLTreeNode, l_ParticleSystem)
				-- end
			-- l_XMLTreeNode:EndElement()
		-- l_XMLTreeNode:EndNewFile()
		-- else
			-- print("File '"..Filename.."'not correctly loaded")
		-- end
end

function WriteParticleSystemInfo(Writer, l_Type)
	Writer:StartElement("particle_system", true)
		Writer:WriteIntProperty("num_frames", l_Type.m_NumFrames)
		Writer:WriteFloatProperty("time_per_frame", l_Type.m_TimerPerFrame)
		Writer:WriteBoolProperty("loop_frames", l_Type.m_LoopFrames)
		Writer:WriteBoolProperty("emit_absolute", l_Type.m_EmitAbsolute)
			
		Writer:WriteFloatProperty("starting_dir_angle", l_Type.m_StartingDirectionAngle)
		Writer:WriteFloatProperty("starting_acc_angle", l_Type.m_StartingAccelerationAngle)
		Writer:WriteVect2fProperty("size", l_Type.m_Size)
			
		Writer:WriteVect2fProperty("emit_rate", l_Type.m_EmitRate)
		Writer:WriteVect2fProperty("awake_time", l_Type.m_AwakeTime)
		Writer:WriteVect2fProperty("sleep_time", l_Type.m_SleepTime)
		Writer:WriteVect2fProperty("life", l_Type.m_Life)
			
		Writer:WriteVect2fProperty("starting_angle", l_Type.m_StartingAngle)
		Writer:WriteVect2fProperty("starting_ang_speed", l_Type.m_StartingAngularSpeed)
		Writer:WriteVect2fProperty("angular_acc", l_Type.m_AngularAcceleration)
		
		Writer:WriteVect3fProperty("starting_speed_1", l_Type.m_StartingSpeed1)
		Writer:WriteVect3fProperty("starting_speed_2", l_Type.m_StartingSpeed2)
		Writer:WriteVect3fProperty("starting_acc_1", l_Type.m_StartingAcceleration1)
		Writer:WriteVect3fProperty("starting_acc_2", l_Type.m_StartingAcceleration2)
		
		local l_Color1 = Vect4f(l_Type.m_Color1:GetArgb())
		local l_Color2 = Vect4f(l_Type.m_Color2:GetArgb())
		Writer:WriteVect4fProperty("color1", l_Color1)
		Writer:WriteVect4fProperty("color2", l_Color2)
	Writer:EndElement()	
end

-- function WriteParticleInfoToXml(Type)
	
	-- --local Filename = "Data\Level"..g_CurrentLevel.."\particles_systems.xml"
	-- local l_Type = g_ParticleSystemManager:GetResource(Type)
	-- local Filename = "Data/Level1/particles_systems.xml"
	-- local l_XMLTreeNode=CXMLTreeNode()
	-- local l_Loaded=l_XMLTreeNode:LoadFile(Filename)
	-- if l_Loaded then
	-- local l_Write = CXMLTreeNode()
	-- local l_CanWrite = l_Write:StartNewFile(Filename)
		-- g_LogManager:Log("Loaded")
		-- for i=0, l_XMLTreeNode:GetNumChildren() do
			-- g_LogManager:Log("child")
			-- local l_Element=l_XMLTreeNode:GetChild(i)
			-- if l_Element:GetName() == "particle_system" and l_Element:GetPszProperty("name", "", false) == Type then
				-- g_LogManager:Log("child")
				-- l_Element:WriteIntProperty("num_frames", l_Type.m_NumFrames)
				-- --l_Element:WriteBoolProperty("loop_frames", l_Type:GetLoopLuaAddress())
			-- end
		-- end
	-- else
		-- print("File '"..Filename.."'not correctly loaded")
	-- end
-- end