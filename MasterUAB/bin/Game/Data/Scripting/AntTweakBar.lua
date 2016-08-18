function AddBackButton(ReturnFunctionCall)
	g_DebugHelper:RegisterButton("..","BackButtonPressed('"..ReturnFunctionCall.."')")
end

function BackButtonPressed(ReturnFunctionCall)
	g_DebugHelper:RemoveBar()
	g_DebugHelper:ResetButtons()
	g_LuabindManager:RunCode(ReturnFunctionCall)
end

function ClickOnElement(NewBarName, ReturnFunctionCall)
	g_DebugHelper:RemoveBar()
	g_DebugHelper:ResetButtons()
	g_DebugHelper:CreateBar(NewBarName)
	AddBackButton(ReturnFunctionCall)
end

function InitializeDebugBar()
	g_DebugHelper:CreateBar("Debug Bar")
	g_DebugHelper:RegisterButton("[1]Scene","ReloadScene()")
	g_DebugHelper:RegisterButton("[2]Effects","OpenEffects()")
	g_DebugHelper:RegisterButton("[3]AnimatedModels","OpenAnimatedModels()")
	g_DebugHelper:RegisterButton("[4]StaticMeshes","OpenStaticMeshes()")
	g_DebugHelper:RegisterButton("[5]Layers","OpenLayers()")
	g_DebugHelper:RegisterButton("[6]Lights","OpenLights()")
	g_DebugHelper:RegisterButton("[7]Materials","OpenMaterials()")
	g_DebugHelper:RegisterButton("[8]Commands","OpenSceneRendererCommands()")
	g_DebugHelper:RegisterButton("[9]GUI","ReloadGUI()")
	g_DebugHelper:RegisterButton("Particles","OpenParticles()")
	g_DebugHelper:RegisterButton("Cameras","OpenCameras()")
	g_DebugHelper:RegisterButton("Scripts","OpenScripts()")
	g_DebugHelper:RegisterButton("Engine","ShowEngineParameters()")
end

function OpenEffects()
	ClickOnElement("Effects","InitializeDebugBar()")
	local l_Effects=g_EffectManager:GetLUAEffects()
	for l_Effect in l_Effects do
		g_DebugHelper:RegisterButton(l_Effect:GetName(),"g_AntTweakbarReloader:ReloadEffectTechnique('"..l_Effect:GetName().."')")
	end
end

function OpenRenderableObjectTechniques()
	ClickOnElement("ROTechniques","InitializeDebugBar()")
end

------------------------------ MATERIALS -------------------------------------------------------------------------
dofile("./Data/Scripting/AntTweakBarMaterials.lua")

------------------------------ CAMERAS -------------------------------------------------------------------------
dofile("./Data/Scripting/AntTweakBarCameras.lua")

------------------------------ ENGINE -------------------------------------------------------------------------
dofile("./Data/Scripting/AntTweakBarEngine.lua")

function OpenAnimatedModels()
	ClickOnElement("Animated Models","InitializeDebugBar()")
	local l_Models=g_AnimatedModelManager:GetLUAAnimatedModels()
	for l_Model in l_Models do
		g_DebugHelper:RegisterButton(l_Model:GetName(),"")
	end
end

function OpenStaticMeshes()
	ClickOnElement("Static Meshes","InitializeDebugBar()")
	local l_SMeshes=g_StaticMeshManager:GetLUAStaticMeshes()
	for l_SMesh in l_SMeshes do
		g_DebugHelper:RegisterExtendedButton(l_SMesh:GetName(),"g_AntTweakbarReloader:ReloadStaticMesh",l_SMesh:GetThisLuaAddress(),"static_mesh")
	end
end

function OpenLayers()
	ClickOnElement("Layers","InitializeDebugBar()")
	--CEngine.GetSingleton():GetDebugHelper():RegisterButton("Reload All","ReloadLayers()")
	local l_ROManagers = g_LayerManager:GetResourcesVector():size()
	for i=0,l_ROManagers-1 do
		local l_ROMan=g_LayerManager:GetResourceById(i)
		AddROManagerButton(l_ROMan)
	end
end

function AddROManagerButton(ROManager)
	g_DebugHelper:RegisterButton(ROManager:GetName(),"OpenRenderableObjectManager('"..ROManager:GetName().."')")
end

function OpenRenderableObjectManager(ROManagerName)
	ClickOnElement(ROManagerName, "OpenLayers()")
	g_DebugHelper:RegisterButton("Reload","ReloadRenderableObjectManager('"..ROManagerName.."')")
	local l_ROManager=g_LayerManager:GetResource(ROManagerName)

	if(l_ROManager:GetName()=="particles") then
		l_ParticlesSize = l_ROManager:GetResourcesVector():size()
		local l_ParticleSystemTypes = g_ParticleSystemManager:GetLUAParticles()
		for l_ParticleSystemType in l_ParticleSystemTypes do
			local l_ParticleSystemTypeName = l_ParticleSystemType:GetName()
			local l_ParticleSystemInstanceName = l_ParticleSystemTypeName.."_particle_"..l_ParticlesSize
			g_DebugHelper:RegisterButton("Add Particle ["..l_ParticleSystemTypeName.."]","AddParticleSystem('"..l_ParticleSystemInstanceName.."' ,'"..l_ParticleSystemTypeName.."')","group=\"Add Particles\"")
		end
	end
	local l_RObjects = l_ROManager:GetResourcesVector():size()
	for i=0,l_RObjects-1 do
		local l_RObject=l_ROManager:GetResourceById(i)
		AddRenderableObjectButton(l_RObject)
	end
end

function AddParticleSystem(ParticleSystemInstanceName, ParticleSystemTypeName)
	local l_LayerManager = CEngine.GetSingleton():GetLayerManager()
	l_LayerManager:AddParticleSystemInstance("particles", ParticleSystemTypeName, ParticleSystemInstanceName, Vect3f(0.0,0.0,0.0), 0.0, 0.0, 0.0)
	OpenRenderableObjectManager("particles")
end


function AddRenderableObjectButton(RenderableObject)
	local l_DebugHelper=CEngine.GetSingleton():GetDebugHelper()
	local l_Name = RenderableObject:GetName()
	g_DebugHelper:RegisterExtendedButton(l_Name,"OpenRenderableObject",RenderableObject:GetThisLuaAddress(),"renderable_object", "group=\"Renderable Object\"")
end

function ReloadRenderableObjectManager(Name)
	--CEngine.GetSingleton():GetLayerManager():GetResource(Name):Reload()
end

function OpenRenderableObject(RenderableObject)
	ClickOnElement(RenderableObject:GetName(),"OpenLayers()")
	g_DebugHelper:RegisterBoolParameter("Active", RenderableObject:GetEnabledLuaAddress(),"")
	g_DebugHelper:RegisterFloatParameter("Yaw", RenderableObject:GetYawLuaAddress(),"step=0.01") --min=-3.14 max=3.14 step=0.01
	g_DebugHelper:RegisterFloatParameter("Pitch", RenderableObject:GetPitchLuaAddress(),"step=0.01")
	g_DebugHelper:RegisterFloatParameter("Roll", RenderableObject:GetRollLuaAddress(),"step=0.01")
	g_DebugHelper:RegisterFloatParameter("Position.x", RenderableObject:GetPositionLuaAddress(0),"step=0.01 group=\"Position\"")
	g_DebugHelper:RegisterFloatParameter("Position.y", RenderableObject:GetPositionLuaAddress(1),"step=0.01 group=\"Position\"")
	g_DebugHelper:RegisterFloatParameter("Position.z", RenderableObject:GetPositionLuaAddress(2),"step=0.01 group=\"Position\"")
	g_DebugHelper:RegisterFloatParameter("Scale.x", RenderableObject:GetScaleLuaAddress(0),"step=0.01 group=\"Scale\"")
	g_DebugHelper:RegisterFloatParameter("Scale.y", RenderableObject:GetScaleLuaAddress(1),"step=0.01 group=\"Scale\"")
	g_DebugHelper:RegisterFloatParameter("Scale.z", RenderableObject:GetScaleLuaAddress(2),"step=0.01 group=\"Scale\"")

	if RenderableObject:GetClassType() == CRenderableObject.PARTICLE_EMITER then
		g_DebugHelper:RegisterFloatParameter("EmissionBoxHalfSize.x",RenderableObject:GetEmissionBoxHalfSizeLuaAddress(0),"step=0.01 group=\"Emission Box Half Size\"")
		g_DebugHelper:RegisterFloatParameter("EmissionBoxHalfSize.y",RenderableObject:GetEmissionBoxHalfSizeLuaAddress(1),"step=0.01 group=\"Emission Box Half Size\"")
		g_DebugHelper:RegisterFloatParameter("EmissionBoxHalfSize.z",RenderableObject:GetEmissionBoxHalfSizeLuaAddress(2),"step=0.01 group=\"Emission Box Half Size\"")
	end
end

------------------------------ LIGHTS -------------------------------------------------------------------------
dofile("./Data/Scripting/AntTweakBarLights.lua")
 
 ------------------------------ SCRIPTS -------------------------------------------------------------------------
dofile("./Data/Scripting/AntTweakBarScripts.lua")
 
 function OpenSceneRendererCommands()
	ClickOnElement("Commands","InitializeDebugBar()")
	local l_SceneRendererCommands = g_SceneRendererCommandManager:GetLUASceneRendererCommands()
	local l_i = 1
	for l_SRCommand in l_SceneRendererCommands do
		g_DebugHelper:RegisterExtendedButton(l_i..". "..l_SRCommand:GetName(),"OpenSceneRendererCommand",l_SRCommand:GetThisLuaAddress(),"scene_renderer_command")
		l_i = l_i+1
	end
 end
 
 function OpenSceneRendererCommand(SceneRendererCommand)
	ClickOnElement(SceneRendererCommand:GetName(),"OpenSceneRendererCommands()")
	g_DebugHelper:RegisterBoolParameter("Active", SceneRendererCommand:GetActiveLuaAddress(),"")
end

------------------------------ PARTICLES -------------------------------------------------------------------------
dofile("./Data/Scripting/AntTweakBarParticles.lua")

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