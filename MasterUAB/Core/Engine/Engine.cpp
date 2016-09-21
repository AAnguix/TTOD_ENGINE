#include "Engine\Engine.h"
#include "GameObject\GameObjectManager.h"
#include "GameObject\LuaGameObjectHandleManager.h"
#include "Materials\MaterialManager.h"
#include "Effects\EffectManager.h"
#include "Textures\TextureManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "LuabindManager\LuabindManager.h"
#include "Animation\AnimatedModelManager.h"
#include "Lights\LightManager.h"
#include "Camera\CameraControllerManager.h"
#include "Render\RenderManager.h"
#include "Log\Log.h"
#include "PhysXManager.h"
#include "RenderableObjects\LayerManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "SceneRendererCommands\SceneRendererCommandManager.h"
#include "DebugHelper\DebugHelperImplementation.h"
#include "Particles\ParticleManager.h"
#include "Input\InputMapperImplementation.h"
#include "GUIManager.h"
#include "ISoundManager.h"
#include "Render\GraphicsStats.h"
#include "Profiler\Profiler.h"
#include "Components\AnimatorController\AnimatorControllerManager.h"
#include "Components\Script\ScriptManager.h"
#include "Render\DebugRender.h"
#include "Level\Level.h"
#include "Utils\FileUtils.h"
#include <windef.h>
#include "Resources\ResourceManager.h"
#include "Engine\EngineSettings.h"
#include <sstream>

CEngine::CEngine()
:m_RenderManager(NULL)
,m_TimeScale(1.0f)
,m_Paused(false)
,m_LoadingLevel(false)
,m_GuiLoaded(false)
,m_PrevTimeStamp(0)
,m_SecsPerCnt(0.0f)
,m_TimeSinceStart(0.0f)
,m_HInstance(nullptr)
,m_Initialized(false)
,m_ElapsedTime(0.0f)
,m_EngineSettings(nullptr)
,m_LevelsToLoad(0)
,m_LevelsToUnload(0)
{
	m_Log = new CLog;
	m_LuabindManager = new CLuabindManager;
	m_CurrentLevel = "0";
	m_InputMapper = new InputMapping::CInputMapperImplementation;
	m_LuaGameObjectHandleManager = new CLuaGameObjectHandleManager();
	m_GameObjectManager = new CGameObjectManager();
	m_CameraControllerManager = new CCameraControllerManager;
	m_EffectManager = new CEffectManager;
	m_MaterialManager = new CMaterialManager;
	m_TextureManager = new CTextureManager;
	m_StaticMeshManager = new CStaticMeshManager;
	m_AnimatedModelManager = new CAnimatedModelManager;
	m_LightManager = new CLightManager;
	m_PhysXManager = CPhysXManager::CreatePhysxManager();
	m_LayerManager = new CLayerManager();
	m_RenderableObjectTechniqueManager = new CRenderableObjectTechniqueManager;
	m_SceneRendererCommandManager = new CSceneRendererCommandManager;
	m_DebugHelper = new CDebugHelperImplementation;
	m_ParticleSystemManager = new CParticleManager;
	m_GUIManager = new CGUIManager();
	m_SoundManager = ISoundManager::CreateSoundManager();
	m_GraphicsStats = new CGraphicsStats;
	m_AnimatorControllerManager = new CAnimatorControllerManager;
	m_ScriptManager = new CScriptManager;
	m_Profiler = new CProfiler;
	m_ResourceManager = new CResourceManager;
}

CEngine::~CEngine()
{
	for (size_t i = 0; i < m_Levels.size(); ++i)
	{
		delete m_Levels[i];
		m_Levels[i] = NULL;
	}
	{CHECKED_DELETE(m_ResourceManager); }
	{CHECKED_DELETE(m_Profiler); }
	{CHECKED_DELETE(m_ScriptManager); }
	{CHECKED_DELETE(m_AnimatorControllerManager); }
	{CHECKED_DELETE(m_GraphicsStats); }
	{CHECKED_DELETE(m_SoundManager); }
	{CHECKED_DELETE(m_GUIManager);}
	{CHECKED_DELETE(m_ParticleSystemManager); }
	{CHECKED_DELETE(m_DebugHelper);}
	{CHECKED_DELETE(m_SceneRendererCommandManager);}
	{CHECKED_DELETE(m_RenderableObjectTechniqueManager);}
	{CHECKED_DELETE(m_LayerManager);}
	{CHECKED_DELETE(m_PhysXManager);}
	{CHECKED_DELETE(m_LightManager);}
	{CHECKED_DELETE(m_AnimatedModelManager);}
	//{CHECKED_DELETE(m_RenderableObjectsManager);}
	{CHECKED_DELETE(m_StaticMeshManager);}
	{CHECKED_DELETE(m_TextureManager);}
	{CHECKED_DELETE(m_MaterialManager);}
	{CHECKED_DELETE(m_EffectManager);}
	{CHECKED_DELETE(m_CameraControllerManager);}
	{CHECKED_DELETE(m_GameObjectManager); }
	{CHECKED_DELETE(m_LuaGameObjectHandleManager); }
	{CHECKED_DELETE(m_InputMapper); }
	{CHECKED_DELETE(m_LuabindManager); }
	{CHECKED_DELETE(m_Log); }
	m_HInstance = nullptr;
	m_EngineSettings = nullptr;
}

void CEngine::Initialize(HINSTANCE* HInstance)
{
	m_HInstance = HInstance;

	__int64 l_CntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&l_CntsPerSec);
	m_SecsPerCnt = 1.0f / (float)l_CntsPerSec;

	QueryPerformanceCounter((LARGE_INTEGER*)&m_PrevTimeStamp);

	m_Profiler->Initialize();

	m_ResourceManager->Initialize();

	CInputMapper::SetCurrentInputMapper(m_InputMapper);


	CContextManager* l_ContextManager = m_RenderManager->GetContextManager();
	m_DebugHelper->Initialize(l_ContextManager->GetDevice());
	CDebugHelper::SetCurrentDebugHelper(m_DebugHelper);

	m_GUIManager->Initialize((float)l_ContextManager->GetFrameBufferWidth(), (float)l_ContextManager->GetFrameBufferHeight());
	m_Log->Initialize(true);

	m_Profiler->Begin("Load Effects");
	CFileUtils::CheckEffectsFolders();
	m_EffectManager->Load("./Data/effects.xml");
	m_RenderableObjectTechniqueManager->Load("Data/renderable_objects_techniques.xml");
	m_Profiler->End("Load Effects");

	m_Profiler->Begin("LoadMaterials");
		m_MaterialManager->Load("./Data/gui_materials.xml");
		m_GUIManager->Load("./Data/gui_start_screen.xml");
		m_GUIManager->Load("./Data/gui_in_game.xml");
	m_Profiler->End("LoadMaterials");

	m_LuabindManager->Initialize();

	m_Profiler->Begin("LoadSoundBanks");
		m_SoundManager->SetPath("./Data/Audio/Soundbanks/General/");
		m_SoundManager->Init();
		m_SoundManager->Load("SoundbanksInfo.xml", "");
	m_Profiler->End("LoadSoundBanks");

	m_SceneRendererCommandManager->Load("./Data/start_screen_scene_renderer_commands.xml");

	char* l_VCDescription = new char[128];
	int l_VCMemory;
	l_ContextManager->GetVideoCardInfo(l_VCDescription, l_VCMemory);

	#ifdef _DEBUG
		Quatf l_QuatCamera = Quatf(0.0f, 0.0f, 0.0f, 1.0f);
		bool l_CamPhysX = CEngine::GetSingleton().GetPhysXManager()->CreateSphereTrigger("DebugPhysxCamera", 0.1f, "GROUP3", Vect3f(0.0f, 0.0f, 0.0f), l_QuatCamera, "kinematic");
		CEngine::GetSingleton().GetLogManager()->Log("VC: " + std::string(l_VCDescription) + ". Memory: " + std::to_string(l_VCMemory)+" mb.");
	#endif

	delete l_VCDescription;

	m_Initialized = true;

	CEngine::GetSingleton().GetLogManager()->Log("Engine initialized.");
}

float CEngine::GetRealTimeSinceStartup()
{
	__int64 currTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
	float l_ElapsedTime = (currTimeStamp - m_PrevTimeStamp)*m_SecsPerCnt;
	m_TimeSinceStart += l_ElapsedTime;

	m_PrevTimeStamp = currTimeStamp;

	return m_TimeSinceStart;
}

/*Data taht needs to be loaded before load evert level*/
void CEngine::LoadLevelsCommonData()
{
	m_MaterialManager->Load("./Data/effects_materials.xml");
	m_CameraControllerManager->Load("./Data/camera_controllers.xml");
}

void CEngine::SetTimeScale(float TimeScale)
{
	m_TimeScale = TimeScale;
}

bool CEngine::Update(float ElapsedTime)
{
	if (!m_PhysXManager->Simulating())
	{
		UpdateLevels();
	}

	ElapsedTime *= m_TimeScale;

	m_ElapsedTime = ElapsedTime;

	CAnimatorControllerManager* l_AnimatorControllerManager = CEngine::GetSingleton().GetAnimatorControllerManager();

	CScriptManager* l_ScriptManager = CEngine::GetSingleton().GetScriptManager();

	CCameraControllerManager* l_CameraController = CEngine::GetSingleton().GetCameraControllerManager();

	std::stringstream l_Ss;
	l_Ss << "Update(" << ElapsedTime << ")";
	std::string l_Code = l_Ss.str();
	m_LuabindManager->RunCode(l_Code);
	
	/*m_RenderManager->GetContextManager()->SetTimeParameters(ElapsedTime);

	l_CameraController->Update(ElapsedTime);

	m_PhysXManager->Update(ElapsedTime);

	m_LayerManager->Update(ElapsedTime);

	l_AnimatorControllerManager->Update(ElapsedTime);

	l_ScriptManager->Update(ElapsedTime);*/

	if (!m_Paused)
	{
		m_RenderManager->GetContextManager()->SetTimeParameters(ElapsedTime);

		l_AnimatorControllerManager->Update(ElapsedTime);

		l_ScriptManager->Update(ElapsedTime);

		//l_CameraController->Update(ElapsedTime);

		m_PhysXManager->Update(ElapsedTime);

		m_LayerManager->Update(ElapsedTime);

		l_CameraController->Update(ElapsedTime);
	}

	CCameraController* l = CEngine::GetSingleton().GetCameraControllerManager()->GetCurrentCameraController();
	CCamera l_Camera = CEngine::GetSingleton().GetRenderManager()->GetCurrentCamera();
	Vect3f x = l->GetUp();
	m_SoundManager->Update(&l_Camera, l->GetForward().Normalize(), ElapsedTime);

	m_LuaGameObjectHandleManager->Update();

	m_GraphicsStats->Update(ElapsedTime);

	m_Profiler->Update();

	return true;
}

bool CEngine::AddLevel(const std::string &Level, const std::string &LuaLoadFunction)
{
	bool l_Exists = false;

	for (size_t i = 0; i < m_Levels.size(); ++i)
	{
		if (m_Levels[i]->GetID() == Level)
			l_Exists = true;
	}
	if (!l_Exists)
		m_Levels.push_back(new CLevel(Level, LuaLoadFunction));

	return !l_Exists;
}

void CEngine::LoadLevel(const std::string &Level)
{
	for (size_t i = 0; i < m_Levels.size(); ++i)
	{
		if (m_Levels[i]->GetID() == Level)
		{
			std::vector<CLevel*>::iterator it;
			it = find(m_LevelsToLoad.begin(), m_LevelsToLoad.end(), m_Levels[i]);
			if (it == m_LevelsToLoad.end())
			{
				m_LevelsToLoad.push_back(m_Levels[i]);
			}
			else 
			{
				LOG("Error. Level " + Level + " is pending to be loaded.");
			}
		}
	}
}

/*
	Loads/Unloads the levels pending to be loaded/unloaded.
*/
void CEngine::UpdateLevels()
{
	for (size_t i = 0; i < m_LevelsToUnload.size(); ++i)
	{
		m_LevelsToUnload[i]->Unload(*this);
		m_CurrentLevel = "";
	}
	m_LevelsToUnload.clear();
	for (size_t i = 0; i < m_LevelsToLoad.size(); ++i)
	{
		LoadLevelsCommonData();
		m_LevelsToLoad[i]->Load(*this);
		m_CurrentLevel = m_LevelsToLoad[i]->GetID();
	}
	m_LevelsToLoad.clear();
}

void CEngine::UnloadLevel(const std::string &Level)
{
	for (size_t i = 0; i < m_Levels.size(); ++i)
	{
		if (m_Levels[i]->GetID() == Level)
		{
			std::vector<CLevel*>::iterator it;
			it = find(m_LevelsToUnload.begin(), m_LevelsToUnload.end(), m_Levels[i]);
			if (it == m_LevelsToUnload.end())
			{
				m_LevelsToUnload.push_back(m_Levels[i]);
			}
			else
			{
				LOG("Error. Level " + Level + " is pending to be unloaded.");
			}
		}
	}
}

CGameObjectManager* CEngine::GetGameObjectManager() const { return m_GameObjectManager; }

CLuaGameObjectHandleManager* CEngine::GetLuaGameObjectHandleManager() const { return m_LuaGameObjectHandleManager; }

CEffectManager* CEngine::GetEffectManager() const {	return m_EffectManager; }

CMaterialManager* CEngine::GetMaterialManager() const {	return m_MaterialManager; }
	
CTextureManager* CEngine::GetTextureManager() const { return m_TextureManager; }

/*
CRenderableObjectsManager* CEngine::GetRenderableObjectsManager() const
{
	return m_RenderableObjectsManager;
}*/

CStaticMeshManager* CEngine::GetStaticMeshManager() const {	return m_StaticMeshManager; }

CLuabindManager* CEngine::GetLuabindManager() const { return m_LuabindManager; }

CAnimatedModelManager* CEngine::GetAnimatedModelManager() const { return m_AnimatedModelManager; }

CLightManager* CEngine::GetLightManager() const { return m_LightManager; }

CLog* CEngine::GetLogManager() const { return m_Log; }

CRenderableObjectTechniqueManager* CEngine::GetRenderableObjectTechniqueManager() const { return m_RenderableObjectTechniqueManager; }

CSceneRendererCommandManager* CEngine::GetSceneRendererCommandManager() const { return m_SceneRendererCommandManager; }

CDebugHelperImplementation* CEngine::GetDebugHelper() const { return m_DebugHelper;}

CParticleManager* CEngine::GetParticleSystemManager() const { return m_ParticleSystemManager;}

InputMapping::CInputMapperImplementation* CEngine::GetInputMapper() const { return m_InputMapper; }

CRenderManager* CEngine::GetRenderManager() const { return m_RenderManager; }

CGraphicsStats* CEngine::GetGraphicsStats() const { return m_GraphicsStats; }

CProfiler* CEngine::GetProfiler() const { return m_Profiler; }

CPhysXManager* CEngine::GetPhysXManager() const { return m_PhysXManager; }

CLayerManager* CEngine::GetLayerManager() const { return m_LayerManager; }
CGUIManager* CEngine::GetGUIManager() const{ return m_GUIManager; }
ISoundManager* CEngine::GetSoundManager() const { return m_SoundManager; }
CAnimatorControllerManager* CEngine::GetAnimatorControllerManager() const { return m_AnimatorControllerManager;}
CCameraControllerManager* CEngine::GetCameraControllerManager() const { return m_CameraControllerManager; }
CScriptManager* CEngine::GetScriptManager() const { return m_ScriptManager; }

CResourceManager* CEngine::GetResourceManager() const { return m_ResourceManager; }

void CEngine::TerminateApplication()
{
	PostQuitMessage(0);
}

CEmptyPointerClass* CEngine::GetPausedLuaAddress() const { return (CEmptyPointerClass *)&m_Paused; }
CEmptyPointerClass* CEngine::GetTimeScaleLuaAddress() const { return (CEmptyPointerClass *)&m_TimeScale; }

CEngineSettings* CEngine::GetEngineSettings() const{ return m_EngineSettings; }

void CEngine::SetEngineSettings(CEngineSettings* Settings){ m_EngineSettings = Settings; }