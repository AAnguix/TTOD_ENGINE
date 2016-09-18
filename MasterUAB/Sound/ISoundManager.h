#ifndef _ISOUNDMANAGER_H
#define _ISOUNDMANAGER_H

#include <string>
#include <vector>
#include "Math\Vector3.h"

class CCamera;
class C3DElement;
class CAudioSource;
class CGameObject;
class CLuaGameObjectHandle;

struct SoundEvent
{
	std::string m_EventName;
	SoundEvent(const std::string &EventName):m_EventName(EventName){};
};
struct SoundSwitch
{
	std::string m_SwitchName;
	SoundSwitch(const std::string &SwitchName):m_SwitchName(SwitchName){};
};
struct SoundSwitchValue
{
	SoundSwitch m_SoundSwitch;
	std::string m_ValueName;
	SoundSwitchValue(const std::string &SwitchName, const std::string &ValueName) :m_SoundSwitch(SoundSwitch(SwitchName)),m_ValueName(ValueName){};
};
struct SoundRTPC
{
	std::string m_RtpcName;
	SoundRTPC(const std::string &RtpcName):m_RtpcName(RtpcName){};
};
struct SoundStateValue
{
	std::string m_ValueName;
	SoundStateValue(const std::string &StateValue):m_ValueName(StateValue){};
};


class ISoundManager
{

protected:
	ISoundManager(){}

public:
	static ISoundManager* CreateSoundManager();
	virtual ~ISoundManager() {}

protected:
	std::string m_Path;

public:
	void SetPath(const std::string& Path){ m_Path = Path; }

	virtual CAudioSource* AddComponent(const std::string &Name, CLuaGameObjectHandle* Owner) = 0;
	virtual void RemoveComponent(CGameObject* Owner) = 0;
	virtual void RemoveComponents() = 0;

	virtual bool Init() = 0;
	virtual void Update(const CCamera *Camera, const Vect3f &Forward, float ElapsedTime) = 0;
	virtual bool Load(const std::string& SoundBanksFilename, const std::string& SpeakersFilename) = 0;
	virtual bool Reload() = 0;

	virtual bool LoadSoundBank(const std::string& Bank) = 0;
	virtual bool UnloadSoundBank(const std::string& Bank) = 0;
	virtual void ClearNamedSpeakers() = 0;

	virtual void RegisterSpeaker(const C3DElement* Speaker) = 0;
	virtual void UnregisterSpeaker(const C3DElement* Speaker) = 0;

	virtual void PlayEvent(const SoundEvent& Event) = 0;
	virtual void PlayEvent(const SoundEvent& Event, const std::string& Speaker) = 0;
	virtual void PlayEvent(const SoundEvent& Event, const C3DElement* Speaker) = 0;

	virtual void SetSwitch(const SoundSwitchValue& SwitchValue) = 0;
	virtual void SetSwitch(const SoundSwitchValue& SwitchValue, const std::string& Speaker) = 0;
	virtual void SetSwitch(const SoundSwitchValue& SwitchValue, const C3DElement* Speaker) = 0;

	virtual void BroadcastRTPCValue(const SoundRTPC& Rtpc, float Value) = 0;
	virtual void SetRTPCValue(const SoundRTPC& Rtpc, float Value) = 0;
	virtual void SetRTPCValue(const SoundRTPC& Rtpc, float Value, const std::string& Speaker) = 0;
	virtual void SetRTPCValue(const SoundRTPC& Rtpc, float Value, const C3DElement* Speaker) = 0;

	virtual void BroadcastState(const SoundStateValue &State) = 0;

	virtual SoundEvent GetSoundEvent(const std::string &SoundEventName) = 0;
};

#endif