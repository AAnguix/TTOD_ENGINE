#include "ScriptManager\ScriptManager.h"

#include <luabind/luabind.hpp>

#include <luabind/operator.hpp>
#include <luabind/iterator_policy.hpp>

#include "Application.h"
#include "Engine.h"
#include "Log.h"
#include "Input\InputManagerImplementation.h"
#include "DebugHelperImplementation.h"

#include "Render\RenderManager.h"
#include "Materials\MaterialManager.h"
#include "Textures\TextureManager.h"
#include "Camera\CameraControllerManager.h"
#include "RenderableObjects\LayerManager.h"
#include "SceneRendererCommands\SceneRendererCommandManager.h"
#include "Lights\LightManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "Animation\AnimatedModelManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "PhysXManager.h"

#include "Components\ComponentManager.h"
#include "Components\Script.h"
#include "Components\Collider.h"
#include "Components\CharacterCollider.h"
#include "Components\LuaComponent.h"
#include "Components\AnimatorController\AnimatorController.h"
#include "Components\AnimatorController\Transition.h"
#include "Components\AudioSource.h"

#include "GUIManager.h"
#include "Particles\ParticleManager.h"
#include "ISoundManager.h"
#include "Render\GraphicsStats.h"
//#include "AStar.h"

using namespace luabind;

#define LUA_STATE CEngine::GetSingleton().GetScriptManager()->GetLuaState()
#define REGISTER_LUA_FUNCTION(FunctionName,AddrFunction) {luabind::module(LUA_STATE) [ luabind::def(FunctionName,AddrFunction) ];}

//CLUAComponent* f(CLUAComponent *Pointer) { return Pointer; }

//CRenderableObject* GetEntity(const std::string &Layer, const std::string &Object)
//{
//	return CEngine::GetSingleton().GetLayerManager()->GetResource(Layer)->GetResource(Object);
//}

void CScriptManager::RegisterCore()
{
	RegisterComponents();

	module(LUA_STATE)
	[
		class_<CApplication>("CApplication")
		.scope
		[
			def("IsGamePaused", &CApplication::IsGamePaused)
			,def("Pause", &CApplication::Pause)
		]
	];

	module(LUA_STATE) 
	[  
		class_<CSingleton<CEngine>>("EngineSingleton")  
		.scope
		[
			def("GetSingleton", &CSingleton<CEngine>::GetSingleton) 
			,def("GetSingletonPtr", &CSingleton<CEngine>::GetSingletonPtr) 
		]
	];

	module(LUA_STATE) 
	[
		class_<CEngine, CSingleton<CEngine>>("CEngine")
		.scope
		[
			def("TerminateApplication", &CEngine::TerminateApplication)
		]
		.def("GetEffectManager", &CEngine::GetEffectManager)
		.def("GetRenderableObjectTechniqueManager", &CEngine::GetRenderableObjectTechniqueManager)
		.def("GetMaterialManager", &CEngine::GetMaterialManager)
		.def("GetAnimatedModelManager", &CEngine::GetAnimatedModelManager)
		.def("GetStaticMeshManager", &CEngine::GetStaticMeshManager)
		.def("GetLayerManager", &CEngine::GetLayerManager)
		.def("GetLightManager", &CEngine::GetLightManager)
		.def("GetSceneRendererCommandManager", &CEngine::GetSceneRendererCommandManager)
		.def("GetCameraControllerManager", &CEngine::GetCameraControllerManager)
		.def("GetParticleSystemManager", &CEngine::GetParticleSystemManager)
		.def("GetInputManager", &CEngine::GetInputManager)

		.def("GetTextureManager", &CEngine::GetTextureManager)
		.def("GetRenderManager", &CEngine::GetRenderManager)
		.def("GetScriptManager", &CEngine::GetScriptManager)
		.def("GetLogManager", &CEngine::GetLogManager)
		.def("GetDebugHelper", &CEngine::GetDebugHelper)
		.def("GetPhysXManager", &CEngine::GetPhysXManager)
		.def("GetGUIManager", &CEngine::GetGUIManager)
		.def("GetSoundManager", &CEngine::GetSoundManager)
		.def("GetGraphicsStats", &CEngine::GetGraphicsStats)
		.def("LoadLevel", &CEngine::LoadLevel)
	];

	/*module(LUA_STATE) 
	[
		class_<CAStar>("CAStar")
		.def(constructor<>())
		.def("SearchPathA", &CAStar::SearchPathA)
		.def("AddNode", &CAStar::AddNode)
		.def("AddNeighbours", &CAStar::AddNeighbours)
	];*/

	module(LUA_STATE) 
	[
		class_<CLog>("CLog")
		.def(constructor<>())
		.def("Initialize", &CLog::Initialize)
		.def("Reload", &CLog::Reload)
		.def("Log", (void(CLog::*)(int))&CLog::Log)
		.def("Log", (void(CLog::*)(float))&CLog::Log)
		.def("Log", (void(CLog::*)(const std::string&))&CLog::Log)
		.def("Log", (void(CLog::*)(const Vect3f&))&CLog::Log)
		//.def("Log", (void(*)(int)) &CLog::Log)
	];

	module(LUA_STATE) 
	[  
		class_<CScriptManager>("CScriptManager")   
		.def(constructor<>())
		.def("run_code", & CScriptManager::RunCode)   
		.def("run_file", & CScriptManager::RunFile)   
		.def("load", & CScriptManager::Load) 
	];  

	module(LUA_STATE) 
	[
		class_<CInputManager>("CInputManager")
		.def("IsActionActive", &CInputManager::IsActionActive)
		.def("GetAxis", &CInputManager::GetAxis)
		.def("GetCursor", &CInputManager::GetCursor)
		.def("GetCursorMovement", &CInputManager::GetCursorMovement)
		.def("HasFocus", &CInputManager::HasFocus)
		.scope
		[
			def("GetInputManager", &CInputManager::GetInputManager) 
			,def("SetCurrentInputManager", &CInputManager::SetCurrentInputManager) 
		]
	];

	module(LUA_STATE) 
	[
		class_<CInputManagerImplementation,CInputManager>("CInputManagerImplementation")
		.def(constructor<>())
		.def("Initialize", &CInputManagerImplementation::Initialize)
		.def("load_commands_from_file", &CInputManagerImplementation::LoadCommandsFromFile)
		.def("begin_frame", &CInputManagerImplementation::BeginFrame)
		.def("end_frame", &CInputManagerImplementation::EndFrame)
		.def("set_focus", &CInputManagerImplementation::SetFocus)
		.def("set_mouse_speed", &CInputManagerImplementation::SetMouseSpeed)
		.def("key_event_received", &CInputManagerImplementation::KeyEventReceived)
		.def("update_cursor", &CInputManagerImplementation::UpdateCursor)
		.def("update_cursor_movement", &CInputManagerImplementation::UpdateCursorMovement)
		.def("GetHwnd", &CInputManagerImplementation::GetHwnd)
	];

	/*module(LUA_STATE) [  
		class_<CDebugHelper>("CDebugHelper")   
		.def("render", & CDebugHelper::Render)   
		.def("log", & CDebugHelper::Log)   
		.def("register_bar", & CDebugHelper::RegisterBar) 
		.def("remove_bar", & CDebugHelper::RemoveBar) 
		.scope
		[
			def("get_debug_helper", &CDebugHelper::GetDebugHelper) 
			,def("set_current_debug_helper", &CDebugHelper::SetCurrentDebugHelper) 
		]
		.enum_("EDebugType")
        [
            value("BUTTON",0),
            value("BOOL",1),
            value("FLOAT",2),
			value("INT",3),
			value("COLOR32",4),
			value("COLOR",5),
			value("STRING",6),
			value("POSITION_ORIENTATION",7)
        ]

		.enum_("Mode")
        [
            value("READ",0),
            value("READ_WRITE",1)
		]
	];  

	module(LUA_STATE) [  
		class_<CDebugHelper::SDebugVariable>("SDebugVariable") 
		//.def_readwrite("callback", &CDebugHelper::SDebugVariable::callback)
		.def_readwrite("data", &CDebugHelper::SDebugVariable::data)
		.def_readwrite("mode", &CDebugHelper::SDebugVariable::mode)
		.def_readwrite("name", &CDebugHelper::SDebugVariable::name)
		.def_readwrite("pBool", &CDebugHelper::SDebugVariable::pBool)
		.def_readwrite("pColor", &CDebugHelper::SDebugVariable::pColor)
		.def_readwrite("pColor32", &CDebugHelper::SDebugVariable::pColor32)
		.def_readwrite("pFloat", &CDebugHelper::SDebugVariable::pFloat)
		.def_readwrite("pInt", &CDebugHelper::SDebugVariable::pInt)
		.def_readwrite("pPositionOrientation", &CDebugHelper::SDebugVariable::pPositionOrientation)
		.def_readwrite("pString", &CDebugHelper::SDebugVariable::pString)
		.def_readwrite("ptr", &CDebugHelper::SDebugVariable::ptr)
		.def_readwrite("type", &CDebugHelper::SDebugVariable::type)
	];

	module(LUA_STATE) [  
		class_<CDebugHelper::SDebugBar>("SDebugBar")   
		.def_readwrite("name", &CDebugHelper::SDebugBar::name)
		.def_readwrite("variables", &CDebugHelper::SDebugBar::variables)
	];*/

	module(LUA_STATE) 
	[  
		class_<CDebugHelperImplementation>("CDebugHelperImplementation")   
		.def("CreateBar", & CDebugHelperImplementation::CreateBar)   
		.def("RemoveBar", & CDebugHelperImplementation::RemoveBar)   
		.def("RegisterButton", & CDebugHelperImplementation::RegisterLUAButton) 
		.def("RegisterExtendedButton", &CDebugHelperImplementation::RegisterLUAExtendedButton)
		.def("RegisterChangeTextureButton", &CDebugHelperImplementation::RegisterLUAChangeTextureButton)
		.def("ResetButtons", & CDebugHelperImplementation::ResetButtons) 
		.def("RegisterBoolParameter", & CDebugHelperImplementation::RegisterBoolParameter)
		.def("RegisterInt32Parameter", & CDebugHelperImplementation::RegisterInt32Parameter)
		.def("RegisterFloatParameter", & CDebugHelperImplementation::RegisterFloatParameter)
		.def("RegisterVect2fParameter", & CDebugHelperImplementation::RegisterVect2fParameter)
		.def("RegisterVect3fParameter", & CDebugHelperImplementation::RegisterVect3fParameter)
		.def("RegisterVect4fParameter", & CDebugHelperImplementation::RegisterVect4fParameter)
		.def("RegisterColorParameter", & CDebugHelperImplementation::RegisterColorParameter)
		.def("RegisterColor32Parameter", & CDebugHelperImplementation::RegisterColor32Parameter)
		.def("RegisterStringParameter", & CDebugHelperImplementation::RegisterStringParameter)
		.def("RegisterPositionOrientationParameter", & CDebugHelperImplementation::RegisterPositionOrientationParameter)
	];

	module(LUA_STATE)
	[
		class_<CDebugHelperImplementation::ClientData>("ClientData")
		.def(constructor<std::string, CEmptyPointerClass*, std::string>())
	];
	module(LUA_STATE)
	[
		class_<CDebugHelperImplementation::ChangeTextureClientData>("ChangeTextureClientData")
		.def(constructor<const std::string, CMaterial*, CEmptyPointerClass*, unsigned int>())
	];
}

struct CLUAComponent_wrapper : CLUAComponent, luabind::wrap_base
{
public:
	CLUAComponent_wrapper(const std::string Type) :CLUAComponent(Type){}
	/*std::string GetType() const
	{
		return call_member<std::string>(this, "GetType");
	}*/
};

void CScriptManager::RegisterComponents()
{
	module(LUA_STATE)
	[
		class_<CLUAComponent, CLUAComponent_wrapper>("CLUAComponent")
		.def(constructor<const std::string>())
		.def("AddTime", &CLUAComponent::AddTime)
		.def("GetTimer", &CLUAComponent::GetTimer)
		.def("ResetTimer", &CLUAComponent::ResetTimer)
		.def("GetType", &CLUAComponent::GetType)
	];

	module(LUA_STATE)
	[
		class_<CTemplatedVectorMapManager<CComponent>>("CTemplatedVectorMapManager")
		.def("GetResource", &CTemplatedVectorMapManager<CComponent>::GetResource)
		.def("GetResourceById", &CTemplatedVectorMapManager<CComponent>::GetResourceById)
		.def("AddResource", &CTemplatedVectorMapManager<CComponent>::AddResource)
		.def("RemoveResource", &CTemplatedVectorMapManager<CComponent>::RemoveResource)
		.def("GetResourcesMap", &CTemplatedVectorMapManager<CComponent>::GetResourcesMap)
		.def("GetResourcesVector", &CTemplatedVectorMapManager<CComponent>::GetResourcesVector)
	];

	module(LUA_STATE)
	[
		class_< CComponentManager, CTemplatedVectorMapManager<CComponent>>("CComponentManager")
		.def(constructor<>())
		.def("Update", &CComponentManager::Update)
		.def("Render", &CComponentManager::Render)
		.def("RenderDebug", &CComponentManager::RenderDebug)
		.def("AddComponent", &CComponentManager::AddComponent)
	];

	module(LUA_STATE)
	[
		class_<CComponent, CNamed>("CComponent")
		.def(constructor<const std::string&, CRenderableObject*>())
		.def("Update", &CComponent::Update)
		.def("Render", &CComponent::Render)
		.def("RenderDebug", &CComponent::RenderDebug)
		.def("GetOwner", &CComponent::GetOwner)
	];

	module(LUA_STATE)
	[
		class_<CScript, CComponent>("CScript")
		.def(constructor<const std::string&, CRenderableObject*, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&>())
		.scope
		[
			def("AddScript", &CScript::AddScript)
		]
	];

	module(LUA_STATE)
	[
		class_<CCollider, CComponent>("CCollider")
		.def(constructor<const std::string&, CMeshInstance*>())
		.def("GetPhysxMaterial", &CCollider::GetPhysxMaterial)
		.scope
		[
			def("AddCollider", &CCollider::AddCollider)
		]
	];

	module(LUA_STATE)
	[
		class_<CCharacterCollider, CComponent>("CCharacterCollider")
		.def(constructor<const std::string&, CAnimatedInstanceModel*>())
		.def("GetPhysxMaterial",&CCharacterCollider::GetPhysxMaterial)
		.scope
		[
			def("AddCharacterCollider", &CCharacterCollider::AddCharacterCollider)
		]
	];

	module(LUA_STATE)
	[
		class_<CTransition>("CTransition")
		.def(constructor<CState*, const bool, const float, const float, const float >())

		.def("AddFloatCondition", &CTransition::AddFloatCondition)
		.def("AddIntegerCondition", &CTransition::AddIntegerCondition)
		.def("AddBoolCondition", &CTransition::AddBoolCondition)
		.def("AddTriggerCondition", &CTransition::AddTriggerCondition)

		.def("MeetsConditions", &CTransition::MeetsConditions)
		.def("GetNewState", &CTransition::GetNewState)
		.def("GetDelayIn", &CTransition::GetDelayIn)
		.def("GetDelayOut", &CTransition::GetDelayOut)
	];

	module(LUA_STATE)
	[
		class_<CState>("CState")
		.def(constructor<CAnimatorController*, const std::string, const EAnimation, const float, const std::string, const std::string, const std::string>())
		.def("AddTransition", &CState::AddTransition)
		.def("OnEnter", &CState::OnEnter)
		.def("OnUpdate", &CState::OnUpdate)
		.def("OnExit", &CState::OnExit)
		.def("GetOwnAnimatorController", &CState::GetOwnAnimatorController)
	];

	module(LUA_STATE)
	[
		class_<CAnimatorController>("CAnimatorController")
		.def(constructor<const std::string, CRenderableObject*>())
		.def("AddState", &CAnimatorController::AddState)
		.def("AddInteger", &CAnimatorController::AddInteger)
		.def("AddFloat", &CAnimatorController::AddFloat)
		.def("AddBool", &CAnimatorController::AddBool)
		.def("AddTrigger", &CAnimatorController::AddTrigger)
		.def("SetInteger", &CAnimatorController::SetInteger)
		.def("SetFloat", &CAnimatorController::SetFloat)
		.def("SetBool", &CAnimatorController::SetBool)
		.def("SetTrigger", &CAnimatorController::SetTrigger)
		.scope
		[
			def("AddAnimatorController", &CAnimatorController::AddAnimatorController)
		]
	];

	module(LUA_STATE)
	[
		class_<CAudioSource>("CAudioSource")
		.def(constructor<const std::string, CRenderableObject*>())
		.def("RemoveSounds", &CAudioSource::RemoveSounds)
		.def("PlayEvent", &CAudioSource::PlayEvent)
		.def("AddSound", &CAudioSource::AddSound)
		.scope
		[
			def("AddAudioSource", &CAudioSource::AddAudioSource)
		]
	];
}