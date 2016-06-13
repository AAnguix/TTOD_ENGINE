#ifndef _AUDIOSOURCE_H
#define _AUDIOSOURCE_H

#include "Components\Component.h"
#include <vector>
struct SoundEvent;

class CAudioSource : public CComponent
{
private:
	std::vector<std::pair<std::string, SoundEvent>> m_Sounds;
	std::vector<SoundEvent> m_SoundsToPlay;

public:
	CAudioSource(const std::string &Name, CGameObject *Owner);
	virtual ~CAudioSource();

	void Update(float ElapsedTime);
	bool AddSound(const std::string &Key, const std::string &SoundEventName);
	void PlayEvent(const std::string &Key);
	void RemoveSounds(); 
};

#endif