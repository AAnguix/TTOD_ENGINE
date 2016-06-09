function AddBackButton()
	g_DebugHelper:RegisterButton("..","BackButtonPressed()")
end

function BackButtonPressed()
	g_DebugHelper:RemoveBar()
	g_DebugHelper:ResetButtons()
	InitializeDebugBar()
end

function ClickOnElement(NewBarName)
	g_DebugHelper:RemoveBar()
	g_DebugHelper:ResetButtons()
	g_DebugHelper:CreateBar(NewBarName)
	AddBackButton()
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
	ClickOnElement("Effects")
	local l_Effects=g_EffectManager:GetLUAEffects()
	for l_Effect in l_Effects do
		g_DebugHelper:RegisterButton(l_Effect:GetName(),"ReloadEffectTechnique('"..l_Effect:GetName().."')")
	end
end

function OpenRenderableObjectTechniques()
	ClickOnElement("ROTechniques")
end

------------------------------ MATERIALS -------------------------------------------------------------------------
dofile("./Data/Scripting/AntTweakBarMaterials.lua")

------------------------------ CAMERAS -------------------------------------------------------------------------
dofile("./Data/Scripting/AntTweakBarCameras.lua")

------------------------------ ENGINE -------------------------------------------------------------------------
dofile("./Data/Scripting/AntTweakBarEngine.lua")

function OpenAnimatedModels()
	ClickOnElement("Animated Models")
	local l_Models=g_AnimatedModelManager:GetLUAAnimatedModels()
	for l_Model in l_Models do
		g_DebugHelper:RegisterButton(l_Model:GetName(),"")
	end
end

function OpenStaticMeshes()
	ClickOnElement("Static Meshes")
	local l_SMeshes=g_StaticMeshManager:GetLUAStaticMeshes()
	for l_SMesh in l_SMeshes do
		g_DebugHelper:RegisterExtendedButton(l_SMesh:GetName(),"ReloadStaticMesh",l_SMesh:GetThisLuaAddress(),"static_mesh")
	end
end

function ReloadStaticMesh(CStaticMesh) CStaticMesh:Reload() end

function OpenLayers()
	ClickOnElement("Layers")
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
	ClickOnElement(ROManagerName)
	g_DebugHelper:RegisterButton("Reload","ReloadRenderableObjectManager('"..ROManagerName.."')")
	local l_ROManager=g_LayerManager:GetResource(ROManagerName)	
	local l_RObjects = l_ROManager:GetResourcesVector():size()
	for i=0,l_RObjects-1 do
		local l_RObject=l_ROManager:GetResourceById(i)
		AddRenderableObjectButton(l_RObject)
	end
end

function AddRenderableObjectButton(RenderableObject)
	local l_DebugHelper=CEngine.GetSingleton():GetDebugHelper()
	local l_Name = RenderableObject:GetName()
	g_DebugHelper:RegisterExtendedButton(l_Name,"OpenRenderableObject",RenderableObject:GetThisLuaAddress(),"renderable_object")
end

function ReloadRenderableObjectManager(Name)
	--CEngine.GetSingleton():GetLayerManager():GetResource(Name):Reload()
end

function OpenRenderableObject(RenderableObject)
	ClickOnElement(RenderableObject:GetName())
	g_DebugHelper:RegisterFloatParameter("Yaw", RenderableObject:GetYawLuaAddress(),"step=0.01") --min=-3.14 max=3.14 step=0.01
	g_DebugHelper:RegisterFloatParameter("Pitch", RenderableObject:GetPitchLuaAddress(),"step=0.01")
	g_DebugHelper:RegisterFloatParameter("Roll", RenderableObject:GetRollLuaAddress(),"step=0.01")
	g_DebugHelper:RegisterVect3fParameter("Position", RenderableObject:GetPositionLuaAddress(),"")
	g_DebugHelper:RegisterVect3fParameter("Scale", RenderableObject:GetScaleLuaAddress(),"")
end

------------------------------ LIGHTS -------------------------------------------------------------------------
dofile("./Data/Scripting/AntTweakBarLights.lua")
 
 ------------------------------ SCRIPTS -------------------------------------------------------------------------
dofile("./Data/Scripting/AntTweakBarScripts.lua")
 
 function OpenSceneRendererCommands()
	ClickOnElement("Commands")
	local l_SceneRendererCommands = g_SceneRendererCommandManager:GetLUASceneRendererCommands()
	local l_i = 1
	for l_SRCommand in l_SceneRendererCommands do
		g_DebugHelper:RegisterExtendedButton(l_i..". "..l_SRCommand:GetName(),"OpenSceneRendererCommand",l_SRCommand:GetThisLuaAddress(),"scene_renderer_command")
		l_i = l_i+1
	end
 end
 
 function OpenSceneRendererCommand(SceneRendererCommand)
	ClickOnElement(SceneRendererCommand:GetName())
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