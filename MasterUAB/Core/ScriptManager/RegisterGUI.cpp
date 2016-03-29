#include "ScriptManager\ScriptManager.h"

#include <luabind/luabind.hpp>

#include <luabind/operator.hpp>
#include <luabind/iterator_policy.hpp>

#include "Engine.h"
#include "GUIManager.h"

using namespace luabind;

#define LUA_STATE CEngine::GetSingleton().GetScriptManager()->GetLuaState()
#define REGISTER_LUA_FUNCTION(FunctionName,AddrFunction) {luabind::module(LUA_STATE) [ luabind::def(FunctionName,AddrFunction) ];}

void CScriptManager::RegisterGUI()
{
	/*module(LUA_STATE)
	[
		class_<CTemplatedMapManager<CMaterial>::TMapResource>("TMapResources")
		.def("size", &CTemplatedMapManager<CMaterial>::TMapResource::size)
	];*/

	module(LUA_STATE)
	[
		class_<CGUIManager>("CGUIManager")
		.def("AddButton", &CGUIManager::AddButton)
		.def("AddSlider", &CGUIManager::AddSlider)
		.def("DoButton", &CGUIManager::DoButton)
		.def("DoSlider", &CGUIManager::DoSlider)
		.def("Reload", &CGUIManager::Reload)
		.enum_("GUICoordType")
		[
			value("GUI_ABSOLUTE", 0),
			value("GUI_RELATIVE", 1),
			value("GUI_RELATIVE_WIDTH", 2),
			value("GUI_RELATIVE_HEIGHT", 3)
		]
		.enum_("GUIAnchor")
		[
			value("TOP", 0x1),
			value("MID", 0x2),
			value("BOTTOM", 0x4),

			value("LEFT", 0x10),
			value("CENTER", 0x20),
			value("RIGHT", 0x40),

			value("TOP_LEFT", 0x17),
			value("TOP_CENTER", 0x33),
			value("TOP_RIGHT", 0x65),
			value("MID_LEFT", 0x18),
			value("MID_CENTER", 0x34),
			value("MID_RIGHT", 0x66),
			value("BOTTOM_LEFT", 0x20),
			value("BOTTOM_CENTER", 0x36),
			value("BOTTOM_RIGHT", 0x68)
		]
	];

	module(LUA_STATE)
	[
		class_<CGUIManager::SGUIPosition>("SGUIPosition")
		.def(constructor<float, float, float, float>())
		//.def(constructor<float, float, float, float, CGUIManager::GUIAnchor, CGUIManager::GUICoordType, CGUIManager::GUICoordType>())
		.def_readwrite("x", &CGUIManager::SGUIPosition::x)
		.def_readwrite("y", &CGUIManager::SGUIPosition::y)
		.def_readwrite("width", &CGUIManager::SGUIPosition::width)
		.def_readwrite("height", &CGUIManager::SGUIPosition::height)
	];
	module(LUA_STATE)
	[
		class_<CGUIManager::SSliderResult>("SSliderResult")
		.def_readwrite("x", &CGUIManager::SSliderResult::real)
		.def_readwrite("x", &CGUIManager::SSliderResult::temp)
	];
}