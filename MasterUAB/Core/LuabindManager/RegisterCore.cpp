#include "LuabindManager\LuabindManager.h"
#include "LuabindManager\LuabindMacros.h"
#include <luabind/luabind.hpp>

#include <luabind/operator.hpp>
#include <luabind/iterator_policy.hpp>

#include "Engine\Engine.h"
#include "Log\Log.h"
#include "DebugHelper\DebugHelperImplementation.h"
#include "Utils\EmptyPointerClass.h"

/*Base*/
#include "GameObject\GameObjectManager.h"
#include "GameObject\LuaGameObjectHandleManager.h"

/*Graphics*/
#include "Render\RenderManager.h"
#include "Materials\MaterialManager.h"
#include "Materials\Material.h"
#include "Textures\TextureManager.h"
#include "Camera\CameraControllerManager.h"
#include "RenderableObjects\LayerManager.h"
#include "SceneRendererCommands\SceneRendererCommandManager.h"
#include "Lights\LightManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "Animation\AnimatedModelManager.h"
#include "Components\AnimatorController\AnimatorControllerManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "PhysXManager.h"
#include "Render\ContextManager.h"

/*Core*/
#include "GameObject\LuaGameObjectHandleManager.h"
#include "Components\Script\Script.h"
#include "Components\Collider.h"
#include "Components\CharacterCollider.h"
#include "Components\LuaComponent.h"
#include "Components\AnimatorController\AnimatorController.h"
#include "Components\AnimatorController\Transition.h"
#include "Components\AnimatorController\Animation.h"
#include "Components\AudioSource.h"
#include "Components\Script\ScriptManager.h"
#include "Level\Level.h"
#include "Input\InputConstants.h"

#include "GUIManager.h"
#include "Particles\ParticleManager.h"
#include "ISoundManager.h"
#include "Render\GraphicsStats.h"
#include "Profiler\Profiler.h"
#include "Resources\ResourceManager.h"
//#include "AStar.h"

using namespace luabind;

//CLUAComponent* f(CLUAComponent *Pointer) { return Pointer; }

//CRenderableObject* GetEntity(const std::string &Layer, const std::string &Object)
//{
//	return CEngine::GetSingleton().GetLayerManager()->GetResource(Layer)->GetResource(Object);
//}

void CLuabindManager::RegisterCore()
{
	RegisterComponents();

	module(LUA_STATE)
	[
		class_<CLevel>("CLevel")
		.def(constructor<const std::string&, const std::string&>())
		.def("GetID", &CLevel::GetID)
		.def("Load", &CLevel::Load)
		.def("Unload", &CLevel::Unload)
		.def("GetPercentageLoaded", &CLevel::GetPercentageLoaded)
		.def("AddSoundBankInfoFile", &CLevel::AddSoundBankInfoFile)
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
		class_<CResourceManager>("CResourceManager")
		.def(constructor<>())
		.def("SetCurrentCursor", &CResourceManager::SetCurrentCursor)
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
		.def("GetGameObjectManager", &CEngine::GetGameObjectManager)
		.def("GetLuaGameObjectHandleManager", &CEngine::GetLuaGameObjectHandleManager)
		.def("GetMaterialManager", &CEngine::GetMaterialManager)
		.def("GetAnimatedModelManager", &CEngine::GetAnimatedModelManager)
		.def("GetStaticMeshManager", &CEngine::GetStaticMeshManager)
		
		.def("GetLightManager", &CEngine::GetLightManager)
		.def("GetSceneRendererCommandManager", &CEngine::GetSceneRendererCommandManager)
		
		.def("GetParticleSystemManager", &CEngine::GetParticleSystemManager)

		//.def("GetInputMapper", &CEngine::GetInputMapper)
		.def("GetPhysXManager", &CEngine::GetPhysXManager)
		.def("GetGUIManager", &CEngine::GetGUIManager)
		.def("GetSoundManager", &CEngine::GetSoundManager)
		.def("GetCameraControllerManager", &CEngine::GetCameraControllerManager)
		.def("GetAnimatorControllerManager", &CEngine::GetAnimatorControllerManager)
		.def("GetScriptManager", &CEngine::GetScriptManager)
		.def("GetLayerManager", &CEngine::GetLayerManager)

		.def("GetElapsedTime", &CEngine::GetElapsedTime)
		.def("GetRealTimeSinceStartup", &CEngine::GetRealTimeSinceStartup)
		.def("GetProfiler", &CEngine::GetProfiler)
		.def("GetGraphicsStats", &CEngine::GetGraphicsStats)
		.def("GetTextureManager", &CEngine::GetTextureManager)
		.def("GetRenderManager", &CEngine::GetRenderManager)
		.def("GetLuabindManager", &CEngine::GetLuabindManager)
		.def("GetLogManager", &CEngine::GetLogManager)
		.def("GetDebugHelper", &CEngine::GetDebugHelper)
		.def("IsPaused", &CEngine::IsPaused)
		.def("Pause", &CEngine::Pause)
		.def("Resume", &CEngine::Resume)
		.def("SetTimeScale", &CEngine::SetTimeScale)
		.def("GetPausedLuaAddress", &CEngine::GetPausedLuaAddress)
		.def("GetTimeScaleLuaAddress", &CEngine::GetTimeScaleLuaAddress)
		.def("GetResourceManager", &CEngine::GetResourceManager)
		
		.def("LoadingLevel", &CEngine::LoadingLevel)
		.def("AddLevel", &CEngine::AddLevel)
		.def("LoadLevel", &CEngine::LoadLevel)
		.def("UnloadLevel", &CEngine::UnloadLevel)
		.def("GetCurrentLevel", &CEngine::GetCurrentLevel)
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
		class_<CLuaGameObjectHandleManager>("CLuaGameObjectHandleManager")
		.def(constructor<>())
		.def("Add", &CLuaGameObjectHandleManager::Add)
		.def("Get", &CLuaGameObjectHandleManager::Get)
		.def("Remove", &CLuaGameObjectHandleManager::Remove)
		.def("RemoveCharacterCollider", &CLuaGameObjectHandleManager::RemoveCharacterCollider)
	];

	module(LUA_STATE)
	[
		class_<std::vector<CMaterial*>>("materialsVector")
		.def(constructor<>())
		.def("size", &std::vector<CMaterial*>::size)
		.def("clear", &std::vector<CMaterial*>::clear)
		.def("at", (std::vector<CMaterial*>::reference(std::vector<CMaterial*>::*)(std::vector<CMaterial*>::size_type))&std::vector<CMaterial*>::at)
	];

	module(LUA_STATE)
	[
		class_<CLuaGameObjectHandle>("LuaGameObjectHandle")
		.def(constructor<CGameObject*>())
		
		.def("EnableRenderableObject", &CLuaGameObjectHandle::EnableRenderableObject)
		.def("GetPosition", &CLuaGameObjectHandle::GetPosition)
		.def("SetPosition", &CLuaGameObjectHandle::SetPosition)
		.def("GetForward", &CLuaGameObjectHandle::GetForward)
		.def("GetUp", &CLuaGameObjectHandle::GetUp)
		.def("GetRight", &CLuaGameObjectHandle::GetRight)
		.def("GetYaw", &CLuaGameObjectHandle::GetYaw)
		.def("GetPitch", &CLuaGameObjectHandle::GetPitch)
		.def("GetRoll", &CLuaGameObjectHandle::GetRoll)
		.def("SetYaw", &CLuaGameObjectHandle::SetYaw)
		.def("SetPitch", &CLuaGameObjectHandle::SetPitch)
		.def("SetRoll", &CLuaGameObjectHandle::SetRoll)
		.def("GetAnimatedCoreModel", &CLuaGameObjectHandle::GetAnimatedCoreModel)
		.def("SetParent", &CLuaGameObjectHandle::SetParent)
		.def("GetTransform", &CLuaGameObjectHandle::GetTransform)
		.def("GetBoneTransformationMatrix", &CLuaGameObjectHandle::GetBoneTransformationMatrix)
		.def("SetTemporalRenderableObjectTechnique", &CLuaGameObjectHandle::SetTemporalRenderableObjectTechnique)
		.def("CreateCopyMaterialsFromCore", &CLuaGameObjectHandle::CreateCopyMaterialsFromCore)

		.def("EnableAnimatorController", &CLuaGameObjectHandle::EnableAnimatorController)
		.def("AddState", (CState*(CLuaGameObjectHandle::*)(const std::string &, const std::string &, float, const std::string &, const std::string &, const std::string &))&CLuaGameObjectHandle::AddState)
		.def("AddState", (CState*(CLuaGameObjectHandle::*)(const std::string &, std::vector<const std::string>, float, float, const std::string &, const std::string &, const std::string &))&CLuaGameObjectHandle::AddState)
		.def("GetState", &CLuaGameObjectHandle::GetState)
		.def("AddAnyStateTransition", (CTransition*(CLuaGameObjectHandle::*)(const std::string&, CState*, bool, float))&CLuaGameObjectHandle::AddAnyStateTransition)
		.def("AddAnyStateTransition", (CTransition*(CLuaGameObjectHandle::*)(const std::string&, CState*, bool, float, float))&CLuaGameObjectHandle::AddAnyStateTransition)
		.def("AddInteger", &CLuaGameObjectHandle::AddInteger)
		.def("AddFloat", &CLuaGameObjectHandle::AddFloat)
		.def("AddBool", &CLuaGameObjectHandle::AddBool)
		.def("AddTrigger", &CLuaGameObjectHandle::AddTrigger)
		.def("SetInteger", &CLuaGameObjectHandle::SetInteger)
		.def("SetFloat", &CLuaGameObjectHandle::SetFloat)
		.def("SetBool", &CLuaGameObjectHandle::SetBool)
		.def("SetTrigger", &CLuaGameObjectHandle::SetTrigger)

		.def("GetName", &CLuaGameObjectHandle::GetName)
		.def("GetGameObject", &CLuaGameObjectHandle::GetGameObject)

		.def("GetPhysxMaterial", &CLuaGameObjectHandle::GetPhysxMaterial)

		.def("EnableAudioSource", &CLuaGameObjectHandle::EnableAudioSource)
		.def("AddSound", &CLuaGameObjectHandle::AddSound)
		.def("PlayEvent", (void(CLuaGameObjectHandle::*)(const std::string&))&CLuaGameObjectHandle::PlayEvent)
		.def("PlayEvent", (void(CLuaGameObjectHandle::*)(const std::string&, const std::string&))&CLuaGameObjectHandle::PlayEvent)
		.def("PlayEvent", (void(CLuaGameObjectHandle::*)(const std::string&, const C3DElement*))&CLuaGameObjectHandle::PlayEvent)
		.def("Get3DElement", &CLuaGameObjectHandle::Get3DElement)
	];

	module(LUA_STATE)
	[
		class_<CProfilerRecord>("CProfilerRecord")
		.def(constructor<const std::string&>())
		.def("Start", &CProfilerRecord::Start)
		.def("Stop", &CProfilerRecord::Stop)
		.def("Reset", &CProfilerRecord::Reset)
		.def("RaiseError", &CProfilerRecord::RaiseError)
		.def("Seconds", &CProfilerRecord::Seconds)
		.def("Count", &CProfilerRecord::Count)
	];

	module(LUA_STATE)
	[
		class_<CProfiler>("CProfiler")
		.def(constructor<>())
		.def("Initialize", &CProfiler::Initialize)
		.def("Begin", &CProfiler::Begin)
		.def("End", &CProfiler::End)
	];

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
		.def("Log", (void(CLog::*)(const Mat44f&))&CLog::Log)
		.def("Log", (void(CLog::*)(std::vector<Vect3f>))&CLog::Log)
		//.def("Log", (void(*)(int)) &CLog::Log)
	];

	module(LUA_STATE) 
	[  
		class_<CLuabindManager>("CLuabindManager")   
		.def(constructor<>())
		.def("RunCode", &CLuabindManager::RunCode)
		.def("RunFile", &CLuabindManager::RunFile)
	];  

	module(LUA_STATE)
	[
		class_<CScriptManager>("CScriptManager")
		.def(constructor<>())
		.def("AddComponent", &CScriptManager::AddComponent)
		.def("RemoveComponents", &CScriptManager::RemoveComponents)
	];


	//Obsolete
	/*module(LUA_STATE) 
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
	];*/

	//Obsolete
	/*module(LUA_STATE) 
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
	];*/

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
		.def("RegisterButton", (void(CDebugHelperImplementation::*)(const std::string&, const std::string&, const std::string&))&CDebugHelperImplementation::RegisterLUAButton)
		.def("RegisterButton", (void(CDebugHelperImplementation::*)(const std::string&, const std::string&))&CDebugHelperImplementation::RegisterLUAButton)
		.def("RegisterExtendedButton", (void(CDebugHelperImplementation::*)(const std::string&, const std::string&, CEmptyPointerClass*, const std::string&))&CDebugHelperImplementation::RegisterLUAExtendedButton)
		.def("RegisterExtendedButton", (void(CDebugHelperImplementation::*)(const std::string&, const std::string&, CEmptyPointerClass*, const std::string&, const std::string&))&CDebugHelperImplementation::RegisterLUAExtendedButton)
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
		.def("InsertGroupIntoGroup", &CDebugHelperImplementation::InsertGroupIntoGroup)
		.def("Define", &CDebugHelperImplementation::Define)
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
	CLUAComponent_wrapper(const std::string Name):CLUAComponent(Name){}
	virtual void Update(float ElapsedTime)
	{	
		if (IsEnabled())
		{
			call<void>("Update", ElapsedTime);
		}
	}
	virtual void OnTriggerEnter(const std::string& Actor)
	{
		if (IsEnabled())
		{
			call_member<void>(this,"OnTriggerEnter", Actor);
		}
	}
	virtual void OnTriggerExit(const std::string& Actor)
	{
		if (IsEnabled())
		{
			call<void>("OnTriggerExit", Actor);
		}
	}
	virtual void OnShapeHit(const std::string& Actor)
	{
		if (IsEnabled())
		{
			call<void>("OnShapeHit", Actor);
		}
	}
	static void default_Update(CLUAComponent* ptr, float ElapsedTime)
	{
		return ptr->CLUAComponent::Update(ElapsedTime);
	}
	static void default_OnTriggerEnter(CLUAComponent* ptr, const std::string& Actor)
	{
		return ptr->CLUAComponent::OnTriggerEnter(Actor);
	}
	static void default_OnTriggerExit(CLUAComponent* ptr, const std::string& Actor)
	{
		return ptr->CLUAComponent::OnTriggerExit(Actor);
	}
	static void default_OnShapeHit(CLUAComponent* ptr, const std::string& Actor)
	{
		return ptr->CLUAComponent::OnShapeHit(Actor);
	}
	/*std::string GetType() const
	{
		return call_member<std::string>(this, "GetType");
	}*/
};

void CLuabindManager::RegisterComponents()
{
	module(LUA_STATE)
	[
		class_<CLUAComponent, CLUAComponent_wrapper>("CLUAComponent")
		.def(constructor<const std::string>())
		.def("GetName", &CLUAComponent::GetName)
		.def("Update", &CLUAComponent::Update, &CLUAComponent_wrapper::default_Update)
		.def("OnTriggerEnter", &CLUAComponent::OnTriggerEnter, &CLUAComponent_wrapper::default_OnTriggerEnter)
		.def("OnTriggerExit", &CLUAComponent::OnTriggerExit, &CLUAComponent_wrapper::default_OnTriggerExit)
		.def("OnShapeHit", &CLUAComponent::OnShapeHit, &CLUAComponent_wrapper::default_OnShapeHit)
		.def("AddTime", &CLUAComponent::AddTime)
		.def("GetTimer", &CLUAComponent::GetTimer)
		.def("ResetTimer", &CLUAComponent::ResetTimer)
		//.def("GetType", &CLUAComponent::GetType)
		.def("Update", &CLUAComponent::Update)
		.def("Enable", &CLUAComponent::Enable)
		.def("Disable", &CLUAComponent::Disable)
		.def("IsEnabled", &CLUAComponent::IsEnabled)
		.def("GetThisLuaAddress", &CLUAComponent::GetThisLuaAddress)
		.def("GetEnabledLuaAddress", &CLUAComponent::GetEnabledLuaAddress)
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

	/*module(LUA_STATE)
	[
		class_< CComponentManager, CTemplatedVectorMapManager<CComponent>>("CComponentManager")
		.def(constructor<>())
		.def("Update", &CComponentManager::Update)
		.def("Render", &CComponentManager::Render)
		.def("RenderDebug", &CComponentManager::RenderDebug)
		.def("AddComponent", &CComponentManager::AddComponent)
	];*/

	module(LUA_STATE)
	[
		class_<CComponent, CNamed>("CComponent")
		//.def(constructor<const std::string&, CRenderableObject*>())
		.def("Update", &CComponent::Update)
		.def("Render", &CComponent::Render)
		.def("RenderDebug", &CComponent::RenderDebug)
		.def("GetOwner", &CComponent::GetOwner)
		.def("GetEnabledLuaAddress", &CComponent::GetEnabledLuaAddress)
	];

	module(LUA_STATE)
	[
		class_<CScript, CComponent>("CScript")
		.def(constructor<const std::string&, CGameObject*,CLUAComponent*>())
		.def("GetLuaComponent", &CScript::GetLuaComponent)
		.def("Update", &CScript::Update)
	];

	module(LUA_STATE)
	[
		class_<CCollider, CComponent>("CCollider")
		.def(constructor<const std::string&, CGameObject*>())
		.def("GetPhysxMaterial", &CCollider::GetPhysxMaterial)
	];

	module(LUA_STATE)
	[
		class_<CCharacterCollider, CComponent>("CCharacterCollider")
		.def(constructor<const std::string&, CGameObject*>())
		.def("GetPhysxMaterial",&CCharacterCollider::GetPhysxMaterial)
	];

	module(LUA_STATE)
	[
		class_<CTransition>("CTransition")
		.def(constructor<CState*, const bool, const float, const float >())

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
		class_<EAnimation>("EAnimation")
		.def(constructor<const std::string&, bool, unsigned int, float, float, bool>())
		.def(constructor<>())
		.def_readonly("m_Loop", &EAnimation::m_Loop)
		.def_readonly("m_ID", &EAnimation::m_ID)
		.def_readonly("m_Weight", &EAnimation::m_Weight)
		.def_readonly("m_Duration", &EAnimation::m_Duration)
		.def_readonly("m_AutoLock", &EAnimation::m_AutoLock)
	];

	module(LUA_STATE)
	[
		class_<std::vector<EAnimation> >("EAnimationVector")
		.def(constructor<>())
		.def("size", &std::vector<EAnimation>::size)
		.def("clear", &std::vector<EAnimation>::clear)
		.def("at", (std::vector<EAnimation>::reference(std::vector<EAnimation>::*)(std::vector<EAnimation>::size_type))&std::vector<EAnimation>::at)
		.def("push_back", (void(std::vector<EAnimation>::*)(const EAnimation&))&std::vector<EAnimation>::push_back)
		//.def("push_back", (void(std::vector<EAnimation>::*)(EAnimation&&))&std::vector<EAnimation>::push_back)
	];

	module(LUA_STATE)
	[
		class_<CState>("CState")
		.def(constructor<CAnimatorController*, const std::string, const EAnimation, const float, const std::string, const std::string, const std::string>())
		.def(constructor<CAnimatorController*, const std::string, const std::vector<EAnimation>&, float, const float, const std::string, const std::string, const std::string>())
		.def("AddTransition", (CTransition* (CState::*)(const std::string&, CState*, bool, float, float))&CState::AddTransition)
		.def("AddTransition", (CTransition* (CState::*)(const std::string&, CState*, bool, float))&CState::AddTransition)
		.def("OnEnter", &CState::OnEnter)
		.def("OnUpdate", &CState::OnUpdate)
		.def("OnExit", &CState::OnExit)
		.def("GetOwnAnimatorController", &CState::GetOwnAnimatorController)
		.def("GetAnimation", &CState::GetAnimation)
		.def("GetCurrentAnimation", &CState::GetCurrentAnimation)
	];

	module(LUA_STATE)
	[
		class_<CAnimatorController>("CAnimatorController")
		/*.def(constructor<const std::string, CGameObject*>())
		.def("AddState", &CAnimatorController::AddState)
		.def("AddInteger", &CAnimatorController::AddInteger)
		.def("AddFloat", &CAnimatorController::AddFloat)
		.def("AddBool", &CAnimatorController::AddBool)
		.def("AddTrigger", &CAnimatorController::AddTrigger)
		.def("SetInteger", &CAnimatorController::SetInteger)
		.def("SetFloat", &CAnimatorController::SetFloat)
		.def("SetBool", &CAnimatorController::SetBool)
		.def("SetTrigger", &CAnimatorController::SetTrigger)
		.def("AddAnyStateTransition", &CAnimatorController::AddAnyStateTransition)*/
	];

	module(LUA_STATE)
	[
		class_<CAudioSource>("CAudioSource")
		/*.def(constructor<const std::string, CGameObject*>())
		.def("RemoveSounds", &CAudioSource::RemoveSounds)
		.def("PlayEvent", &CAudioSource::PlayEvent)
		.def("AddSound", &CAudioSource::AddSound)*/
	];
}