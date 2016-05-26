#include "Components\AudioSource.h"
#include "Engine.h"
#include "RenderableObjects\RenderableObject.h"
#include <assert.h>

CAudioSource::CAudioSource(const std::string &Name, CRenderableObject *Owner)
	:CComponent(Name, Owner)
{


}

CAudioSource::~CAudioSource()
{

}

CAudioSource* CAudioSource::AddAudioSource(const std::string &Name, CRenderableObject *Owner)
{
	CAudioSource* l_AudioSource = new CAudioSource(Name, Owner);

	if (!Owner->AddComponent(l_AudioSource))
	{
		CHECKED_DELETE(l_AudioSource);
	}

	return l_AudioSource;
}


void CAudioSource::PlayEvent(const std::string &Key)
{
	bool l_Found = false;
	if (IsEnabled())
	{
		for (unsigned int i = 0; i < m_Sounds.size(); ++i)
		{
			if (m_Sounds[i].first == Key)

				//CEngine::GetSingleton().GetSoundManager()->PlayEvent(m_Sounds[i].second, m_Owner);
				CEngine::GetSingleton().GetSoundManager()->PlayEvent(m_Sounds[i].second);
			l_Found = true;
		}
		assert(l_Found);
	}
}

bool CAudioSource::AddSound(const std::string &Key, const std::string &SoundEventName)
{
	SoundEvent l_SoundEvent = CEngine::GetSingleton().GetSoundManager()->GetSoundEvent(SoundEventName);
	if (l_SoundEvent.m_EventName != "")
	{
		m_Sounds.push_back(std::make_pair(Key, l_SoundEvent));
		return true;
	}
	return false;
}

void CAudioSource::RemoveSounds()
{
	m_Sounds.clear();
}
