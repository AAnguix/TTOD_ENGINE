#include "SoundManager.h"
#include "Math\Vector3.h"
#include "Utils\3DElement.h"
#include "XML\XMLTreeNode.h"
#include "Camera.h"
#include <assert.h>
#include "Components\AudioSource.h"
#include "GameObject\GameObject.h"
#include "Engine\Engine.h"
#include "Log\Log.h"

namespace AK
{
#ifdef WIN32
	void * AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}
	void FreeHook(void * in_ptr)
	{
		free(in_ptr);
	}

	void * VirtualAllocHook(void * in_pMemAddress, size_t in_size, DWORD in_dwAllocationType, DWORD in_dwProtect)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}
	void VirtualFreeHook(void * in_pMemAddress, size_t in_size, DWORD in_dwFreeType)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
#endif
}

ISoundManager* ISoundManager::CreateSoundManager()
{
	return new CSoundManager();
}

CSoundManager::CSoundManager() 
:m_SoundBanksFilename("")
,m_SpeakersFilename("")
,m_InitOk(false)
{
	
}

CSoundManager::~CSoundManager()
{
	RemoveComponents();
	Terminate();
}

/*Public*/
void CSoundManager::PlayEvent(const SoundEvent &Event)
{
	PlayEvent(Event, m_DefaultSpeakerID);
}
void CSoundManager::PlayEvent(const SoundEvent &Event, const std::string& Speaker)
{
	std::unordered_map<std::string, AkGameObjectID>::iterator it;
	it = m_NamedSpeakers.find(Speaker);
	if (it != m_NamedSpeakers.end())
	{
		PlayEvent(Event, it->second);
	}
	else
	{
		assert(false);
	}
}
void CSoundManager::PlayEvent(const SoundEvent &Event, const C3DElement* Speaker)
{
	std::unordered_map<const C3DElement*, AkGameObjectID>::iterator it;
	it = m_GameObjectSpeakers.find(Speaker);
	if (it != m_GameObjectSpeakers.end())
	{
		PlayEvent(Event, it->second);
	}
	else
	{
		assert(false);
	}
}

void CSoundManager::SetSwitch(const SoundSwitchValue &SwitchValue)
{
	SetSwitch(SwitchValue, m_DefaultSpeakerID);
}
void CSoundManager::SetSwitch(const SoundSwitchValue &SwitchValue, const std::string& Speaker)
{
	std::unordered_map<std::string, AkGameObjectID>::iterator it;
	it = m_NamedSpeakers.find(Speaker);
	if (it != m_NamedSpeakers.end())
	{
		SetSwitch(SwitchValue, it->second);
	}
	else
	{
		assert(false);
	}
}
void CSoundManager::SetSwitch(const SoundSwitchValue &SwitchValue, const C3DElement* Speaker)
{
	std::unordered_map<const C3DElement*, AkGameObjectID>::iterator it;
	it = m_GameObjectSpeakers.find(Speaker);
	if (it != m_GameObjectSpeakers.end())
	{
		SetSwitch(SwitchValue, it->second);
	}
	else
	{
		assert(false);
	}
}

void CSoundManager::SetRTPCValue(const SoundRTPC& Rtpc, float Value)
{
	SetRTPCValue(Rtpc, Value, m_DefaultSpeakerID);
}
void CSoundManager::SetRTPCValue(const SoundRTPC& Rtpc, float Value, const std::string& Speaker)
{
	std::unordered_map<std::string, AkGameObjectID>::iterator it;
	it = m_NamedSpeakers.find(Speaker);
	if (it != m_NamedSpeakers.end())
	{
		SetRTPCValue(Rtpc, Value, it->second);
	}
	else
	{
		assert(false);
	}
}
void CSoundManager::SetRTPCValue(const SoundRTPC& Rtpc, float Value, const C3DElement* Speaker)
{
	std::unordered_map<const C3DElement*, AkGameObjectID>::iterator it;
	it = m_GameObjectSpeakers.find(Speaker);
	if (it != m_GameObjectSpeakers.end())
	{
		SetRTPCValue(Rtpc, Value, it->second);
	}
	else
	{
		assert(false);
	}
}

void CSoundManager::BroadcastRTPCValue(const SoundRTPC& Rtpc, float Value)
{
	//AKRESULT l_Result = AK::SoundEngine::SetRTPCValue(Rtpc.RTPCName.c_str(), AkRtpcValue()Value); TODO
	//assert (l_Result);
}
void CSoundManager::BroadcastState(const SoundStateValue& State)
{
	//AKRESULT l_Result = AK::SoundEngine::SetState(State.soundState.stateName.c_str(), State.valueName.c_str()); TODO
	//assert (l_Result);
}

void CSoundManager::RegisterSpeaker(const C3DElement* Speaker)
{
	assert(m_GameObjectSpeakers.find(Speaker) == m_GameObjectSpeakers.end());

	AkGameObjectID l_ID = GenerateObjectID();
	m_GameObjectSpeakers[Speaker] = l_ID;

	Vect3f l_Position = Speaker->GetPosition();
	float l_Yaw = Speaker->GetYaw();
	float l_Pitch = Speaker->GetPitch();

	Vect3f l_Orientation(cos(l_Yaw)*cos(l_Pitch), sin(l_Pitch), sin(l_Yaw)*cos(l_Pitch));
	//Vect3f l_Orientation = Speaker->GetForward(); TODO

	AkSoundPosition l_SoundPosition = {};
	l_SoundPosition.Position.X = l_Position.x;
	l_SoundPosition.Position.Y = l_Position.y;
	l_SoundPosition.Position.Z = l_Position.z;

	l_SoundPosition.Orientation.X = l_Orientation.x;
	l_SoundPosition.Orientation.Y = l_Orientation.y;
	l_SoundPosition.Orientation.Z = l_Orientation.z;

	AK::SoundEngine::RegisterGameObj(l_ID);
	AK::SoundEngine::SetPosition(l_ID, l_SoundPosition);
}
void CSoundManager::UnregisterSpeaker(const C3DElement* Speaker)
{
	auto it = m_GameObjectSpeakers.find(Speaker);
	if (it != m_GameObjectSpeakers.end())
	{
		AK::SoundEngine::UnregisterGameObj(it->second);
		m_FreeObjectIDs.push_back(it->second);
		m_GameObjectSpeakers.erase(it);
	}
	else { assert(false); }
}


/*Private*/
void CSoundManager::Terminate()
{
	AK::SoundEngine::ClearBanks();
	if (m_InitOk)
	{
		AK::SoundEngine::UnregisterAllGameObj();
		AK::SOUNDENGINE_DLL::Term();
	}
}

void CSoundManager::Clean()
{
	AK::SoundEngine::ClearBanks();

	std::unordered_map<std::string, AkGameObjectID>::iterator it;
	for (it = m_NamedSpeakers.begin(); it != m_NamedSpeakers.end(); it++)
	{
		AK::SoundEngine::UnregisterGameObj(it->second);
	}
	m_NamedSpeakers.clear();
}

bool CSoundManager::Init()
{
	//Initialize audio engine

	//Memory
	AkMemSettings l_MemSettings;
	l_MemSettings.uMaxNumPools = 20;

	//Streaming
	AkStreamMgrSettings l_StmSettings;
	AK::StreamMgr::GetDefaultSettings(l_StmSettings);

	AkDeviceSettings l_DeviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(l_DeviceSettings);

	AkInitSettings l_InitSettings;
	AkPlatformInitSettings l_PlatformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(l_InitSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(l_PlatformInitSettings);

	//Setting pool sizes for this game
	//Memoria usada para todos los sonidos que se cargan en el juego
	l_InitSettings.uDefaultPoolSize = 4 * 1024 * 1024;
	l_InitSettings.uMaxNumPaths = 16;
	l_InitSettings.uMaxNumTransitions = 128;

	//Memoria usada para todos los sonidos reproducidos simultaneamente
	l_PlatformInitSettings.uLEngineDefaultPoolSize = 4 * 1024 * 1024;

	AkMusicSettings l_MusicInit;
	AK::MusicEngine::GetDefaultInitSettings(l_MusicInit);

	AKRESULT l_EResult = AK::SOUNDENGINE_DLL::Init(&l_MemSettings, &l_StmSettings, &l_DeviceSettings, &l_InitSettings, &l_PlatformInitSettings, &l_MusicInit);
	if (l_EResult != AK_Success)
	{
		CEngine::GetSingleton().GetLogManager()->Log("Error initializing WWise.");
		//Return game without sound
		AK::SOUNDENGINE_DLL::Term();
		return false;
	}
	else
	{
		CEngine::GetSingleton().GetLogManager()->Log("WWise initialized properly.");
		m_InitOk = true;
	}

	m_DefaultSpeakerID = GenerateObjectID();
	AK::SoundEngine::RegisterGameObj(m_DefaultSpeakerID);
	return true;
}

bool CSoundManager::Load(const std::string& SoundBanksFilename, const std::string& SpeakersFilename)
{
	m_SoundBanksFilename = SoundBanksFilename;
	m_SpeakersFilename = SpeakersFilename;

	bool l_IsOk = true;
	InitBanks();
	l_IsOk = LoadSoundBanksXML();
	l_IsOk = LoadSpeakersXML();

	return l_IsOk;
}

bool CSoundManager::Reload()
{
	Clean();
	RemoveComponents();
	return Load(m_SoundBanksFilename, m_SpeakersFilename);
}

void CSoundManager::Update(const CCamera *Camera, float ElapsedTime)
{
	std::unordered_map<const C3DElement*, AkGameObjectID>::iterator it;

	for (it = m_GameObjectSpeakers.begin(); it != m_GameObjectSpeakers.end(); ++it)
	{
		Vect3f l_Position = it->first->GetPosition();
		float l_Yaw = it->first->GetYaw();
		float l_Pitch = it->first->GetPitch();

		Vect3f l_Orientation(cos(l_Yaw)*cos(l_Pitch), sin(l_Pitch), sin(l_Yaw)*cos(l_Pitch));

		AkSoundPosition l_SoundPosition = {};
		l_SoundPosition.Position.X = l_Position.x;
		l_SoundPosition.Position.Y = l_Position.y;
		l_SoundPosition.Position.Z = l_Position.z;

		l_SoundPosition.Orientation.X = l_Orientation.x;
		l_SoundPosition.Orientation.Y = l_Orientation.y;
		l_SoundPosition.Orientation.Z = l_Orientation.z;

		AK::SoundEngine::SetPosition(it->second, l_SoundPosition);
	}

	SetListenerPosition(Camera);

	UpdateComponents(ElapsedTime);

	AK::SOUNDENGINE_DLL::Tick();
}

void CSoundManager::UpdateComponents(float ElapsedTime)
{
	for (size_t i = 0; i < m_Components.size(); ++i)
	{
		m_Components[i]->Update(ElapsedTime);
	}
}

CAudioSource* CSoundManager::AddComponent(const std::string &Name, CGameObject *Owner)
{
	bool l_Found = false;
	CAudioSource* l_AudioSource = nullptr;
	for (size_t i = 0; i < m_Components.size(); ++i)
	{
		if (m_Components[i]->GetName() == Name)
		{
			l_Found = true;
			i = m_Components.size();
		}
	}
	if (!l_Found)
	{
		l_AudioSource = new CAudioSource(Name, Owner);
		Owner->SetAudioSource(l_AudioSource);
		m_Components.push_back(l_AudioSource);
	}
	else
	{
		assert(false);
		delete(l_AudioSource); l_AudioSource = NULL;
	}

	return l_AudioSource;
}

void CSoundManager::RemoveComponents()
{
	for (size_t i = 0; i < m_Components.size(); ++i)
	{
		delete m_Components[i];
		m_Components[i] = NULL;
	}
	m_Components.clear();
}

void CSoundManager::SetListenerPosition(const CCamera *Camera)
{
	Vect3f l_Position = Camera->GetPosition();
	//Vect3f l_Orientation = Camera->GetForward(); //TODO
	Vect3f l_Orientation = Camera->GetLookAt();
	Vect3f l_VectorUp = Camera->GetUp();

	AkListenerPosition l_ListenerPosition = {};

	l_ListenerPosition.Position.X = l_Position.x;
	l_ListenerPosition.Position.Y = l_Position.y;
	l_ListenerPosition.Position.Z = l_Position.z;

	l_ListenerPosition.OrientationFront.X = l_Orientation.x;
	l_ListenerPosition.OrientationFront.Y = l_Orientation.y;
	l_ListenerPosition.OrientationFront.Z = l_Orientation.z;

	l_ListenerPosition.OrientationTop.X = l_VectorUp.x;
	l_ListenerPosition.OrientationTop.Y = l_VectorUp.y;
	l_ListenerPosition.OrientationTop.Z = l_VectorUp.z;

	AK::SoundEngine::SetListenerPosition(l_ListenerPosition);
}

bool CSoundManager::LoadSoundBanksXML()
{
	CXMLTreeNode l_XML;

	if (l_XML.LoadFile((m_Path + m_SoundBanksFilename).c_str()))
	{
		CXMLTreeNode l_Soundbanks = l_XML["SoundBanks"];

		if (l_Soundbanks.Exists())
		{
			for (int i = 0; i < l_Soundbanks.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Soundbanks(i);

				if (l_Element.GetName() == std::string("SoundBank"))
				{

					for (int j = 0; j < l_Element.GetNumChildren(); ++j)
					{
						CXMLTreeNode l_SoundBankElement = l_Element(j);

						if (l_SoundBankElement.GetName() == std::string("Path"))
						{
							std::string l_Path = l_SoundBankElement.GetPszKeyword("Path");
							if (l_Path != "Init.bnk")
								LoadSoundBank(l_Path);
						}
						if (l_SoundBankElement.GetName() == std::string("IncludedEvents"))
						{
							for (int k = 0; k < l_SoundBankElement.GetNumChildren(); ++k)
							{
								CXMLTreeNode l_SoundEvent = l_SoundBankElement(k);
								if (l_SoundEvent.GetName() == std::string("Event"))
								{
									std::string l_SoundBankID = l_SoundEvent.GetPszProperty("Name", "");
									m_SoundEvents.push_back(l_SoundBankID);
								}
							}
						}
					}
				}
			}
		}
	}

	return true;
}
bool CSoundManager::InitBanks()
{
	//Load initialization and main soundbanks
	AkOSChar *l_Path;
	AKRESULT l_ReturnValue;
	CONVERT_CHAR_TO_OSCHAR(m_Path.c_str(), l_Path);
	l_ReturnValue = AK::SOUNDENGINE_DLL::SetBasePath(l_Path);
	l_ReturnValue = AK::StreamMgr::SetCurrentLanguage(L"English(US)");

	AkBankID l_BankID;

	l_ReturnValue = AK::SoundEngine::LoadBank("Init.bnk", AK_DEFAULT_POOL_ID, l_BankID);
	if (l_ReturnValue != AK_Success)
	{
		return false;
	}

	return true;
}
bool CSoundManager::LoadSoundBank(const std::string& Bank)
{
	AkBankID l_BankID;
	AKRESULT l_ReturnValue;

	l_ReturnValue = AK::SoundEngine::LoadBank(Bank.c_str(), AK_DEFAULT_POOL_ID, l_BankID);
	if (l_ReturnValue != AK_Success)
	{
		return false;
	}

	return true;
}
bool CSoundManager::UnloadSoundBank(const std::string& Bank)
{
	//AkBankID l_BankID;
	AKRESULT l_ReturnValue;

	l_ReturnValue = AK::SoundEngine::UnloadBank(Bank.c_str(), nullptr);
	if (l_ReturnValue != AK_Success)
	{
		return false;
	}

	return true;
}

bool CSoundManager::LoadSpeakersXML()
{
	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(m_SpeakersFilename.c_str()))
	{
		CXMLTreeNode l_Speakers = l_XML["speakers"];

		if (l_Speakers.Exists())
		{
			for (int i = 0; i < l_Speakers.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Speaker = l_Speakers(i);

				if (l_Speaker.GetName() == std::string("speaker"))
				{
					std::string l_Name = l_Speaker.GetPszProperty("name", "");
					Vect3f l_Position = l_Speaker.GetVect3fProperty("position", v3fZERO);
					Vect3f l_Orientation = l_Speaker.GetVect3fProperty("orientation", v3fZERO);

					AkSoundPosition l_SoundPosition = {};
					l_SoundPosition.Position.X = l_Position.x;
					l_SoundPosition.Position.Y = l_Position.y;
					l_SoundPosition.Position.Z = l_Position.z;

					l_SoundPosition.Orientation.X = l_Orientation.x;
					l_SoundPosition.Orientation.Y = l_Orientation.y;
					l_SoundPosition.Orientation.Z = l_Orientation.z;

					AkGameObjectID l_ID = GenerateObjectID();
					m_NamedSpeakers[l_Name] = l_ID;
					AK::SoundEngine::RegisterGameObj(l_ID);
					AK::SoundEngine::SetPosition(l_ID, l_SoundPosition);
				}
			}
		}
	}

	return true;
}

AkGameObjectID CSoundManager::GenerateObjectID()
{
	AkGameObjectID l_Result;
	if (m_FreeObjectIDs.size() > 0)
	{
		l_Result = m_FreeObjectIDs.back();
		m_FreeObjectIDs.pop_back();
	}
	else
	{
		l_Result = ++m_LastGameObjectID;
	}
	return l_Result;
}

void CSoundManager::PlayEvent(const SoundEvent& Event, const AkGameObjectID& ID)
{
	AK::SoundEngine::PostEvent(Event.m_EventName.c_str(), ID); //TODO 
}
void CSoundManager::SetSwitch(const SoundSwitchValue &SwitchValue, const AkGameObjectID& ID)
{
	//AKRESULT l_Result = AK::SoundEngine::SetSwitch(SwitchValue.soundSwitch.switchName.c_str(), SwitchValue.valueName.c_str(), ID); //TODO
	//assert(l_Result);
}
void CSoundManager::SetRTPCValue(const SoundRTPC &Rtpc, float Value, const AkGameObjectID& ID)
{
	AKRESULT l_Result = AK::SoundEngine::SetRTPCValue(Rtpc.m_RtpcName.c_str(), AkRtpcValue(Value), ID);
	assert(l_Result);
}

SoundEvent CSoundManager::GetSoundEvent(const std::string &SoundEventName)
{
	for (size_t i = 0; i < m_SoundEvents.size(); i++)
	{
		if (SoundEventName == m_SoundEvents[i])
		{
			return m_SoundEvents[i];
		}
	}
	#ifdef _DEBUG
		CEngine::GetSingleton().GetLogManager()->Log("Sound " + SoundEventName + " not found");
	#endif
	return SoundEvent("");
}


