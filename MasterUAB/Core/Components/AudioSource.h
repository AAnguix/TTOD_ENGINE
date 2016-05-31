#ifndef _AUDIOSOURCE_H
#define _AUDIOSOURCE_H

#include "Components\Component.h"
#include "ISoundManager.h"
#include <vector>

class CAudioSource : public CComponent
{
private:
	std::vector<std::pair<std::string, SoundEvent>> m_Sounds;

public:
	static CAudioSource* AddAudioSource(const std::string &Name, CRenderableObject *Owner);

	CAudioSource(const std::string &Name, CRenderableObject *Owner);
	virtual ~CAudioSource();

	bool AddSound(const std::string &Key, const std::string &SoundEventName);
	void PlayEvent(const std::string &Key);
	void RemoveSounds(); 
};

#endif