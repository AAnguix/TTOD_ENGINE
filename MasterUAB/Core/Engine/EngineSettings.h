#ifndef _ENGINESETTINGS_H
#define _ENGINESETTINGS_H

#include <string>
#include "Math\Vector2.h"

/*Class that stores the Engine Global Settings*/

class CEngineSettings
{

private:

	std::string m_SettingsFile;
	bool m_FullScreen;
	bool m_VSyncEnabled;
	size_t m_Fps;
	
	Vect2i m_ScreenSize;
	Vect2i m_ScreenResolution;
	Vect2i m_ScreenPosition;

	std::string m_DataPath;
	bool m_D3DDebug;

	std::string m_GuiMaterials;
	std::string m_EffectsMaterials;

public:
	CEngineSettings();
	virtual ~CEngineSettings();

	bool GetFullScreen() { return m_FullScreen; };
	Vect2i GetScreenSize() const { return m_ScreenSize; };
	Vect2i GetScreenPosition() const { return m_ScreenPosition; };
	Vect2i GetScreenResolution() const { return m_ScreenResolution; };
	bool VSyncEnabled() const { return m_VSyncEnabled; };

	std::string GetEffectsMaterialsFileName() const{ return m_EffectsMaterials; };
	std::string GetGuiMaterialsFileName() const{ return m_GuiMaterials; };

	bool LoadSettings(const std::string &SettingsFile);
	
};

#endif