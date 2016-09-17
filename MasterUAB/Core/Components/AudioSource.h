#ifndef _AUDIOSOURCE_H
#define _AUDIOSOURCE_H

#include "Components\Component.h"
#include <vector>
struct SoundEvent;
class C3DElement;

class CAudioSource : public CComponent
{
private:
	std::vector<std::pair<std::string, SoundEvent>> m_Sounds;
	std::vector<SoundEvent> m_DefaultSpeakerSoundsToPlay;
	std::vector<std::pair<SoundEvent, const std::string>> m_NamedSpeakersSoundsToPlay;
	std::vector<std::pair<SoundEvent, const C3DElement*>> m_GameObjectSpeakersSoundsToPlay;


	bool SoundAdded(const std::string &SoundEventName);

public:
	CAudioSource(const std::string &Name, CGameObject *Owner);
	virtual ~CAudioSource();

	void Update(float ElapsedTime);
	bool AddSound(const std::string &Key, const std::string &SoundEventName);
	void PlayEvent(const std::string &Key);
	void PlayEvent(const std::string &Key, const std::string& Speaker);
	void PlayEvent(const std::string &Key, const C3DElement* Speaker);
	void RemoveSounds(); 
};

#endif