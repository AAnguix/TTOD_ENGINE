#include "Components\AudioSource.h"
#include "Engine\Engine.h"
#include "Utils\GameObject.h"
#include <assert.h>
#include "ISoundManager.h"

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
