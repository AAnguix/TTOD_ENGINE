#ifndef _AUDIOSOURCE_H
#define _AUDIOSOURCE_H

#include "Components\Component.h"
#include "ISoundManager.h"
#include <vector>

class CAudioSource : public CComponent
{
public:
	enum ESpeakerType{ BACKGROUND_MUSIC, STATIC, DYNAMIC };

private:
	ESpeakerType m_SpeakerType;

	struct ESoundEventPair
	{
		std::string m_Play;
		std::string m_Stop;
	};

public:
	static CAudioSource* AddAudioSource(const std::string &Name, CRenderableObject *Owner, ESpeakerType SpeakerType);

	CAudioSource(const std::string &Name, CRenderableObject *Owner, ESpeakerType SpeakerTyoe);
	virtual ~CAudioSource();
	
	void PlayEvent(const std::string &EventName);
};

#endif