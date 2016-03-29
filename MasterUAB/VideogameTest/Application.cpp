#include "Application.h"

#include "Math\Matrix44.h"
#include "Math\Vector4.h"

#include "Render\ContextManager.h"
#include "Input\InputManagerImplementation.h"
#include "DebugHelper.h"
#include "Engine.h"
#include "Effects\EffectManager.h"
#include "Lights\LightManager.h"

#include "StaticMeshes\StaticMeshManager.h"
#include "RenderableObjects\LayerManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "RenderableObjects\LayerManager.h"
#include "Particles\ParticleManager.h"

#include "ScriptManager\ScriptManager.h"

#include "Animation\AnimatedModelManager.h"
#include "Animation\AnimatedInstanceModel.h"
#include "Camera\CameraControllerManager.h"
#include "Log.h"
#include "PhysXManager.h"
#include "GUIManager.h"
#include "Cinematics\Cinematic.h"

#include "SceneRendererCommands\SceneRendererCommandManager.h"

#include "DebugHelperImplementation.h"

#include <cmath>

#include "AStar.h"
#include "Render\DebugRender.h"

static void __stdcall SwitchCameraCallback(void* _app)
{
	((CApplication*)_app)->SwitchCamera();
}

CApplication::CApplication(CContextManager *_ContextManager)
	: m_ContextManager(_ContextManager)
	, m_BackgroundColor(0.8f, 1.0f, 1.0f)
	, m_CurrentCamera(0)
	, m_RenderManager(_ContextManager)
{
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
	CEngine &l_Engine=CEngine::GetSingleton();
	
	l_Engine.GetInputManager()->Initialize(Hwnd);
	l_Engine.GetInputManager()->LoadCommandsFromFile("./Data/Input.xml");
	CInputManager::SetCurrentInputManager(l_Engine.GetInputManager());

	l_Engine.GetDebugHelper()->Initialize(m_ContextManager->GetDevice());
	CDebugHelper::SetCurrentDebugHelper(l_Engine.GetDebugHelper());

	l_Engine.GetGUIManager()->Initialize(m_ContextManager->GetFrameBufferWidth(), m_ContextManager->GetFrameBufferHeight());

	l_Engine.GetLogManager()->Initialize(true);
	
	//CAStar l_AStar;
	//m_RenderManager.GetDebugRender()->InitializeASTarDebug(l_AStar.SearchPathA(Vect3f(0.0f,0.0f,0.0f),Vect3f(-10.0f,0.0f,5.0f)));
	/*TODO ARREGLAR*/
	
	l_Engine.GetEffectManager()->Load("./Data/Effects.xml");
	l_Engine.GetCameraControllerManager()->Load("./Data/level/Cameras.xml");
	
	l_Engine.GetRenderableObjectTechniqueManager()->Load("Data/renderable_objects_techniques.xml");
	
	l_Engine.GetMaterialManager()->Load("./Data/level/Materials.xml");
	l_Engine.GetMaterialManager()->Load("./Data/level/EffectsMaterials.xml");
	l_Engine.GetMaterialManager()->Load("./Data/GUIMaterials.xml");

	l_Engine.GetGUIManager()->Load("./Data/GUI.xml");
	l_Engine.GetStaticMeshManager()->Load("./Data/level/StaticMeshes.xml");
	
	l_Engine.GetParticleSystemManager()->Load("./Data/level/ParticlesSystems.xml");
	l_Engine.GetAnimatedModelManager()->Load("./Data/level/AnimatedModels.xml");

	l_Engine.GetRenderManager()->GetContextManager()->SetMatrices(CEngine::GetSingleton().GetRenderManager()->GetCurrentCamera());
	/*Used in particles rendering order*/

	l_Engine.GetLayerManager()->Load("./Data/level/RenderableObjects.xml");
	l_Engine.GetLightManager()->Load("./Data/level/Lights.xml");

	/*
	l_Engine.GetPhysXManager()->RegisterMaterial("DefaultMaterial",1.0f,1.0f,0.5f);
	l_Engine.GetPhysXManager()->CreateRigidStatic("Cube0",Vect3f(1.0f,1.0f,1.0f),Vect3f(0.0f,0.0f,0.0f),Quatf(0.0f,0.0f,0.0f,1.0f),"DefaultMaterial");
	//l_Engine.GetPhysXManager()->CreatePlane("Plane001",Vect3f(0.0f,1.0f,0.0f),1.0f,"DefaultMaterial");
	//l_Engine.GetPhysXManager()->CreateCharacterController("TestCharacter",1.0f,0.1f,1.0f,Vect3f(0.0f,0.0f,0.0f),"DefaultMaterial");
	//l_Engine.GetPhysXManager()->MoveCharacterController("TestCharacter",Vect3f(4.0f,0.0f,0.0f),0.5f);
	l_Engine.GetPhysXManager()->CreateRigidStatic("Cube1",Vect3f(1.0f,1.0f,1.0f),Vect3f(1.0f,1.0f,1.0f),Quatf(0.0f,0.0f,0.0f,1.0f),"DefaultMaterial");
	l_Engine.GetPhysXManager()->CreateRigidStatic("Cube2",Vect3f(1.0f,1.0f,1.0f),Vect3f(2.0f,2.0f,2.0f),Quatf(0.0f,0.0f,0.0f,1.0f),"DefaultMaterial");
	l_Engine.GetPhysXManager()->CreateRigidStatic("Cube3",Vect3f(1.0f,1.0f,1.0f),Vect3f(3.0f,3.0f,3.0f),Quatf(0.0f,0.0f,0.0f,1.0f),"DefaultMaterial");
	l_Engine.GetPhysXManager()->CreateRigidStatic("Cube4",Vect3f(1.0f,1.0f,1.0f),Vect3f(4.0f,4.0f,4.0f),Quatf(0.0f,0.0f,0.0f,1.0f),"DefaultMaterial");
	l_Engine.GetPhysXManager()->CreateRigidStatic("Cube5",Vect3f(1.0f,1.0f,1.0f),Vect3f(5.0f,5.0f,5.0f),Quatf(0.0f,0.0f,0.0f,1.0f),"DefaultMaterial");

	l_Engine.GetPhysXManager()->RemoveActor("Cube0");

	l_Engine.GetPhysXManager()->RemoveActor("Cube4");*/

	//((CCinematic*)l_Engine.GetRenderableObjectsManager()->GetResource("BoxMovement"))->Play(true);

	//CEngine::GetSingleton().GetScriptManager()->RunFile("./Data/scripting/ReloadShaders.lua");

	m_RenderManager.InitializeDebugRender();

	CEngine::GetSingleton().GetSceneRendererCommandManager()->Load("./Data/scene_renderer_commands.xml");

	l_Engine.GetScriptManager()->Initialize();
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
	CCameraControllerManager* l_CCManager = CEngine::GetSingleton().GetCameraControllerManager();

	std::stringstream l_Ss;
	l_Ss << "Update(" << ElapsedTime << ")";
	std::string l_Code = l_Ss.str();
	CEngine::GetSingleton().GetScriptManager()->RunCode(l_Code);

	//CEngine::GetSingleton().GetLogManager()->Log(boost::lexical_cast<string>(ElapsedTime));

	//switch (m_CurrentCamera)
	switch(l_CCManager->GetCurrentCameraController()->GetType())
	{
		/*case 0:
			if (CInputManager::GetInputManager()->IsActionActive("MOVE_FWD"))
			{
				Vect3f cameraMovement(0, 0, 0);
				float k= CInputManager::GetInputManager()->GetAxis("MOVE_FWD") * ElapsedTime * 0.1f;
				cameraMovement.x +=k;
				//cameraMovement.y += CInputManager::GetInputManager()->GetAxis("Y_AXIS") * ElapsedTime * 20.5f;

				
				((CSphericalCameraController*)CEngine::GetSingleton().GetCameraControllerManager()->GetCurrentCameraController())->Update(cameraMovement);
				//m_SphericalCamera.Update(cameraMovement);
			}
			break;*/

		case 0: //Fixed
			{
				/*CFPSCameraController* l_FPSController = ((CFPSCameraController*)l_CCManager->GetCurrentCameraController());
				l_FPSController->AddYaw(-CInputManager::GetInputManager()->GetAxis("X_AXIS") * ElapsedTime * 0.05f);
				l_FPSController->AddPitch(-CInputManager::GetInputManager()->GetAxis("Y_AXIS") * ElapsedTime * -0.05f);*/
				
				/*if (CInputManager::GetInputManager()->IsActionActive("MOVE_FWD"))
				{
					l_FPSController->Move(0.0f,CInputManager::GetInputManager()->GetAxis("MOVE_FWD")*ElapsedTime,false,ElapsedTime);
				}
				if (CInputManager::GetInputManager()->IsActionActive("MOVE_BACK"))
				{
					l_FPSController->Move(0.0f,CInputManager::GetInputManager()->GetAxis("MOVE_BACK")*ElapsedTime,false,ElapsedTime);
				}
				if (CInputManager::GetInputManager()->IsActionActive("STRAFE_RIGHT"))
				{
					l_FPSController->Move(CInputManager::GetInputManager()->GetAxis("STRAFE")*ElapsedTime,0.0f,false,ElapsedTime);
				}
				if (CInputManager::GetInputManager()->IsActionActive("STRAFE_LEFT"))
				{
					l_FPSController->Move(CInputManager::GetInputManager()->GetAxis("STRAFE")*ElapsedTime,0.0f,false,ElapsedTime);
				}*/

				//CInputManager::GetInputManager()->GetAxis("ZOOM") * ElapsedTime * -0.05f
				//if (CInputManager::GetInputManager()->IsActionActive("MOVE_BACK"))
				//m_FPSCamera.Move(CInputManager::GetInputManager()->GetAxis("MOVE_BACK") * ElapsedTime * -0.05f,CInputManager::GetInputManager()->GetAxis("MOVE_BACK"),false,ElapsedTime);
			

					//if (CInputManager::GetInputManager()->IsActionActive("STRAFE_LEFT"))
				//m_FPSCamera.Move(CInputManager::GetInputManager()->GetAxis("STRAFE"), CInputManager::GetInputManager()->GetAxis("MOVE_FWD"), false, ElapsedTime);
			}
		break;
	}
	
	//CCamera camera;
	
	//FPS
	/*m_FPSCamera.SetCamera(&camera);
	camera.SetFOV(1.047f);
	camera.SetAspectRatio(m_ContextManager->GetAspectRatio());
	camera.SetZNear(0.1f);
	camera.SetZFar(100.f);
	camera.SetMatrixs();
	m_RenderManager.SetCurrentCamera(camera);*/
	
	//Fixed
	/*CEngine::GetSingleton().GetCameraManager()->GetResource("FixedCamera")->SetCamera(&camera);
	camera.SetAspectRatio(m_ContextManager->GetAspectRatio());
	camera.SetMatrixs();
	m_RenderManager.SetCurrentCamera(camera);*/

	//CameraKeyController

	/*CEngine::GetSingleton().GetCameraManager()->GetResource("Camera001")->Update(ElapsedTime);
	CEngine::GetSingleton().GetCameraManager()->GetResource("Camera001")->SetCamera(&camera);
	camera.SetAspectRatio(m_ContextManager->GetAspectRatio());
	camera.SetMatrixs();
	m_RenderManager.SetCurrentCamera(camera);*/
	

	//Spherical
	/*m_SphericalCamera.SetCamera(&camera);
	camera.SetFOV(1.047f);
	camera.SetAspectRatio(m_ContextManager->GetAspectRatio());
	camera.SetZNear(0.1f);
	camera.SetZFar(100.f);
	camera.SetMatrixs();
	m_RenderManager.SetDebugCamera(camera);

	m_RenderManager.SetUseDebugCamera(m_CurrentCamera == 0);*/
	
	CEngine::GetSingleton().GetCameraControllerManager()->Update(ElapsedTime);

	CEngine::GetSingleton().GetPhysXManager()->Update(ElapsedTime);

	CEngine::GetSingleton().GetLayerManager()->Update(ElapsedTime);

	//FollowPlayer(ElapsedTime,0.25f);

	//CEffectManager::m_SceneEffectParameters.m_Time = Vect4f(ElapsedTime,ElapsedTime,ElapsedTime,1.0f);
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

Vect3f CApplication::CalcForward(float Yaw)
{
	/*Vect3f right;
	right.x = cos(Yaw);
	right.y = 0.0f;
	right.z = sin(Yaw);
	
	Vect3f forward;
	forward = up^right;
	*/
	Vect3f forward = Vect3f(sin(Yaw), 0.0f, cos(Yaw));

	return forward;
}