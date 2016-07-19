#include "Components\AudioSource.h"
#include "Engine\Engine.h"
#include "GameObject\GameObject.h"
#include <assert.h>
#include "ISoundManager.h"
#include "Log\Log.h"

CAudioSource::CAudioSource(const std::string &Name, CGameObject *Owner)
:CComponent(Name, Owner)
{
}

CAudioSource::~CAudioSource()
{
	m_SoundsToPlay.clear();
	m_Sounds.clear();
}

void CAudioSource::Update(float ElapsedTime)
{
	for (size_t i = 0; i < m_SoundsToPlay.size();i++)
	{
		CEngine::GetSingleton().GetSoundManager()->PlayEvent(m_SoundsToPlay[i]);
	}
	m_SoundsToPlay.clear();
}

void CAudioSource::PlayEvent(const std::string &Key)
{
	bool l_Found = false;
	if (IsEnabled())
	{
		for (unsigned int i = 0; i < m_Sounds.size(); ++i)
		{
			if (m_Sounds[i].first == Key)
			{
				m_SoundsToPlay.push_back(m_Sounds[i].second);
				l_Found = true;
				i = m_Sounds.size();
			}
		}
		assert(l_Found);
	}
}

bool CAudioSource::SoundAdded(const std::string &SoundEventName)
{
	for (size_t i = 0; i < m_Sounds.size();++i)
	{
		if (m_Sounds[i].second.m_EventName == SoundEventName)
			return true;
	}
	return false;
}

bool CAudioSource::AddSound(const std::string &Key, const std::string &SoundEventName)
{
	SoundEvent l_SoundEvent = CEngine::GetSingleton().GetSoundManager()->GetSoundEvent(SoundEventName);
	if (l_SoundEvent.m_EventName != "")
	{
		if (!SoundAdded(SoundEventName))
		{
			m_Sounds.push_back(std::make_pair(Key, l_SoundEvent));
			return true;
		}
		#ifdef _DEBUG
			CEngine::GetSingleton().GetLogManager()->Log("Sound " + SoundEventName + " already exists in GameObject "+m_Owner->GetName());
		#endif
	}

	return false;
}

void CAudioSource::RemoveSounds()
{
	m_Sounds.clear();
}
