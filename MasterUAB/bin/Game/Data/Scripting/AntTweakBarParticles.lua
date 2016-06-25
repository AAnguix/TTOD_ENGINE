------------------------------ PARTICLES -------------------------------------------------------------------------

function OpenParticles()
	ClickOnElement("Particle Emitters Types","InitializeDebugBar()")
	g_DebugHelper:RegisterButton("Reload","ReloadParticleSystems()")
	local l_ParticleSystemTypes = g_ParticleSystemManager:GetLUAParticles()
	for l_ParticleSystemType in l_ParticleSystemTypes do
		--g_DebugHelper:RegisterExtendedButton(l_ParticleSystemType:GetName(),"OpenParticle",l_ParticleSystemType:GetThisLuaAddress(),"particle")
		g_DebugHelper:RegisterButton(l_ParticleSystemType:GetName(),"OpenParticle('"..l_ParticleSystemType:GetName().."')")
	end
end

function ReloadParticleSystems()
	-- g_MaterialManager:Reload()
	g_ParticleSystemManager:Reload()
end

function OpenParticle(ParticleSystemTypeName)
	ParticleSystemType = g_ParticleSystemManager:GetResource(ParticleSystemTypeName)
	ClickOnElement(ParticleSystemTypeName,"OpenParticles()")
	local l_DebugHelper=CEngine.GetSingleton():GetDebugHelper()
	local l_Material = ParticleSystemType:GetMaterial()

	local l_FunctionToCall = "OpenMaterial('"..l_Material:GetName().."','OpenParticles()')"
	g_DebugHelper:RegisterButton(l_Material:GetName(),l_FunctionToCall)
	l_DebugHelper:RegisterButton("Export[XML]","WriteParticleInfoToXml()")
	
	l_DebugHelper:RegisterInt32Parameter("Num.Frames",ParticleSystemType:GetNumFramesLuaAddress(),"min=1 max=100 step=1")
	l_DebugHelper:RegisterFloatParameter("Time/frame",ParticleSystemType:GetTimePerFrameLuaAddress(),"min=0.0 max=5.0 step=0.01")
	l_DebugHelper:RegisterBoolParameter("Loop",ParticleSystemType:GetLoopLuaAddress(),"")
	l_DebugHelper:RegisterBoolParameter("Emit absolute",ParticleSystemType:GetEmitAbsoluteLuaAddress(),"")
	
	l_DebugHelper:RegisterFloatParameter("Starting direction angle",ParticleSystemType:GetStartingDirectionAngleLuaAddress(),"min=0 max=100 step=1")
	l_DebugHelper:RegisterFloatParameter("Starting acceleration angle ",ParticleSystemType:GetStartingAccelerationAngleLuaAddress(),"min=0 max=100 step=1")
	
	l_DebugHelper:RegisterFloatParameter("Size.x",ParticleSystemType:GetSizeLuaAddress(0),"step=0.01 group=\"Size\"")
	l_DebugHelper:RegisterFloatParameter("Size.y",ParticleSystemType:GetSizeLuaAddress(1),"step=0.01 group=\"Size\"")
	
	l_DebugHelper:RegisterFloatParameter("EmitRate.x",ParticleSystemType:GetEmitRateLuaAddress(0),"step=0.01 group=\"Emit rate\"")
	l_DebugHelper:RegisterFloatParameter("EmitRate.y",ParticleSystemType:GetEmitRateLuaAddress(1),"step=0.01 group=\"Emit rate\"")
	
	l_DebugHelper:RegisterFloatParameter("AwakeTime.x",ParticleSystemType:GetAwakeTimeLuaAddress(0),"step=0.01 group=\"Awake time\"")
	l_DebugHelper:RegisterFloatParameter("AwakeTime.y",ParticleSystemType:GetAwakeTimeLuaAddress(1),"step=0.01 group=\"Awake time\"")
	
	l_DebugHelper:RegisterFloatParameter("SleepTime.x",ParticleSystemType:GetSleepTimeLuaAddress(0),"step=0.01 group=\"Sleep time\"")
	l_DebugHelper:RegisterFloatParameter("SleepTime.y",ParticleSystemType:GetSleepTimeLuaAddress(1),"step=0.01 group=\"Sleep time\"")
	
	l_DebugHelper:RegisterFloatParameter("Life.x",ParticleSystemType:GetLifeLuaAddress(0),"step=0.01 group=\"Life\"")
	l_DebugHelper:RegisterFloatParameter("Life.y",ParticleSystemType:GetLifeLuaAddress(1),"step=0.01 group=\"Life\"")
	
	l_DebugHelper:RegisterFloatParameter("StartingAngle.x",ParticleSystemType:GetStartingAngleLuaAddress(0),"step=0.01 group=\"Starting Angle\"")
	l_DebugHelper:RegisterFloatParameter("StartingAngle.y",ParticleSystemType:GetStartingAngleLuaAddress(1),"step=0.01 group=\"Starting Angle\"")
	
	l_DebugHelper:RegisterFloatParameter("StartingAngularSpeed.x",ParticleSystemType:GetStartingAngularSpeedLuaAddress(0),"step=0.01 group=\"Starting Angular Speed\"")
	l_DebugHelper:RegisterFloatParameter("StartingAngularSpeed.y",ParticleSystemType:GetStartingAngularSpeedLuaAddress(1),"step=0.01 group=\"Starting Angular Speed\"")
	
	l_DebugHelper:RegisterFloatParameter("AngularAcceleration.x",ParticleSystemType:GetAngularAccelerationLuaAddress(0),"step=0.01 group=\"Angular Acceleration\"")
	l_DebugHelper:RegisterFloatParameter("AngularAcceleration.y",ParticleSystemType:GetAngularAccelerationLuaAddress(1),"step=0.01 group=\"Angular Acceleration\"")
	
	l_DebugHelper:RegisterFloatParameter("StartingSpeed1.x",ParticleSystemType:GetStartingSpeed1LuaAddress(0),"step=0.01 group=\"Starting Speed 1\"")
	l_DebugHelper:RegisterFloatParameter("StartingSpeed1.y",ParticleSystemType:GetStartingSpeed1LuaAddress(1),"step=0.01 group=\"Starting Speed 1\"")
	l_DebugHelper:RegisterFloatParameter("StartingSpeed1.z",ParticleSystemType:GetStartingSpeed1LuaAddress(2),"step=0.01 group=\"Starting Speed 1\"")

	l_DebugHelper:RegisterFloatParameter("StartingSpeed2.x",ParticleSystemType:GetStartingSpeed2LuaAddress(0),"step=0.01 group=\"Starting Speed 2\"")
	l_DebugHelper:RegisterFloatParameter("StartingSpeed2.y",ParticleSystemType:GetStartingSpeed2LuaAddress(1),"step=0.01 group=\"Starting Speed 2\"")
	l_DebugHelper:RegisterFloatParameter("StartingSpeed2.z",ParticleSystemType:GetStartingSpeed2LuaAddress(2),"step=0.01 group=\"Starting Speed 2\"")
	
	l_DebugHelper:RegisterFloatParameter("StartingAcceleration1.x",ParticleSystemType:GetStartingAcceleration1LuaAddress(0),"step=0.01 group=\"Starting Acceleration 1\"")
	l_DebugHelper:RegisterFloatParameter("StartingAcceleration1.y",ParticleSystemType:GetStartingAcceleration1LuaAddress(1),"step=0.01 group=\"Starting Acceleration 1\"")
	l_DebugHelper:RegisterFloatParameter("StartingAcceleration1.z",ParticleSystemType:GetStartingAcceleration1LuaAddress(2),"step=0.01 group=\"Starting Acceleration 1\"")
	
	l_DebugHelper:RegisterFloatParameter("StartingAcceleration2.x",ParticleSystemType:GetStartingAcceleration2LuaAddress(0),"step=0.01 group=\"Starting Acceleration 2\"")
	l_DebugHelper:RegisterFloatParameter("StartingAcceleration2.y",ParticleSystemType:GetStartingAcceleration2LuaAddress(1),"step=0.01 group=\"Starting Acceleration 2\"")
	l_DebugHelper:RegisterFloatParameter("StartingAcceleration2.z",ParticleSystemType:GetStartingAcceleration2LuaAddress(2),"step=0.01 group=\"Starting Acceleration 2\"")
	
	l_DebugHelper:RegisterColorParameter("Color1",ParticleSystemType:GetColor1LuaAddress(),"")
	l_DebugHelper:RegisterColorParameter("Color2",ParticleSystemType:GetColor2LuaAddress(),"")

	l_DebugHelper:RegisterButton("SizeCP","OpenSizeCP('"..ParticleSystemType:GetName().."')")
	l_DebugHelper:RegisterButton("ColorCP","OpenColorCP('"..ParticleSystemType:GetName().."')")
	
end

function OpenSizeCP(ParticleSystemTypeName)
	ClickOnElement("Size Control Points","OpenParticles()")
	local l_ParticleSystemType = g_ParticleSystemManager:GetResource(ParticleSystemTypeName)
	local l_ControlPointsSize=l_ParticleSystemType:GetLUAControlPointsSizeSize()	
	
	for i=0,l_ControlPointsSize-1 do
		g_DebugHelper:RegisterVect2fParameter(i..". Size_",l_ParticleSystemType:GetControlPointSizeSizeLuaAddress(i),"")
		g_DebugHelper:RegisterVect2fParameter(i..". Time_",l_ParticleSystemType:GetControlPointSizeTimeLuaAddress(i),"")
		g_DebugHelper:RegisterButton(i..". Remove","RemoveControlPointSize('"..l_ParticleSystemType:GetName().."',"..i..")")
	end
	g_DebugHelper:RegisterExtendedButton("Add","AddControlPointSize",l_ParticleSystemType:GetThisLuaAddress(),"particle")
	
end
function OpenColorCP(ParticleSystemTypeName)
	ClickOnElement("Color Control Points","OpenParticles()")
	
	local l_ParticleSystemType = g_ParticleSystemManager:GetResource(ParticleSystemTypeName)
	local l_ControlPointsColor = l_ParticleSystemType:GetLUAControlPointsColorSize()
	
	for i=0,l_ControlPointsColor-1 do
		g_DebugHelper:RegisterColorParameter(i..". Color1",l_ParticleSystemType:GetControlPointColorColor1LuaAddress(i),"")
		g_DebugHelper:RegisterColorParameter(i..". Color2",l_ParticleSystemType:GetControlPointColorColor2LuaAddress(i),"")
		g_DebugHelper:RegisterVect2fParameter(i..". Time",l_ParticleSystemType:GetControlPointColorTimeLuaAddress(i),"")
		g_DebugHelper:RegisterButton(i..". Remove","RemoveControlPointColor('"..l_ParticleSystemType:GetName().."',"..i..")")
	end
	g_DebugHelper:RegisterExtendedButton("Add","AddControlPointColor",l_ParticleSystemType:GetThisLuaAddress(),"particle")
end

function AddControlPointSize(ParticleSystemType)
	ParticleSystemType:AddControlPointSize(Vect2f(0.0,0.0),Vect2f(0.0,0.0))
	OpenSizeCP(ParticleSystemType:GetName())
end
function AddControlPointColor(ParticleSystemType)
	ParticleSystemType:AddControlPointColor(CColor(0.0,0.0,0.0,1.0),CColor(0.0,0.0,0.0,1.0),Vect2f(0.0,0.0))
	OpenColorCP(ParticleSystemType:GetName())
end

function RemoveControlPointSize(ParticleSystemTypeName, Index)
	local l_ParticleSystemType = g_ParticleSystemManager:GetResource(ParticleSystemTypeName)
	local l_ControlPointsSize=l_ParticleSystemType:GetLUAControlPointsSizeSize()	
	if l_ControlPointsSize > 1 then
		l_ParticleSystemType:RemoveControlPointSize(Index)
		OpenSizeCP(ParticleSystemTypeName)
	end
end
function RemoveControlPointColor(ParticleSystemTypeName, Index)
	local l_ParticleSystemType = g_ParticleSystemManager:GetResource(ParticleSystemTypeName)
	local l_ControlPointsColor = l_ParticleSystemType:GetLUAControlPointsColorSize()
	if l_ControlPointsColor > 1 then
		l_ParticleSystemType:RemoveControlPointColor(Index)
		OpenColorCP(ParticleSystemTypeName)
	end
end

function WriteParticleInfoToXml()

	local l_Export = CFileUtils.ShowDialog()
	if l_Export then
		WriteParticleMaterialInfoToXml()
		local Filename = "Data\\Level"..g_Engine:GetCurrentLevel().."\\particles_systems.xml"
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
	end
end

function WriteParticleMaterialInfoToXml()
	local l_MaterialsFilename = "Data/Level"..g_Engine:GetCurrentLevel().."/materials.xml"
	WriteMaterialsVectorToXml(l_MaterialsFilename)
end

function WriteParticleSystemInfo(Writer, l_Type)
	Writer:StartElement("particle_system", true)
		g_LogManager:Log("Writting particle system info")
		Writer:WriteStringProperty("name", l_Type:GetName())
		local l_MaterialName = l_Type:GetMaterial():GetName()
		Writer:WriteStringProperty("material",l_MaterialName)
	
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

function WriteControlPoints(Writer,ParticleSystemType)

	local l_ControlPointsSize = ParticleSystemType:GetLUAControlPointsSize()
	local l_ControlPointsColor = ParticleSystemType:GetLUAControlPointsColor()

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