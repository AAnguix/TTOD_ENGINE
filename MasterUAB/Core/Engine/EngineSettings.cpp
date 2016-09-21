#include "Engine\EngineSettings.h"
#include "XML\XMLTreeNode.h"
#include "Engine\Engine.h"
#include <assert.h>
#include "Log\Log.h"

CEngineSettings::CEngineSettings()
:m_FullScreen(false)
,m_VSyncEnabled(false)
,m_ScreenSize(Vect2i(800, 600))
,m_ScreenResolution(Vect2i(1280, 720))
,m_ScreenPosition(Vect2i(0, 0))
,m_DataPath("")
,m_D3DDebug(false)
,m_GuiMaterials("")
,m_EffectsMaterials("")
{

}

CEngineSettings::~CEngineSettings()
{

}

bool CEngineSettings::LoadSettings(const std::string &SettingsFile)
{
	m_SettingsFile = SettingsFile;
	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(SettingsFile.c_str()))
	{
		CXMLTreeNode l_Settings = l_XML["engine_settings"];

		if (l_Settings.Exists())
		{
			for (int i = 0; i < l_Settings.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Settings(i);

				if (l_Element.GetName() == std::string("window"))
				{
					m_FullScreen = l_Element.GetBoolProperty("full_screen");
					m_ScreenPosition = l_Element.GetVect2iProperty("screen_position", Vect2i(0, 0));
					m_ScreenSize = l_Element.GetVect2iProperty("screen_size", Vect2i(800, 600));
					m_ScreenResolution = l_Element.GetVect2iProperty("screen_resolution", Vect2i(1280, 760));
				}
				else if (l_Element.GetName() == std::string("debug_options"))
				{
					m_DebugOptions.m_DebugAnimatorController = l_Element.GetBoolProperty("animator_controller", true);
					m_DebugOptions.m_DebugScriptManager = l_Element.GetBoolProperty("script_manager", true);
					m_DebugOptions.m_DebugInputMapper = l_Element.GetBoolProperty("input_mapper", true);
					m_DebugOptions.m_DebugPhysXTriggers = l_Element.GetBoolProperty("physx_triggers", true);
				}
				else if (l_Element.GetName() == std::string("fps"))
				{
					m_Fps = l_Element.GetIntProperty("value", 30);
				}
				else if (l_Element.GetName() == std::string("vsync"))
				{
					m_VSyncEnabled = l_Element.GetBoolProperty("value", false);
				}
				else if (l_Element.GetName() == std::string("d3d_debug"))
				{
					m_D3DDebug = l_Element.GetBoolProperty("value", false);
				}

				else if (l_Element.GetName() == std::string("gui_materials"))
				{
					m_GuiMaterials = l_Element.GetPszProperty("value", false);
				}
				else if (l_Element.GetName() == std::string("effects_materials"))
				{
					m_EffectsMaterials = l_Element.GetPszProperty("value", false);
				}
				else if (l_Element.GetName() == std::string("data_path"))
				{
					m_DataPath = l_Element.GetPszProperty("value","");
					assert(m_DataPath != "");
				}
			}

			return true;
		}
		else { assert(false); }
	}
	else { assert(false); }

	return false;
}