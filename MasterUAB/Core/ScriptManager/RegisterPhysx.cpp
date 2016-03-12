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
		.def("Reload", &CPhysXManager::Reload)
	];

	module(LUA_STATE) 
	[
		class_<CPhysXManager::CharacterControllerData>("CharacterControllerData")
	];
}