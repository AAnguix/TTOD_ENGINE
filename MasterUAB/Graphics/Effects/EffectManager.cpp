#include "EffectManager.h"
#include "Lights\Light.h"
#include "Engine\Engine.h"
#include "Lights\LightManager.h"
#include "Lights\SpotLight.h"
#include "Lights\OmniLight.h"
#include "Lights\Light.h"
#include "Textures\DynamicTexture.h"
#include "XML\XMLTreeNode.h"
#include "Utils\FileUtils.h"

CSceneEffectParameters CEffectManager::m_SceneEffectParameters;
CLightEffectParameters CEffectManager::m_LightEffectParameters;
CAnimatedModelEffectParameters CEffectManager::m_AnimatedModelEffectParameters;
CMaterialEffectParameters CEffectManager::m_MaterialEffectParameters;

#define UAB_ID_SHADOW_MAP 6

CEffectManager::CEffectManager()
:m_EffectsStateCode(0)
{

}

CEffectManager::~CEffectManager()
{
	Destroy();
}

/*void CEffectManager::Destroy()
{
	m_VertexShaders.Destroy();
	m_PixelShaders.Destroy();
	CTemplatedMapManager::Destroy();
	//Check memory leaks
}*/

void CEffectManager::ReloadFile()
{
	m_VertexShaders.Destroy();
	m_PixelShaders.Destroy();
	m_GeometryShaders.Destroy();
	Destroy();
	Load(m_Filename);
}

void CEffectManager::Reload()
{
	//¿?
}

void CEffectManager::Load(const std::string &Filename)
{
	m_Filename=Filename;
	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(Filename.c_str()))
	{
		CXMLTreeNode l_Effects = l_XML["effects"];

		if (l_Effects.Exists())
		{
			m_EffectsStateCode = CFileUtils::GetEffectsFileStateCode();

			for (int i = 0; i < l_Effects.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Effects(i);

				if (l_Element.GetName() == std::string("vertex_shader"))
				{
					std::string l_Name = l_Element.GetPszProperty("name","");

					CEffectVertexShader *l_VertexShader = new CEffectVertexShader(l_Element);
					l_VertexShader->Load();
					if(!m_VertexShaders.AddResource(l_Name,l_VertexShader))
					{
						CHECKED_DELETE(l_VertexShader);
					}
				}
				else if (l_Element.GetName() == std::string("pixel_shader"))
				{
					std::string l_Name = l_Element.GetPszProperty("name","");
					
					CEffectPixelShader *l_PixelShader = new CEffectPixelShader(l_Element);
					l_PixelShader->Load();
					if(!m_PixelShaders.AddResource(l_Name,l_PixelShader))
					{
						CHECKED_DELETE(l_PixelShader);
					}
				}
				else if (l_Element.GetName() == std::string("geometry_shader"))
				{
					std::string l_Name = l_Element.GetPszProperty("name", "");

					CEffectGeometryShader *l_GeometryShader = new CEffectGeometryShader(l_Element);
					l_GeometryShader->Load();
					if (!m_GeometryShaders.AddResource(l_Name, l_GeometryShader))
					{
						CHECKED_DELETE(l_GeometryShader);
					}
				}
				else if (l_Element.GetName() == std::string("effect_technique"))
				{
					std::string l_Name = l_Element.GetPszProperty("name","");

					CEffectTechnique *l_EffectTechnique = new CEffectTechnique(l_Element);
					if(!AddResource(l_Name,l_EffectTechnique))
					{
						CHECKED_DELETE(l_EffectTechnique);
					}
				}
			}
		}
		else { assert(false); }
	}
	else { assert(false); }
}

CEffectVertexShader * CEffectManager::GetVertexShader(const std::string &VertexShader)
{
	return m_VertexShaders.GetResource(VertexShader);
}

CEffectPixelShader * CEffectManager::GetPixelShader(const std::string &PixelShader)
{
	return m_PixelShaders.GetResource(PixelShader);
}

CEffectGeometryShader * CEffectManager::GetGeometryShader(const std::string &GeometryShader)
{
	return m_GeometryShaders.GetResource(GeometryShader);
}

void CEffectManager::SetSceneConstants()
{
	/*Se utilizará para cambiar las constant buffers, ya que la matriz de world del objeto esta puesto 
	en el constant buffer de la escena y esto lo deberemos cambiar para poder poner 
	la view y projection una vez y la world para cada objeto.
	*/
}

void CEffectManager::SetLightsConstantsDefaultValues(unsigned int MaxLights)
{
	for (size_t i = 0; i < MaxLights; ++i)
	{
		m_LightEffectParameters.m_LightEnabled[i] = 0.0f;
		m_LightEffectParameters.m_LightType[i] = 0.0f;
		m_LightEffectParameters.m_LightPosition[i] = v4fZERO;

		m_LightEffectParameters.m_LightDirection[i] = v4fZERO;
		m_LightEffectParameters.m_LightAngle[i] = 0.0f;
		m_LightEffectParameters.m_LightFallOffAngle[i] = 0.0f;

		m_LightEffectParameters.m_LightAttenuationStartRange[i] = 0.0f;
		m_LightEffectParameters.m_LightAttenuationEndRange[i] = 0.0f;

		m_LightEffectParameters.m_LightIntensity[i] = 0.0f;
		m_LightEffectParameters.m_LightColor[i] = v4fZERO;

		m_LightEffectParameters.m_UseShadowMap[i] = false;
		m_LightEffectParameters.m_UseShadowMask[i] = false;
		m_LightEffectParameters.m_LightView[i] = m44fZERO; //m44fIDENTITY
		m_LightEffectParameters.m_LightProjection[i] = m44fZERO;
	}
}

void CEffectManager::SetLightConstants(unsigned int IdLight, CLight *Light)
{
	assert(Light != nullptr);

	if(Light!=nullptr)
	{	
		m_LightEffectParameters.m_LightEnabled[IdLight]=1.0f;
		if(!Light->GetActive())
			m_LightEffectParameters.m_LightEnabled[IdLight]=0.0f;

		m_LightEffectParameters.m_LightType[IdLight]=(float)Light->GetType();
		m_LightEffectParameters.m_LightPosition[IdLight]=Light->GetPosition();

		m_LightEffectParameters.m_LightAttenuationStartRange[IdLight]=Light->GetStartRangeAttenuation();
		m_LightEffectParameters.m_LightAttenuationEndRange[IdLight]= Light->GetEndRangeAttenuation();
		m_LightEffectParameters.m_LightIntensity[IdLight]=Light->GetIntensity();
		m_LightEffectParameters.m_LightColor[IdLight]=Light->GetColor();
		m_LightEffectParameters.m_UseShadowMap[IdLight]=Light->GetGenerateShadowMap();

		if(Light->GetType() == CLight::DIRECTIONAL)
		{
			m_LightEffectParameters.m_LightDirection[IdLight]=((CDirectionalLight*)Light)->GetDirection();
		}
		else if(Light->GetType() == CLight::SPOT)
		{	
			m_LightEffectParameters.m_LightDirection[IdLight]= ((CSpotLight*)Light)->GetDirection();
			m_LightEffectParameters.m_LightAngle[IdLight]= ((CSpotLight*)Light)->GetAngle();
			m_LightEffectParameters.m_LightFallOffAngle[IdLight]= ((CSpotLight*)Light)->GetFallOff();
		}

		if(Light->GetGenerateShadowMap())
		{
			CDynamicTexture *l_ShadowMap=Light->GetShadowMap();
			CTexture *l_ShadowMask=Light->GetShadowMaskTexture();
			
			CEffectManager::m_LightEffectParameters.m_UseShadowMask[IdLight]=l_ShadowMask!=NULL ? 1.0f : 0.0f;
			CEffectManager::m_LightEffectParameters.m_LightView[IdLight]=Light->GetViewShadowMap();
			CEffectManager::m_LightEffectParameters.m_LightProjection[IdLight]=Light->GetProjectionShadowMap();
			l_ShadowMap->Activate(UAB_ID_SHADOW_MAP+IdLight*2);
			
			if(l_ShadowMask!=NULL)
				l_ShadowMask->Activate(UAB_ID_SHADOW_MAP+1+IdLight*2);
		}
	}
}

void CEffectManager::SetLightsConstants(unsigned int MaxLights)
{
	SetLightsConstantsDefaultValues(MaxLights);
	CLightManager *l_LightManager = CEngine::GetSingleton().GetLightManager();
	m_LightEffectParameters.m_LightAmbient= l_LightManager->GetAmbientLight();
	m_LightEffectParameters.m_FogColor = l_LightManager->GetFogColor();
	m_LightEffectParameters.m_FogParameters = l_LightManager->GetFogParameters();

	size_t l_Size=l_LightManager->GetResourcesVector().size();
	
	for(size_t i=0;i<MaxLights;++i)
	{
		if(i<l_Size)
		{
			CLight *l_Light = l_LightManager->GetResourceById(i);
			SetLightConstants(i, l_Light);
		}
	}
	
	std::map<std::string, CEffectTechnique*>::iterator itMap;
	for ( itMap = m_Resources.begin(); itMap != m_Resources.end(); ++itMap)
	{
		//CSceneEffectParameters,0. CLightEffectParameters,1. CAnimatedModelEffectParameters,2.
		CEffectPixelShader* l_PShader = itMap->second->GetPixelShader();
		if (l_PShader) l_PShader->SetConstantBuffer(1, &m_LightEffectParameters);

		CEffectVertexShader* l_VShader = itMap->second->GetVertexShader();
		if (l_VShader)  itMap->second->GetVertexShader()->SetConstantBuffer(1, &m_LightEffectParameters);

		//CEffectGeometryShader* l_GShader = itMap->second->GetGeometryShader();
		/*if (l_GShader!=nullptr)
			l_GShader->SetConstantBuffer(1, &m_LightEffectParameters);*/
	}
}

const std::vector<CEffectTechnique *> & CEffectManager::GetLUAEffects()
{
	l_EffectTechniqueVector.clear();
	CTemplatedMapManager<CEffectTechnique>::TMapResource &l_EffectsMap=GetResourcesMap();
	for(CTemplatedMapManager<CEffectTechnique>::TMapResource::iterator it=l_EffectsMap.begin();it!=l_EffectsMap.end();++it)
		l_EffectTechniqueVector.push_back(it->second);

	return l_EffectTechniqueVector;
}