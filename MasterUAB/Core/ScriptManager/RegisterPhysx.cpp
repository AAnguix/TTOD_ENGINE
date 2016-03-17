#include "ScriptManager\ScriptManager.h"

#include <luabind/luabind.hpp>

#include <luabind/operator.hpp>
#include <luabind/iterator_policy.hpp>


#include "Engine.h"
#include "PhysXManager.h"

using namespace luabind;

#define LUA_STATE CEngine::GetSingleton().GetScriptManager()->GetLuaState()
#define REGISTER_LUA_FUNCTION(FunctionName,AddrFunction) {luabind::module(LUA_STATE) [ luabind::def(FunctionName,AddrFunction) ];}

void CScriptManager::RegisterPhysics()
{
	module(LUA_STATE) 
	[
		class_<CPhysXManager>("CPhysXManager")
		.def("MoveCharacterController", &CPhysXManager::MoveCharacterController)
		.def("GetCharacterControllerPosition", &CPhysXManager::GetCharacterControllerPosition)
		.def("GetCharacterControllerFootPosition", &CPhysXManager::GetCharacterControllerFootPosition)
		.def("GetActorPosition", &CPhysXManager::GetActorPosition)
		.def("GetActorOrientation", &CPhysXManager::GetActorOrientation)
		.def("GetActorPositionAndOrientation", &CPhysXManager::GetActorPositionAndOrientation)
		.def("SetShapeAsTrigger", &CPhysXManager::SetShapeAsTrigger)
		.def("Raycast", &CPhysXManager::Raycast)
		.def("MoveKinematicActor", (void(CPhysXManager::*)(const std::string&, const Vect3f&))&CPhysXManager::MoveKinematicActor)
		.def("MoveKinematicActor", (void(CPhysXManager::*)(const std::string&, const Quatf&))&CPhysXManager::MoveKinematicActor)
		.def("MoveKinematicActor", (void(CPhysXManager::*)(const std::string&, const Vect3f&, const Quatf&))&CPhysXManager::MoveKinematicActor)
		.def("Reload", &CPhysXManager::Reload)
	];
	module(LUA_STATE) 
	[
		class_<CPhysXManager::SCharacterControllerData>("SCharacterControllerData")
	];
	module(LUA_STATE)
	[
		class_<CPhysXManager::SRaycastData>("SRaycastData")
	];
	module(LUA_STATE)
	[
		class_<CPhysXManager::SActorData>("SActorData")
	];
}