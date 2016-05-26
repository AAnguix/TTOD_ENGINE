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

	l_DebugHelper:RegisterButton("SizeCP","OpenSizeCP('"..l_Type:GetName().."')")
	l_DebugHelper:RegisterButton("ColorCP","OpenColorCP('"..l_Type:GetName().."')")
	-- l_DebugHelper:RegisterExtendedButton("ColorCP","OpenColorCP",l_Type:GetThisLuaAddress(),"partycle")
	
	l_DebugHelper:RegisterButton("Export[XML]","WriteParticleInfoToXml('"..l_Type:GetName().."')")
	
end

function OpenSizeCP(ParticleSystemTypeName)
	ClickOnElement("Size Control Points")
	local l_ParticleSystemType = g_ParticleSystemManager:GetResource(ParticleSystemTypeName)
	local l_ControlPointsSize=l_ParticleSystemType:GetLUAControlPointsSizeSize()	
	
	for i=0,l_ControlPointsSize-1 do
		g_DebugHelper:RegisterVect2fParameter(i..". Size_",l_ParticleSystemType:GetControlPointSizeSizeLuaAddress(i),"")
		g_DebugHelper:RegisterVect2fParameter(i..". Time_",l_ParticleSystemType:GetControlPointSizeTimeLuaAddress(i),"")
	end
	g_DebugHelper:RegisterExtendedButton("Add","AddControlPointSize",l_ParticleSystemType:GetThisLuaAddress(),"partycle")
end

function AddControlPointSize(ParticleSystemType)
	ParticleSystemType:AddControlPointSize(Vect2f(0.0,0.0),Vect2f(0.0,0.0))
	OpenSizeCP(ParticleSystemType:GetName())
end

function OpenColorCP(ParticleSystemTypeName)
	ClickOnElement("Color Control Points")
	
	local l_ParticleSystemType = g_ParticleSystemManager:GetResource(ParticleSystemTypeName)
	local l_ControlPointsColor = l_ParticleSystemType:GetLUAControlPointsColorSize()
	
	for i=0,l_ControlPointsColor-1 do
		g_DebugHelper:RegisterVect4fParameter(i..". Color1",l_ParticleSystemType:GetControlPointColorColor1LuaAddress(i),"")
		g_DebugHelper:RegisterVect4fParameter(i..". Color2",l_ParticleSystemType:GetControlPointColorColor2LuaAddress(i),"")
		g_DebugHelper:RegisterVect2fParameter(i..". Time",l_ParticleSystemType:GetControlPointColorTimeLuaAddress(i),"")
		i = i+1
	end
	g_DebugHelper:RegisterExtendedButton("Add","AddControlPointColor",l_ParticleSystemType:GetThisLuaAddress(),"partycle")
end

function AddControlPointColor(ParticleSystemType)
	ParticleSystemType:AddControlPointColor(Vect2f(0.0,0.0),CColor(0.0,0.0,0.0,1.0),CColor(0.0,0.0,0.0,1.0))
	OpenColorCP(ParticleSystemType)
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
		
		Writer:WriteStringProperty("name", l_Type:GetName())
		Writer:WriteStringProperty("material", l_Type:GetMaterial():GetName())
	
		-- if g_ParticleSystemManager:GetDefaultType():GetName() == l_Type:GetName() then
			-- Writer:WriteStringProperty("default", "true")
		-- else
			-- Writer:WriteStringProperty("default", "false")
		-- end
	
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
		
		WriteControlPoints(Writer, l_Type)
		
	Writer:EndElement()	
end

function WriteControlPoints(Writer,PartycleSystemType)

	local l_ControlPointsSize = PartycleSystemType:GetLUAControlPointsSize()
	local l_ControlPointsColor = PartycleSystemType:GetLUAControlPointsColor()

	for l_CPS in l_ControlPointsSize do
		Writer:StartElement("control_point_size", true)
			Writer:WriteVect2fProperty("size", l_CPS.m_Size)
			Writer:WriteVect2fProperty("time", l_CPS.m_Time)
		Writer:EndElement()	
	end
	
	for l_CPC in l_ControlPointsColor do
		Writer:StartElement("control_point_color", true)
			Writer:WriteVect4fProperty("color1", l_CPC.m_Color1)
			Writer:WriteVect4fProperty("color2", l_CPC.m_Color2)
			Writer:WriteVect2fProperty("time", l_CPC.m_Time)
		Writer:EndElement()
	end	
end