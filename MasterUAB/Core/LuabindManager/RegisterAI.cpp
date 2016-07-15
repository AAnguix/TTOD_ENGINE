#include "LuabindManager\LuabindManager.h"
#include "LuabindManager\LuabindMacros.h"
#include <luabind/luabind.hpp>

#include "Engine\Engine.h"
#include "AStar.h"

using namespace luabind;

void CLuabindManager::RegisterAI()
{
	module(LUA_STATE)
	[
		class_<CAStar>("CAStar")
		.def(constructor<>())
		.def("SearchPath", &CAStar::SearchPath)
		.def("LoadNodes", &CAStar::LoadNodes)
	];
}