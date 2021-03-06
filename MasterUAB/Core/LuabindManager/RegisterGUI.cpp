#include "LuabindManager\LuabindManager.h"
#include "LuabindManager\LuabindMacros.h"
#include <luabind/luabind.hpp>

#include <luabind/operator.hpp>
#include <luabind/iterator_policy.hpp>

#include "Engine\Engine.h"
#include "Render\ContextManager.h"
#include "GUIManager.h"

#include <luabind/out_value_policy.hpp>

using namespace luabind;

void CLuabindManager::RegisterGUI()
{
	/*module(LUA_STATE)
	[
		class_<CTemplatedMapManager<CMaterial>::TMapResource>("TMapResources")
		.def("size", &CTemplatedMapManager<CMaterial>::TMapResource::size)
	];*/

	module(LUA_STATE)
	[
		class_<CGUIManager::SGUIPosition>("SGUIPosition")
		.def(constructor<float, float, float, float>())
		.def(constructor<float, float, float, float, CGUIManager::GUIAnchor, CGUIManager::GUICoordType, CGUIManager::GUICoordType>())
		.def_readwrite("x", &CGUIManager::SGUIPosition::x)
		.def_readwrite("y", &CGUIManager::SGUIPosition::y)
		.def_readwrite("width", &CGUIManager::SGUIPosition::width)
		.def_readwrite("height", &CGUIManager::SGUIPosition::height)
	];


	module(LUA_STATE)
	[
		class_<CGUIManager::SSpriteMapInfo>("SSpriteMapInfo")
		.def(constructor<const std::string&, int, int , int >())
	];

	module(LUA_STATE)
	[
		class_<CGUIManager::SSpriteInfo>("SSpriteInfo")
		.def(constructor<CGUIManager::SSpriteMapInfo*, float, float, float, float>())
	];

	module(LUA_STATE)
	[
		class_<CGUIManager::SSliderResult>("SSliderResult")
		.def_readwrite("real", &CGUIManager::SSliderResult::real)
		.def_readwrite("temp", &CGUIManager::SSliderResult::temp)
		.def_readwrite("hot", &CGUIManager::SSliderResult::hot)
		.def_readwrite("active", &CGUIManager::SSliderResult::active)
		.def(constructor<>())
	];

	module(LUA_STATE)
	[
		class_<CGUIManager::SButtonState>("SButtonState")
		.def_readonly("hot", &CGUIManager::SButtonState::hot)
		.def_readonly("active", &CGUIManager::SButtonState::active)
		.def_readonly("pressed", &CGUIManager::SButtonState::pressed)
		.def(constructor<>())
	];

	module(LUA_STATE)
	[
		class_<CGUIManager>("CGUIManager")
		.def("AddImage", &CGUIManager::AddImage)
		.def("AddButton", &CGUIManager::AddButton)
		.def("AddSlider", (void(CGUIManager::*)(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&))&CGUIManager::AddSlider)
		.def("AddSlider", (void(CGUIManager::*)(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, int, int))&CGUIManager::AddSlider)
	
		.def("AddHealthBar", (void(CGUIManager::*)(const std::string&, const std::string&, const std::string&))&CGUIManager::AddHealthBar)
		.def("AddHealthBar", (void(CGUIManager::*)(const std::string&, const std::string&, const std::string&, const std::string&))&CGUIManager::AddHealthBar)

		.def("AddFont", &CGUIManager::AddFont)
		.def("DoImage", (void(CGUIManager::*)(const std::string&, const std::string&, const const CGUIManager::SGUIPosition& Position))&CGUIManager::DoImage)
		.def("DoImage", (void(CGUIManager::*)(const std::string&, const std::string&, const const CGUIManager::SGUIPosition& Position, const CColor &Color))&CGUIManager::DoImage)
		.def("DoButton", (bool(CGUIManager::*)(const std::string&, const std::string&, const const CGUIManager::SGUIPosition&))&CGUIManager::DoButton)
		.def("DoButton", (bool(CGUIManager::*)(const std::string&, const std::string&, const const CGUIManager::SGUIPosition&, const CColor &))&CGUIManager::DoButton)
		.def("DoSButton", (CGUIManager::SButtonState(CGUIManager::*)(const std::string&, const std::string&, const const CGUIManager::SGUIPosition&, const CColor &))&CGUIManager::DoSButton)

		.def("DoSlider", &CGUIManager::DoSlider)
		.def("DoHealthBar", &CGUIManager::DoHealthBar)
		.def("DoTextBox", &CGUIManager::DoTextBox)
		.def("DoText", (void(CGUIManager::*)(const std::string&, const std::string&, const CGUIManager::SGUIPosition&, const std::string&, const std::string&))&CGUIManager::DoText)
		.def("DoText", (void(CGUIManager::*)(const std::string&, const std::string&, const CGUIManager::SGUIPosition&, const std::string&, const std::string&, const CColor&))&CGUIManager::DoText)
		.def("CreateConsole", &CGUIManager::CreateConsole)
		.def("Destroy", &CGUIManager::Destroy)
		.def("Load", &CGUIManager::Load)
		.def("Reload", &CGUIManager::Reload)
		.enum_("GUICoordType")
		[
			value("GUI_ABSOLUTE", CGUIManager::GUICoordType::GUI_ABSOLUTE),
			value("GUI_RELATIVE", CGUIManager::GUICoordType::GUI_RELATIVE),
			value("GUI_RELATIVE_WIDTH", CGUIManager::GUICoordType::GUI_RELATIVE_WIDTH),
			value("GUI_RELATIVE_HEIGHT", CGUIManager::GUICoordType::GUI_RELATIVE_HEIGHT)
		]
		.enum_("GUIAnchor")
		[
			value("TOP", CGUIManager::GUIAnchor::TOP),
			value("MID", CGUIManager::GUIAnchor::MID),
			value("BOTTOM", CGUIManager::GUIAnchor::BOTTOM),

			value("LEFT", CGUIManager::GUIAnchor::LEFT),
			value("CENTER", CGUIManager::GUIAnchor::CENTER),
			value("RIGHT", CGUIManager::GUIAnchor::RIGHT),
			value("BASE", CGUIManager::GUIAnchor::BASE),

			value("TOP_LEFT", CGUIManager::GUIAnchor::TOP_LEFT),
			value("TOP_CENTER", CGUIManager::GUIAnchor::TOP_CENTER),
			value("TOP_RIGHT", CGUIManager::GUIAnchor::TOP_RIGHT),
			value("MID_LEFT", CGUIManager::GUIAnchor::MID_LEFT),
			value("MID_CENTER", CGUIManager::GUIAnchor::MID_CENTER),
			value("MID_RIGHT", CGUIManager::GUIAnchor::MID_RIGHT),
			value("BOTTOM_LEFT", CGUIManager::GUIAnchor::BOTTOM_LEFT),
			value("BOTTOM_CENTER", CGUIManager::GUIAnchor::BOTTOM_CENTER),
			value("BOTTOM_RIGHT", CGUIManager::GUIAnchor::BOTTOM_RIGHT),
			value("BASE_LEFT", CGUIManager::GUIAnchor::BASE_LEFT),
			value("BASE_CENTER", CGUIManager::GUIAnchor::BASE_CENTER),
			value("BASE_RIGHT", CGUIManager::GUIAnchor::BASE_RIGHT)

			/*value("TOP", 1),
			value("MID", 2),
			value("BOTTOM", 4),

			value("LEFT", 16),
			value("CENTER", 32),
			value("RIGHT", 64),
			value("BASE", 128),

			value("TOP_LEFT", 23),
			value("TOP_CENTER", 51),
			value("TOP_RIGHT", 101),
			value("MID_LEFT", 24),
			value("MID_CENTER", 52),
			value("MID_RIGHT", 102),
			value("BOTTOM_LEFT", 32),
			value("BOTTOM_CENTER", 54),
			value("BOTTOM_RIGHT", 104),
			value("BASE_LEFT", 324),
			value("BASE_CENTER", 352),
			value("BASE_RIGHT", 402)*/
		]
	];

	/*module(LUA_STATE)
	[
		class_<CGUIManager::GUIAnchor>("GUIAnchor")
		.enum_("GUIAnchor")
		[
			value("TOP", 1),
			value("MID", 2),
			value("BOTTOM", 4),

			value("LEFT", 16),
			value("CENTER", 32),
			value("RIGHT", 64),
			value("BASE", 128),

			value("TOP_LEFT", 23),
			value("TOP_CENTER", 51),
			value("TOP_RIGHT", 101),
			value("MID_LEFT", 24),
			value("MID_CENTER", 52),
			value("MID_RIGHT", 102),
			value("BOTTOM_LEFT", 32),
			value("BOTTOM_CENTER", 54),
			value("BOTTOM_RIGHT", 104),
			value("BASE_LEFT", 324),
			value("BASE_CENTER", 352),
			value("BASE_RIGHT", 402)
		]
	];

	module(LUA_STATE)
	[
		class_<CGUIManager::GUICoordType>("GUICoordType")
		.enum_("GUICoordType")
		[
			value("GUI_ABSOLUTE", 0),
			value("GUI_RELATIVE", 1),
			value("GUI_RELATIVE_WIDTH", 2),
			value("GUI_RELATIVE_HEIGHT", 3)
		]
	];*/

	
}