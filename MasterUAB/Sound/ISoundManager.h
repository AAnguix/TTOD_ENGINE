#ifndef _ISOUNDMANAGER_H
#define _ISOUNDMANAGER_H

#include <string>

class CCamera;
class C3DElement;

struct SoundEvent
{
	std::string m_EventName;
	SoundEvent(const std::string EventName) :m_EventName(EventName){};
};
struct SoundSwitch
{
	std::string m_SwitchName;
};
struct SoundSwitchValue
{
	SoundSwitch m_SoundSwitch;
};
struct SoundRTPC
{
	std::string rtpcName;
};
struct SoundStateValue
{
	std::string valueName;
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

	virtual bool Init() = 0;
	virtual void Update(const CCamera *Camera) = 0;
	virtual bool Load(const std::string& SoundBanksFilename, const std::string& SpeakersFilename) = 0;
	virtual bool Reload() = 0;

	virtual bool LoadSoundBank(const std::string& Bank) = 0;
	virtual bool UnloadSoundBank(const std::string& Bank) = 0;

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
};

#endif