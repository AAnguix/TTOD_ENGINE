#ifndef _SOUNDMANAGER_H
#define _SOUNDMANAGER_H

#include "ISoundManager.h"
#include <vector>
#include <unordered_map>

#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/IBytes.h>
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <AkFilePackageLowLevelIOBlocking.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AkSoundEngineDLL.h>

//#pragma comment(lib,"AkSoundEngineDLL.lib")

class CSoundManager : public ISoundManager
{

private:
	std::vector<CAudioSource*> m_Components;
	void UpdateComponents(float ElapsedTime);
public:
	CAudioSource* AddComponent(const std::string &Name, CLuaGameObjectHandle *Owner);
	void RemoveComponent(CGameObject* Owner);
	void RemoveComponents();

private:
	std::vector<std::string> m_SoundEvents;
	AkGameObjectID m_LastGameObjectID; //Only > 0
	std::vector<AkGameObjectID> m_FreeObjectIDs;

	AkGameObjectID m_DefaultSpeakerID;
	std::unordered_map<std::string, AkGameObjectID> m_NamedSpeakers;
	std::unordered_map<const C3DElement*, AkGameObjectID> m_GameObjectSpeakers;

	std::string m_SoundBanksFilename;
	std::string m_SpeakersFilename;
	bool m_InitOk;

	AkGameObjectID GenerateObjectID();
	bool LoadSoundBanksXML();
	bool LoadSpeakersXML();
	bool InitBanks();
	void Terminate();
	void Clean();

	void ClearGameObjectSpeakers();

	void SetListenerPosition(const CCamera *Camera, const Vect3f &Forward);

	void PlayEvent(const SoundEvent& Event, const AkGameObjectID& ID);
	void SetSwitch(const SoundSwitchValue &SwitchValue, const AkGameObjectID& ID);
	void SetRTPCValue(const SoundRTPC &Rtpc, float Value, const AkGameObjectID& ID);

public:
	CSoundManager();
	virtual ~CSoundManager();

	bool Init();
	void Update(const CCamera *Camera, const Vect3f &Forward, float ElapsedTime);
	bool Load(const std::string& SoundBanksFilename, const std::string& SpeakersFilename);
	bool Reload();

	bool LoadSoundBank(const std::string& Bank);
	bool UnloadSoundBank(const std::string& Bank);
	void ClearNamedSpeakers();

	void RegisterSpeaker(const C3DElement* Speaker);
	void UnregisterSpeaker(const C3DElement* Speaker);

	void PlayEvent(const SoundEvent& Event);
	void PlayEvent(const SoundEvent& Event, const std::string& Speaker);
	void PlayEvent(const SoundEvent& Event, const C3DElement* Speaker);

	void SetSwitch(const SoundSwitchValue& SwitchValue);
	void SetSwitch(const SoundSwitchValue& SwitchValue, const std::string& Speaker);
	void SetSwitch(const SoundSwitchValue& SwitchValue, const C3DElement* Speaker);

	void BroadcastRTPCValue(const SoundRTPC &Rtpc, float Value);
	void SetRTPCValue(const SoundRTPC& Rtpc, float Value);
	void SetRTPCValue(const SoundRTPC& Rtpc, float Value, const std::string& Speaker);
	void SetRTPCValue(const SoundRTPC& Rtpc, float Value, const C3DElement* Speaker);

	void BroadcastState(const SoundStateValue &State);

	SoundEvent GetSoundEvent(const std::string &SoundEventName);
};

#endif