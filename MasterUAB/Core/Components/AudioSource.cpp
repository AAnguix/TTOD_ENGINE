#include "Components\AudioSource.h"
#include "Engine\Engine.h"
#include "GameObject\GameObject.h"
#include <assert.h>
#include "ISoundManager.h"
#include "Log\Log.h"
#include "Utils\3DElement.h"

CAudioSource::CAudioSource(const std::string &Name, CGameObject *Owner)
:CComponent(Name, Owner)
{
}

CAudioSource::~CAudioSource()
{
	m_DefaultSpeakerSoundsToPlay.clear();
	m_NamedSpeakersSoundsToPlay.clear();
	m_GameObjectSpeakersSoundsToPlay.clear();
	m_Sounds.clear();
}

void CAudioSource::Update(float ElapsedTime)
{
	for (size_t i = 0; i < m_DefaultSpeakerSoundsToPlay.size();i++)
	{
		CEngine::GetSingleton().GetSoundManager()->PlayEvent(m_DefaultSpeakerSoundsToPlay[i]);
	}
	m_DefaultSpeakerSoundsToPlay.clear();

	for (size_t i = 0; i < m_NamedSpeakersSoundsToPlay.size(); i++)
	{
		CEngine::GetSingleton().GetSoundManager()->PlayEvent(m_NamedSpeakersSoundsToPlay[i].first, m_NamedSpeakersSoundsToPlay[i].second);
	}
	m_NamedSpeakersSoundsToPlay.clear();

	for (size_t i = 0; i < m_GameObjectSpeakersSoundsToPlay.size(); i++)
	{
		CEngine::GetSingleton().GetSoundManager()->PlayEvent(m_GameObjectSpeakersSoundsToPlay[i].first, m_GameObjectSpeakersSoundsToPlay[i].second);
	}
	m_GameObjectSpeakersSoundsToPlay.clear();
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
				m_DefaultSpeakerSoundsToPlay.push_back(m_Sounds[i].second);
				l_Found = true;
				i = m_Sounds.size();
			}
		}
		#ifdef _DEBUG
		if (!l_Found)
		{
			LOG("Unable to find key " + Key);
			assert(false);
		}
		#endif
	}
}

void CAudioSource::PlayEvent(const std::string &Key, const std::string& Speaker)
{
	bool l_Found = false;
	if (IsEnabled())
	{
		for (unsigned int i = 0; i < m_Sounds.size(); ++i)
		{
			if (m_Sounds[i].first == Key)
			{
				m_NamedSpeakersSoundsToPlay.push_back(std::pair<SoundEvent, const std::string&>(m_Sounds[i].second, Speaker));
				l_Found = true;
				i = m_Sounds.size();
			}
		}
		if (!l_Found)
		{
			LOG("Unable to find key " + Key);
			assert(false);
		}
	}
}

void CAudioSource::PlayEvent(const std::string &Key, const C3DElement* Speaker)
{
	bool l_Found = false;
	if (IsEnabled())
	{
		for (unsigned int i = 0; i < m_Sounds.size(); ++i)
		{
			if (m_Sounds[i].first == Key)
			{
				m_GameObjectSpeakersSoundsToPlay.push_back(std::pair<SoundEvent, const C3DElement*>(m_Sounds[i].second, Speaker));
				l_Found = true;
				i = m_Sounds.size();
			}
		}
		if (!l_Found)
		{
			LOG("Unable to find key " + Key);
			assert(false);
		}
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
	else
	{
		#ifdef _DEBUG
			CEngine::GetSingleton().GetLogManager()->Log("SoundEvent " + SoundEventName + " not found");
		#endif
	}

	return false;
}

void CAudioSource::RemoveSounds()
{
	m_Sounds.clear();
}
