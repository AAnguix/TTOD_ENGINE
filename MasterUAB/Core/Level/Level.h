#ifndef _LEVEL_H
#define _LEVEL_H

#include <string>
class CEngine;

class CLevel
{
private:
	std::string m_ID;

	std::string m_MaterialsFilename;
	std::string m_ParticleSystemsFilename;
	std::string m_StaticMeshesFilename;
	std::string m_AnimatedModelsFilename;
	std::string m_RenderableObjectsFilename;
	std::string m_LightsFilename;

	std::string m_SoundBankFilename;
	std::string m_SpeakersFilename;

	std::string m_SceneRendererCommandsFilename;

public:
	CLevel(const std::string &ID);
	virtual ~CLevel();
	std::string GetID() const{ return m_ID; };

	bool Load(CEngine& Engine);
	bool Unload(CEngine& Engine);
};

#endif