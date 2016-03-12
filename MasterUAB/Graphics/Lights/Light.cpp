#include "Lights\Light.h"
#include "Render\RenderManager.h"
#include "Engine.h"
#include "Textures\TextureManager.h"
#include "Textures\DynamicTexture.h"
#include "RenderableObjects\LayerManager.h"

CLight::TLightType CLight::GetLightTypeByName(const std::string &StrLightType)
{
	if(StrLightType=="OMNI" || StrLightType=="Omni" || StrLightType=="omni")
	{
		return OMNI;
	}
	else if(StrLightType=="SPOT" || StrLightType=="Spot" || StrLightType=="spot")
	{
		return SPOT;
	}
	else if(StrLightType=="DIRECTIONAL" || StrLightType=="Directional" || StrLightType=="directional")
	{
		return DIRECTIONAL;	
	}

	return OMNI;
}

CLight::CLight(CXMLTreeNode &TreeNode) : CNamed(TreeNode)
	,m_ShadowMap(0)
	,m_ShadowMaskTexture(0)
	,m_ViewShadowMap(m44fZERO)
	,m_ProjectionShadowMap(m44fZERO)
{	
	SetActive(TreeNode.GetBoolProperty("active",true));
	SetPosition(TreeNode.GetVect3fProperty("pos",v3fZERO));
	Vect4f l_V4=TreeNode.GetVect4fProperty("color",v4fZERO);
	SetColor(CColor(l_V4));
	SetStartRangeAttenuation(TreeNode.GetFloatProperty("att_start_range",0.0f));
	SetEndRangeAttenuation(TreeNode.GetFloatProperty("att_end_range",0.0f));
	SetIntensity(TreeNode.GetFloatProperty("intensity",0.0f));
	bool l_G=TreeNode.GetBoolProperty("generate_shadow_map",false);
	SetGenerateShadowMap(l_G);
	
	if (l_G)
	{
		std::string l_ShadowTextureMask = TreeNode.GetPszProperty("shadow_texture_mask","");
		if(l_ShadowTextureMask!="")
			m_ShadowMaskTexture = CEngine::GetSingleton().GetTextureManager()->GetTexture(l_ShadowTextureMask);
		
		int l_ShadowMapWidth = TreeNode.GetIntProperty("shadow_map_width",0);
		int l_ShadowMapHeight = TreeNode.GetIntProperty("shadow_map_height",0);		

		m_ShadowMap = new CDynamicTexture(GetName()+"_ShadowMap",l_ShadowMapWidth,l_ShadowMapHeight,true,CDynamicTexture::R32_FLOAT);

		for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
		{
			CXMLTreeNode l_Element = TreeNode(i);

			if (l_Element.GetName() == std::string("layer"))
			{
				std::string l_LayerName = l_Element.GetPszProperty("layer");
				CRenderableObjectsManager* l_Layer=CEngine::GetSingleton().GetLayerManager()->GetResource(l_LayerName);
				if(l_Layer!=NULL)
					m_Layers.push_back(l_Layer);
			}
		}
	}

	//memcpy(address,(void*)&m_Intensity,sizeof(float));
}

CLight::CLight() : CNamed("")
	,m_GenerateShadowMap(false)
	,m_ShadowMap(0)
	,m_ShadowMaskTexture(0)
	,m_Color(NULL)
	,m_Position(v3fZERO)
	,m_Intensity(0)
	,m_StartRangeAttenuation(0)
	,m_EndRangeAttenuation(0)
	,m_Active(true)
{	
}

CLight::~CLight()
{
	CHECKED_DELETE(m_ShadowMap);
	m_Layers.clear();
}

void CLight::Render(CRenderManager *RM)
{
	
}

/*
CEmptyPointerClass * CLight::GetIntensityAddress() const
{
	return (CEmptyPointerClass *)&m_Intensity;
}*/
