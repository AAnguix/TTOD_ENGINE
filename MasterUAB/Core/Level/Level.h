#ifndef _LEVEL_H
#define _LEVEL_H

class CEngine;
#include <string>
#include "GUIManager.h"

class CLevel
{
private:
	std::string m_ID;
	std::string m_LuaLoadFunction;

	std::string m_MaterialsFilename;
	std::string m_ParticleSystemsFilename;
	std::string m_StaticMeshesFilename;
	std::string m_AnimatedModelsFilename;
	std::string m_RenderableObjectsFilename;
	std::string m_LightsFilename;

	std::string m_SoundBankPath;
	std::vector<const std::string> m_SoundBanksInfoFiles;
	std::vector<const std::string> m_SoundBanks;
	std::string m_SpeakersFilename;

	std::string m_SceneRendererCommandsFilename;

	unsigned int m_PercentageLoaded;
	void UpdateLoadPercentageLevel(unsigned int Percentage, CGUIManager::SSliderResult& SliderResult);

public:
	CLevel(const std::string &ID, const std::string LuaLoadFunction);
	virtual ~CLevel();
	std::string GetID() const{ return m_ID; };
	unsigned int GetPercentageLoaded() const{ return m_PercentageLoaded; }

	bool Load(CEngine& Engine);
	bool Unload(CEngine& Engine);

	void AddSoundBankInfoFile(const std::string SoundBankInfoFile){ m_SoundBanksInfoFiles.push_back(SoundBankInfoFile); }
};

#endif