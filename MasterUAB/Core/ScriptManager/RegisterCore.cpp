#include "ScriptManager\ScriptManager.h"

#include <luabind/luabind.hpp>

#include <luabind/operator.hpp>
#include <luabind/iterator_policy.hpp>


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
#include "Components\ScriptedComponent.h"
#include "Components\PhysxComponent.h"
//#include "AStar.h"

using namespace luabind;

#define LUA_STATE CEngine::GetSingleton().GetScriptManager()->GetLuaState()
#define REGISTER_LUA_FUNCTION(FunctionName,AddrFunction) {luabind::module(LUA_STATE) [ luabind::def(FunctionName,AddrFunction) ];}

void CScriptManager::RegisterCore()
{
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
		
		.def("GetEffectManager", &CEngine::GetEffectManager)
		.def("GetRenderableObjectTechniqueManager", &CEngine::GetRenderableObjectTechniqueManager)
		.def("GetMaterialManager", &CEngine::GetMaterialManager)
		.def("GetAnimatedModelManager", &CEngine::GetAnimatedModelManager)
		.def("GetStaticMeshManager", &CEngine::GetStaticMeshManager)
		.def("GetLayerManager", &CEngine::GetLayerManager)
		.def("GetLightManager", &CEngine::GetLightManager)
		.def("GetSceneRendererCommandManager", &CEngine::GetSceneRendererCommandManager)
		.def("GetCameraControllerManager", &CEngine::GetCameraControllerManager)

		.def("GetTextureManager", &CEngine::GetTextureManager)
		.def("GetRenderManager", &CEngine::GetRenderManager)
		.def("GetScriptManager", &CEngine::GetScriptManager)
		.def("GetLogManager", &CEngine::GetLogManager)
		.def("GetDebugHelper", &CEngine::GetDebugHelper)
		.def("GetPhysXManager", &CEngine::GetPhysXManager)
	];

	/*Components*/
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
		.def(constructor<const std::string&,CRenderableObject*>())
		.def("Update", &CComponent::Update)
		.def("Render", &CComponent::Render)
		.def("RenderDebug", &CComponent::RenderDebug)
		.def("GetOwner", &CComponent::GetOwner)
	];

	module(LUA_STATE) 
	[
		class_<CScriptedComponent, CComponent>("CScriptedComponent")
		.def(constructor<const std::string&,CRenderableObject*,const std::string&,const std::string&,const std::string&,const std::string&,const std::string&>())
		.scope
		[
			def("CreateScriptedComponent", &CScriptedComponent::CreateScriptedComponent) 
		]
	];
	
	module(LUA_STATE) 
	[
		class_<CPhysxComponent, CComponent>("CPhysxComponent")
		.def(constructor<const std::string&,CRenderableObject*,bool>())
		.scope
		[
			def("CreatePhysxComponent", &CPhysxComponent::CreatePhysxComponent) 
		]
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
		//.def("write_log", (void(*)(int)) &CLog::Log)
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
		.def(constructor<HWND>())
		.def("load_commands_from_file", &CInputManagerImplementation::LoadCommandsFromFile)
		.def("begin_frame", &CInputManagerImplementation::BeginFrame)
		.def("end_frame", &CInputManagerImplementation::EndFrame)
		.def("set_focus", &CInputManagerImplementation::SetFocus)
		.def("set_mouse_speed", &CInputManagerImplementation::SetMouseSpeed)
		.def("key_event_received", &CInputManagerImplementation::KeyEventReceived)
		.def("update_cursor", &CInputManagerImplementation::UpdateCursor)
		.def("update_cursor_movement", &CInputManagerImplementation::UpdateCursorMovement)
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
}