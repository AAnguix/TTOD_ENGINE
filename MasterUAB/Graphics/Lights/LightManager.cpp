#include "Lights\LightManager.h"
#include "Lights\SpotLight.h"
#include "Lights\Light.h"
#include "Lights\OmniLight.h"
#include "Utils\Utils.h"

CLightManager::CLightManager() : m_AmbientLight(0.0f,0.0f,0.0f,1.0f)
{
}

CLightManager::~CLightManager()
{
}

void CLightManager::Load(const std::string &FileName)
{
	m_FileName=FileName;
	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode l_Lights= l_XML["lights"];

		if (l_Lights.Exists())
		{
			m_AmbientLight = l_Lights.GetVect4fProperty("ambient_light",Vect4f(0.3f,0.3f,0.3f,1.0f));
			m_FogParameters = Vect4f(l_Lights.GetFloatProperty("fog_start",10.0f),l_Lights.GetFloatProperty("fog_end",20.0f),l_Lights.GetFloatProperty("fog_density",0.1f),0.0f);
			m_FogColor = l_Lights.GetVect4fProperty("fog_color",Vect4f(0.1f,0.1f,0.1f,0.0f));

			for (int i = 0; i < l_Lights.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Lights(i);

				if (l_Element.GetName() == std::string("light"))
				{
					CLight *l_Light;
					std::string l_Type=l_Element.GetPszProperty("type","");

					if(CLight::GetLightTypeByName(l_Type) == CLight::OMNI)
					{
						l_Light = new COmniLight(l_Element);
						
					}
					else if(CLight::GetLightTypeByName(l_Type) == CLight::DIRECTIONAL) 
					{
						l_Light = new CDirectionalLight(l_Element);
						
					}
					else if(CLight::GetLightTypeByName(l_Type) == CLight::SPOT)
					{
						l_Light = new CSpotLight(l_Element);	
					}

					if(!AddResource(l_Light->GetName(), l_Light))
					{
						CHECKED_DELETE(l_Light);
					}
				}
			}
		}
	}
}

void CLightManager::Reload()
{
	Destroy();
	Load(m_FileName);
}

void CLightManager::Render(CRenderManager *RenderManager)
{
	for (size_t i=0;i<m_ResourcesVector.size();++i)
	{
		m_ResourcesVector[i]->Render(RenderManager);
	}
}