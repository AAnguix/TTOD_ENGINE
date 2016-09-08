#include "Render\ContextManager.h"
#include "Level\Level.h"
#include "Engine\Engine.h"
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
#include "Components\Script\ScriptManager.h"
#include "PhysXManager.h"
#include "SceneRendererCommands\SceneRendererCommandManager.h"
#include "Components\AnimatorController\AnimatorControllerManager.h"
#include "GUIManager.h"
#include "GameObject\GameObjectManager.h"
#include "GameObject\LuaGameObjectHandleManager.h"
#include "Profiler\Profiler.h"
#include <thread>
#include <direct.h>
#include "RenderableObjects\RenderableObjectTechniqueManager.h"

CLevel::CLevel(const std::string &ID)
:m_ID(ID)
,m_MaterialsFilename("Data/Level" + m_ID + "/materials.xml")
,m_StaticMeshesFilename("./Data/Level" + m_ID + "/static_meshes.xml")
,m_ParticleSystemsFilename("./Data/Level" + m_ID + "/particles_systems.xml")
,m_AnimatedModelsFilename("./Data/Level" + m_ID + "/animated_models.xml")
,m_RenderableObjectsFilename("./Data/Level" + m_ID + "/renderable_objects.xml")
,m_LightsFilename("Data/Level" + m_ID + "/lights.xml")
,m_SoundBankFilename("./Data/Level" + m_ID + "/Audio/Soundbanks/")
,m_SpeakersFilename("./Data/Level" + m_ID + "/Audio/speakers.xml")
,m_SceneRendererCommandsFilename("./Data/Level" + m_ID + "/scene_renderer_commands.xml")
,m_PercentageLoaded(0)
{

}

CLevel::~CLevel()
{

}

//template<class _Fn, class... _Args>
//void FirstThread(_Fn&& _Fx, _Args&&... _Ax)
//{	
//	std::thread l_Thread(_Fx, _Ax);
//}

bool CLevel::Load(CEngine& Engine)
{
	bool l_Result = true;

	/*Engine.GetProfiler()->Begin("LoadLoadScreenSceneRendererCommands");
	Engine.GetSceneRendererCommandManager()->Load("./Data/load_screen_scene_renderer_commands.xml");
	Engine.GetProfiler()->End("LoadLoadScreenSceneRendererCommands");*/

	CEngine::GetSingleton().GetGUIManager()->AddImage("background_image","background_sprite");
	CEngine::GetSingleton().GetGUIManager()->AddSlider("level_percentage_loaded_slider", "level_percentage_loaded_slider_base", "level_percentage_loaded_slider_top", "level_percentage_loaded_slider_handle", "level_percentage_loaded_slider_pressed_handle");
	CGUIManager::SSliderResult l_SliderResult;
	l_SliderResult.real = 0;
	l_SliderResult.temp = 0;
	
	CFileUtils::CheckPhysxFolders(m_ID);

	Engine.GetProfiler()->Begin("LoadMaterials");
		Engine.GetMaterialManager()->Load(m_MaterialsFilename);
	Engine.GetProfiler()->End("LoadMaterials");

	UpdateLoadPercentageLevel(20, l_SliderResult);

	Engine.GetProfiler()->Begin("LoadStaticMeshes");
		Engine.GetStaticMeshManager()->Load(m_StaticMeshesFilename);
	Engine.GetProfiler()->End("LoadStaticMeshes");
	
	UpdateLoadPercentageLevel(40, l_SliderResult);

	Engine.GetProfiler()->Begin("LoadParticleSystems");
		Engine.GetParticleSystemManager()->Load(m_ParticleSystemsFilename);
	Engine.GetProfiler()->End("LoadParticleSystems");

	UpdateLoadPercentageLevel(50, l_SliderResult);

	Engine.GetProfiler()->Begin("LoadAnimatedModels");
		Engine.GetAnimatedModelManager()->Load(m_AnimatedModelsFilename);
	Engine.GetProfiler()->End("LoadAnimatedModels");

	UpdateLoadPercentageLevel(60, l_SliderResult);

	Engine.GetProfiler()->Begin("LoadRenderableObjects");
		Engine.GetLayerManager()->Load(m_RenderableObjectsFilename);
	Engine.GetProfiler()->End("LoadRenderableObjects");

	UpdateLoadPercentageLevel(80, l_SliderResult);

	Engine.GetProfiler()->Begin("LoadLights");
		Engine.GetLightManager()->Load(m_LightsFilename);
	Engine.GetProfiler()->End("LoadLights");

	Engine.GetProfiler()->Begin("InitializeDebugLights");
		Engine.GetRenderManager()->GetDebugRender()->InitializeDebugLights();
	Engine.GetProfiler()->End("InitializeDebugLights");

	UpdateLoadPercentageLevel(90, l_SliderResult);

	Engine.GetProfiler()->Begin("LoadSoundBanks");
	ISoundManager* l_SoundManager = Engine.GetSoundManager();
	l_SoundManager->SetPath(m_SoundBankFilename);
	l_SoundManager->Load("SoundbanksInfo.xml", m_SpeakersFilename);
	Engine.GetProfiler()->End("LoadSoundBanks");

	Engine.GetProfiler()->Begin("LoadSceneRendererCommands");
	Engine.GetSceneRendererCommandManager()->Load(m_SceneRendererCommandsFilename);
	Engine.GetProfiler()->End("LoadSceneRendererCommands");

	UpdateLoadPercentageLevel(100, l_SliderResult);

	return l_Result;
}

void CLevel::UpdateLoadPercentageLevel(unsigned int Percentage, CGUIManager::SSliderResult &SliderResult)
{
	m_PercentageLoaded = Percentage;
	SliderResult.temp = (float)m_PercentageLoaded;

	CGUIManager::SGUIPosition l_BackgroundPosition = CGUIManager::SGUIPosition(0.0f, 0.0f, 1.0f, 1.0f, CGUIManager::BOTTOM_LEFT, CGUIManager::GUI_RELATIVE, CGUIManager::GUI_RELATIVE);
	CEngine::GetSingleton().GetGUIManager()->DoImage("background_image_0", "background_image", l_BackgroundPosition);

	CGUIManager::SGUIPosition l_PercentageLoadedPos = CGUIManager::SGUIPosition(0.5f, 0.85f, 0.8f, 0.015f, CGUIManager::TOP_CENTER, CGUIManager::GUI_RELATIVE, CGUIManager::GUI_RELATIVE_WIDTH);
	SliderResult = CEngine::GetSingleton().GetGUIManager()->DoSlider("level_percentage_loaded_slider_0", "level_percentage_loaded_slider", l_PercentageLoadedPos, 0.0, 100.0, SliderResult.temp);


	CGUIManager::SGUIPosition l_TextPos = CGUIManager::SGUIPosition(0.5f, 0.8f, 0.1f, 0.1f, CGUIManager::BOTTOM_CENTER, CGUIManager::GUI_RELATIVE, CGUIManager::GUI_RELATIVE_WIDTH);
	CEngine::GetSingleton().GetGUIManager()->DoText("LoadingText", "freestyle_script_64_font", l_TextPos, "", "Loading...");

	CRenderManager* l_RenderManager =  CEngine::GetSingleton().GetRenderManager();

	l_RenderManager->GetContextManager()->Clear(true, true);
	CEngine::GetSingleton().GetRenderableObjectTechniqueManager()->GetPoolRenderableObjectTechniques().GetResource("forward_shading")->Apply();
	//<set_depth_stencil_state enable_z_test = "false" write_z_buffer = "false" / >
	l_RenderManager->GetContextManager()->EnableAlphaBlendState();
	CEngine::GetSingleton().GetGUIManager()->Render(CEngine::GetSingleton().GetRenderManager());

	l_RenderManager->GetContextManager()->DisableAlphaBlendState();

	l_RenderManager->GetContextManager()->Present();
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
	Engine.GetLuaGameObjectHandleManager()->Destroy();
	Engine.GetGameObjectManager()->Destroy();
	
	Engine.GetSceneRendererCommandManager()->Destroy();

	return l_Result;
}