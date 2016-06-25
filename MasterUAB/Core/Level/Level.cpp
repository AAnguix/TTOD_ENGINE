#include "Level\Level.h"
#include "Engine.h"
#include "Utils\FileUtils.h"
#include "Materials\MaterialManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "Particles\ParticleManager.h"
#include "Animation\AnimatedModelManager.h"
#include "RenderableObjects\LayerManager.h"
#include "Lights\LightManager.h"
#include "Render\RenderManager.h"
#include "Render\DebugRender.h"
#include "ISoundManager.h"
#include "ScriptManager.h"
#include "PhysXManager.h"
#include "SceneRendererCommands\SceneRendererCommandManager.h"
#include "Animation\AnimatorControllerManager.h"
#include "GUIManager.h"
#include "Utils\GameObjectManager.h"
#include "Profiler\Profiler.h"

CLevel::CLevel(const std::string &ID)
:m_ID(ID)
,m_MaterialsFilename("./Data/Level" + m_ID + "/materials.xml")
,m_StaticMeshesFilename("./Data/Level" + m_ID + "/static_meshes.xml")
,m_ParticleSystemsFilename("./Data/Level" + m_ID + "/particles_systems.xml")
,m_AnimatedModelsFilename("./Data/Level" + m_ID + "/animated_models.xml")
,m_RenderableObjectsFilename("./Data/Level" + m_ID + "/renderable_objects.xml")
,m_LightsFilename("./Data/Level" + m_ID + "/lights.xml")
,m_SoundBankFilename("./Data/Level" + m_ID + "/Audio/Soundbanks/")
,m_SpeakersFilename("./Data/Level" + m_ID + "/Audio/speakers.xml")
,m_SceneRendererCommandsFilename("./Data/Level" + m_ID + "/scene_renderer_commands.xml")
{

}

CLevel::~CLevel()
{

}

bool CLevel::Load(CEngine& Engine)
{
	bool l_Result = true;
	
	CFileUtils::CheckPhysxFolders(m_ID);

	Engine.GetProfiler()->Begin("LoadMaterials");
		Engine.GetMaterialManager()->Load(m_MaterialsFilename);
	Engine.GetProfiler()->End("LoadMaterials");

	Engine.GetProfiler()->Begin("LoadStaticMeshes");
		Engine.GetStaticMeshManager()->Load(m_StaticMeshesFilename);
	Engine.GetProfiler()->End("LoadStaticMeshes");
	
	Engine.GetProfiler()->Begin("LoadParticleSystems");
		Engine.GetParticleSystemManager()->Load(m_ParticleSystemsFilename);
	Engine.GetProfiler()->End("LoadParticleSystems");

	Engine.GetProfiler()->Begin("LoadAnimatedModels");
		Engine.GetAnimatedModelManager()->Load(m_AnimatedModelsFilename);
	Engine.GetProfiler()->End("LoadAnimatedModels");
	
	Engine.GetProfiler()->Begin("LoadRenderableObjects");
		Engine.GetLayerManager()->Load(m_RenderableObjectsFilename);
	Engine.GetProfiler()->End("LoadRenderableObjects");

	Engine.GetProfiler()->Begin("LoadLights");
		Engine.GetLightManager()->Load(m_LightsFilename);
	Engine.GetProfiler()->End("LoadLights");

	Engine.GetProfiler()->Begin("InitializeDebugLights");
		Engine.GetRenderManager()->GetDebugRender()->InitializeDebugLights();
	Engine.GetProfiler()->End("InitializeDebugLights");

	Engine.GetProfiler()->Begin("LoadSoundBanks");
	ISoundManager* l_SoundManager = Engine.GetSoundManager();
	l_SoundManager->SetPath(m_SoundBankFilename);
	l_SoundManager->Load("SoundbanksInfo.xml", m_SpeakersFilename);
	Engine.GetProfiler()->End("LoadSoundBanks");

	Engine.GetProfiler()->Begin("LoadSceneRendererCommands");
		Engine.GetSceneRendererCommandManager()->Load(m_SceneRendererCommandsFilename);
	Engine.GetProfiler()->End("LoadSceneRendererCommands");

	return l_Result;
}

bool CLevel::Unload(CEngine& Engine)
{
	bool l_Result = true;

	Engine.GetScriptManager()->RemoveComponents();
	Engine.GetPhysXManager()->Destroy();
	Engine.GetSoundManager()->RemoveComponents();
	Engine.GetSoundManager()->UnloadSoundBank(m_SoundBankFilename);
	Engine.GetAnimatorControllerManager()->RemoveComponents();
	Engine.GetMaterialManager()->Destroy();

	Engine.GetStaticMeshManager()->Destroy();
	Engine.GetParticleSystemManager()->Destroy();
	Engine.GetAnimatedModelManager()->Destroy();

	//Engine.GetGUIManager()->Destroy();
	Engine.GetLayerManager()->Destroy();
	Engine.GetLightManager()->Destroy();
	Engine.GetGameObjectManager()->Destroy();
	
	Engine.GetSceneRendererCommandManager()->Destroy();

	return l_Result;
}