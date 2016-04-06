#include "ScriptManager\ScriptManager.h"

#include <luabind/luabind.hpp>

#include <luabind/operator.hpp>
//#include <luabind/iterator_policy.hpp>

#include "Engine.h"

using namespace luabind;

#define LUA_STATE CEngine::GetSingleton().GetScriptManager()->GetLuaState()
#define REGISTER_LUA_FUNCTION(FunctionName,AddrFunction) {luabind::module(LUA_STATE) [ luabind::def(FunctionName,AddrFunction) ];}


void CScriptManager::RegisterSound()
{
	/*module(LUA_STATE)
	[
		class_<ISoundManager>("ISoundManager")
		.scope
		[
			def("CreateSoundManager", &ISoundManager::CreateSoundManager)
		]
		.def("Log", (void(ISoundManager::*)(const SoundEvent&))&ISoundManager::PlayEvent)
		.def("Log", (void(ISoundManager::*)(const SoundEvent&, const std::string&))&ISoundManager::PlayEvent)
	];*/
}