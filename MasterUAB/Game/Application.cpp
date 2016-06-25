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

#include <cmath>
#include "AStar.h"

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

	l_Engine.Initialize();

	//CAStar l_AStar;
	//m_RenderManager.GetDebugRender()->InitializeASTarDebug(l_AStar.SearchPathA(Vect3f(0.0f,0.0f,0.0f),Vect3f(-10.0f,0.0f,5.0f)));
		
	l_Engine.LoadLevelsCommonData();

	m_RenderManager.InitializeDebugRender();

	/*CEngine::GetSingleton().GetSceneRendererCommandManager()->Load("./Data/scene_renderer_commands.xml");*/
	l_Engine.GetLuabindManager()->RunLuaMain();

	CEngine::GetSingleton().GetLogManager()->Log("Application initialized properly.");
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
	CEngine::GetSingleton().Update(ElapsedTime);
}

void CApplication::Render()
{
	CEngine::GetSingleton().GetSceneRendererCommandManager()->Execute(m_RenderManager);

	/*m_ContextManager->
	Render(m_BackgroundColor);
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

	//m_RenderManager.Render(m_ContextManager);
	
	//m_ContextManager->EndRender();
}