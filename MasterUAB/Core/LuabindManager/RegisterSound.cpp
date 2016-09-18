#include "LuabindManager\LuabindManager.h"
#include "LuabindManager\LuabindMacros.h"
#include <luabind/luabind.hpp>

#include <luabind/operator.hpp>

#include "Engine\Engine.h"
#include "ISoundManager.h"
#include "Components\AudioSource.h"

using namespace luabind;

void CLuabindManager::RegisterSound()
{
	module(LUA_STATE)
	[
		class_<SoundEvent>("SoundEvent")
		.def(constructor<const std::string&>())
		.def_readonly("m_EventName", &SoundEvent::m_EventName)
	];
	module(LUA_STATE)
	[
		class_<SoundSwitch>("SoundSwitch")
		.def(constructor<const std::string&>())
		.def_readonly("m_SwitchName", &SoundSwitch::m_SwitchName)
	];
	module(LUA_STATE)
	[
		class_<SoundSwitchValue>("SoundSwitchValue")
		.def(constructor<const std::string&, const std::string&>())
		.def_readonly("m_SoundSwitch", &SoundSwitchValue::m_SoundSwitch)
		.def_readonly("m_ValueName", &SoundSwitchValue::m_ValueName)
	];
	module(LUA_STATE)
	[
		class_<SoundRTPC>("SoundRTPC")
		.def(constructor<const std::string&>())
		.def_readonly("m_RtpcName", &SoundRTPC::m_RtpcName)
	];
	module(LUA_STATE)
	[
		class_<SoundStateValue>("SoundStateValue")
		.def(constructor<const std::string&>())
		.def_readonly("m_ValueName", &SoundStateValue::m_ValueName)
	];
	module(LUA_STATE)
	[
		class_<ISoundManager>("ISoundManager")
		.scope
		[
			def("CreateSoundManager", &ISoundManager::CreateSoundManager)
		]
		.def("SetPath", &ISoundManager::SetPath)
		.def("Init", &ISoundManager::Init)
		.def("Update", &ISoundManager::Update)
		.def("Load", &ISoundManager::Load)
		.def("Reload", &ISoundManager::Reload)

		.def("AddComponent", &ISoundManager::AddComponent)
		.def("RemoveComponents", &ISoundManager::RemoveComponents)

		.def("LoadSoundBank", &ISoundManager::LoadSoundBank)
		.def("UnloadSoundBank", &ISoundManager::UnloadSoundBank)

		.def("RegisterSpeaker", &ISoundManager::RegisterSpeaker)
		.def("UnregisterSpeaker", &ISoundManager::UnregisterSpeaker)

		.def("PlayEvent", (void(ISoundManager::*)(const SoundEvent&))&ISoundManager::PlayEvent)
		.def("PlayEvent", (void(ISoundManager::*)(const SoundEvent&, const std::string&))&ISoundManager::PlayEvent)
		.def("PlayEvent", (void(ISoundManager::*)(const SoundEvent& Event, const C3DElement*))&ISoundManager::PlayEvent)

		.def("SetSwitch", (void(ISoundManager::*)(const SoundSwitchValue& SwitchValue))&ISoundManager::SetSwitch)
		.def("SetSwitch", (void(ISoundManager::*)(const SoundSwitchValue& SwitchValue, const std::string&))&ISoundManager::SetSwitch)
		.def("SetSwitch", (void(ISoundManager::*)(const SoundSwitchValue& SwitchValue, const C3DElement*))&ISoundManager::SetSwitch)

		.def("BroadcastRTPCValue", &ISoundManager::BroadcastRTPCValue)
		.def("SetRTPCValue", (void(ISoundManager::*)(const SoundRTPC&, float))&ISoundManager::SetRTPCValue)
		.def("SetRTPCValue", (void(ISoundManager::*)(const SoundRTPC&, float, const std::string&))&ISoundManager::SetRTPCValue)
		.def("SetRTPCValue", (void(ISoundManager::*)(const SoundRTPC&, float, const C3DElement*))&ISoundManager::SetRTPCValue)

		.def("BroadcastState", &ISoundManager::BroadcastState)
	];
}