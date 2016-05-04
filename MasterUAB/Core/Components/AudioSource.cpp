#include "Components\AudioSource.h"
#include "Engine.h"
#include "RenderableObjects\RenderableObject.h"

CAudioSource::CAudioSource(const std::string &Name, CRenderableObject *Owner, ESpeakerType SpeakerType)
:CComponent(Name,Owner)
,m_SpeakerType(SpeakerType)
{
	
	
}

CAudioSource::~CAudioSource()
{
	
}

CAudioSource* CAudioSource::AddAudioSource(const std::string &Name, CRenderableObject *Owner, ESpeakerType SpeakerType)
{
	CAudioSource* l_AudioSource = new CAudioSource(Name, Owner, SpeakerType);

	if (!Owner->AddComponent(l_AudioSource))
	{
		CHECKED_DELETE(l_AudioSource);
	}

	return l_AudioSource;
}


void CAudioSource::PlayEvent(const std::string &EventName)
{
	if (IsEnabled())
	{
		if (m_SpeakerType == STATIC)
			CEngine::GetSingleton().GetSoundManager()->PlayEvent(EventName,m_Owner->GetName());
		else if (m_SpeakerType == DYNAMIC)
			CEngine::GetSingleton().GetSoundManager()->PlayEvent(EventName,m_Owner);
		else if (m_SpeakerType == BACKGROUND_MUSIC)
			CEngine::GetSingleton().GetSoundManager()->PlayEvent(EventName);
	}
}