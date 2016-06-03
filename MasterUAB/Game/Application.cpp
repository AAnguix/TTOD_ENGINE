#include "Application.h"
#include "ScriptManager.h"

/*Base*/
#include "Math\Matrix44.h"
#include "Math\Vector4.h"

/*Core*/
#include "Input\InputManagerImplementation.h"
#include "DebugHelper\DebugHelperImplementation.h"
#include "Log\Log.h"
#include "Engine.h"
#include "LuabindManager\LuabindManager.h"
#include "Render\DebugRender.h"

/*Graphics*/
#include "Render\ContextManager.h"
#include "Effects\EffectManager.h"
#include "Lights\LightManager.h"
#include "Materials\MaterialManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "RenderableObjects\LayerManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "RenderableObjects\LayerManager.h"
#include "Particles\ParticleManager.h"
#include "Animation\AnimatedModelManager.h"
#include "Animation\AnimatedInstanceModel.h"
#include "Camera\CameraControllerManager.h"
#include "Cinematics\Cinematic.h"
#include "Animation\AnimatorControllerManager.h"
#include "SceneRendererCommands\SceneRendererCommandManager.h"

/*Physics*/
#include "PhysXManager.h"

/*GUI*/
#include "GUIManager.h"

/*Sound*/
#include "ISoundManager.h"

#include <sstream>
#include <cmath>
#include "AStar.h"


bool CApplication::m_Paused = false;

static void __stdcall SwitchCameraCallback(void* _app)
{
	((CApplication*)_app)->SwitchCamera();
}

CApplication::CApplication(CContextManager *_ContextManager)
	:m_ContextManager(_ContextManager)
	,m_BackgroundColor(0.8f, 1.0f, 1.0f)
	,m_CurrentCamera(0)
	,m_RenderManager(_ContextManager)
{
	//CSingleton<CEngine> *l_Sing = new CSingleton<CEngine>();
	CEngine::GetSingleton().SetRenderManager(&m_RenderManager);
	CDebugHelper::GetDebugHelper()->Log("CApplication::CApplication");
	m_Paused = false;
	/*CDebugHelper::SDebugBar bar;
	bar.name = "CApplication";
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "switch camera";
		var.type = CDebugHelper::BUTTON;
		var.callback = SwitchCameraCallback;
		var.data = this;

		bar.variables.push_back(var);
	}
	CDebugHelper::GetDebugHelper()->RegisterBar(bar);
	*/
}

CApplication::~CApplication()
{
	CDebugHelper::GetDebugHelper()->Log("CApplication::~CApplication");
}

void CApplication::Initialize(HWND Hwnd)
{
	CEngine &l_Engine = CEngine::GetSingleton();

	l_Engine.GetInputManager()->Initialize(Hwnd);
	l_Engine.GetInputManager()->LoadCommandsFromFile("./Data/input.xml");
	CInputManager::SetCurrentInputManager(l_Engine.GetInputManager());

	l_Engine.GetDebugHelper()->Initialize(m_ContextManager->GetDevice());
	CDebugHelper::SetCurrentDebugHelper(l_Engine.GetDebugHelper());

	l_Engine.GetGUIManager()->Initialize((float)m_ContextManager->GetFrameBufferWidth(), (float)m_ContextManager->GetFrameBufferHeight());
	l_Engine.GetLogManager()->Initialize(true);

	l_Engine.GetLuabindManager()->Initialize();

	//CAStar l_AStar;
	//m_RenderManager.GetDebugRender()->InitializeASTarDebug(l_AStar.SearchPathA(Vect3f(0.0f,0.0f,0.0f),Vect3f(-10.0f,0.0f,5.0f)));

	l_Engine.GetEffectManager()->Load("./Data/effects.xml");
	l_Engine.GetRenderableObjectTechniqueManager()->Load("Data/renderable_objects_techniques.xml");
	l_Engine.GetMaterialManager()->Load("./Data/effects_materials.xml");
	l_Engine.GetMaterialManager()->Load("./Data/gui_materials.xml");
	l_Engine.GetGUIManager()->Load("./Data/gui_start_screen.xml");
	l_Engine.GetCameraControllerManager()->Load("./Data/cameras.xml");

	m_RenderManager.InitializeDebugRender();

//	l_Engine.GetSoundManager()->SetPath("./Data/Audio/Soundbanks/");
	//l_Engine.GetSoundManager()->Init();

	CEngine::GetSingleton().GetSceneRendererCommandManager()->Load("./Data/scene_renderer_commands.xml");
	l_Engine.GetLuabindManager()->RunLuaMain();
}

void CApplication::SwitchCamera()
{
	++m_CurrentCamera;
	if (m_CurrentCamera > 1)
	{
		m_CurrentCamera = 0;
	}
}

void CApplication::Update(float ElapsedTime)
{	
	CLuabindManager* l_LuabindManager = CEngine::GetSingleton().GetLuabindManager();

	CCamera l_Camera = CEngine::GetSingleton().GetRenderManager()->GetCurrentCamera();
	
	ISoundManager* l_SoundManager = CEngine::GetSingleton().GetSoundManager(); assert(l_SoundManager != nullptr);
	
	CAnimatorControllerManager* l_AnimatorControllerManager = CEngine::GetSingleton().GetAnimatorControllerManager();
	
	CScriptManager* l_ScriptManager = CEngine::GetSingleton().GetScriptManager();
	
	CCameraControllerManager* l_CameraController = CEngine::GetSingleton().GetCameraControllerManager(); assert(l_CameraController != nullptr);
	
	std::stringstream l_Ss;
	l_Ss << "Update(" << ElapsedTime << ")";
	std::string l_Code = l_Ss.str();
	l_LuabindManager->RunCode(l_Code);
	
	if (!CApplication::IsGamePaused())
	{
		m_RenderManager.GetContextManager()->SetTimes(ElapsedTime);
		
		l_CameraController->Update(ElapsedTime);

		CEngine::GetSingleton().GetPhysXManager()->Update(ElapsedTime);

		CEngine::GetSingleton().GetLayerManager()->Update(ElapsedTime);

		l_AnimatorControllerManager->Update(ElapsedTime);

		l_ScriptManager->Update(ElapsedTime);
	}

	l_SoundManager->Update(&l_Camera, ElapsedTime);
	
}

void CApplication::Render()
{
	/*m_ContextManager->BeginRender(m_BackgroundColor);
	Mat44f world;

	world.SetIdentity();
	m_ContextManager->SetWorldMatrix(world);
	m_ContextManager->Draw(m_DebugRender->GetAxis());

	world.SetIdentity();
	world.SetFromPos(10, 0, 0);
	m_ContextManager->SetWorldMatrix(world);
	m_ContextManager->Draw(m_DebugRender->GetClassicBlendTriangle(), CContextManager::RS_SOLID, CContextManager::DSS_OFF, CContextManager::BLEND_CLASSIC);
	
	world.SetIdentity();
	world.SetFromPos(0, 0, -10);
	m_ContextManager->SetWorldMatrix(world);
	m_ContextManager->Draw(m_DebugRender->GetPremultBlendTriangle(), CContextManager::RS_SOLID, CContextManager::DSS_OFF, CContextManager::BLEND_PREMULT);
	

	CDebugHelper::GetDebugHelper()->Render();*/

	
	CEngine::GetSingleton().GetSceneRendererCommandManager()->Execute(m_RenderManager);
	//m_RenderManager.Render(m_ContextManager);
	
	//m_ContextManager->EndRender();
}

bool CApplication::IsGamePaused()
{
	return m_Paused; 
};
void CApplication::Pause() 
{ 
	m_Paused = !m_Paused; 
};