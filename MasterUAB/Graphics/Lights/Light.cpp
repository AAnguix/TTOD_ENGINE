#include "Lights\Light.h"
#include "Engine.h"
#include "Textures\TextureManager.h"
#include "Textures\DynamicTexture.h"
#include "RenderableObjects\LayerManager.h"
#include "XML\XMLTreeNode.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "Log\Log.h"

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

CLight::CLight(CXMLTreeNode &TreeNode) 
:CNamed(TreeNode)
,C3DElement(TreeNode)
,m_ShadowMap(nullptr)
,m_ShadowMaskTexture(nullptr)
,m_ViewShadowMap(m44fZERO)
,m_ProjectionShadowMap(m44fZERO)
,m_Active(TreeNode.GetBoolProperty("active", true))
,m_Color(CColor(TreeNode.GetVect4fProperty("color", v4fZERO)))
,m_StartRangeAttenuation(TreeNode.GetFloatProperty("att_start_range", 0.0f))
,m_EndRangeAttenuation(TreeNode.GetFloatProperty("att_end_range", 0.0f))
,m_Intensity(TreeNode.GetFloatProperty("intensity", 0.0f))
,m_GenerateShadowMap(TreeNode.GetBoolProperty("generate_shadow_map", false))
{	
	#ifdef _DEBUG
	if (m_StartRangeAttenuation > m_EndRangeAttenuation)
	{ 
		CEngine::GetSingleton().GetLogManager()->Log("Error. Start attenuation is lower than end attenuation on light "+m_Name);
		assert(false);
	}
	#endif

	//SetPosition(TreeNode.GetVect3fProperty("pos",v3fZERO));
	if (m_GenerateShadowMap)
	{
		std::string l_ShadowTextureMask = TreeNode.GetPszProperty("shadow_texture_mask","");
		if(l_ShadowTextureMask!="")
			m_ShadowMaskTexture = CEngine::GetSingleton().GetTextureManager()->GetTexture(l_ShadowTextureMask,false);
		
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

CLight::CLight() 
:CNamed("")
,m_GenerateShadowMap(false)
,m_ShadowMap(nullptr)
,m_ShadowMaskTexture(nullptr)
,m_Color(v4fZERO)
,m_Intensity(0.0f)
,m_StartRangeAttenuation(0.0f)
,m_EndRangeAttenuation(0.0f)
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

CEmptyPointerClass* CLight::GetIntensityLuaAddress() const  { return (CEmptyPointerClass *)((void*)&m_Intensity); }
CEmptyPointerClass* CLight::GetColorLuaAddress() const  { return (CEmptyPointerClass *)((void*)&m_Color); }
CEmptyPointerClass* CLight::GetActiveLuaAddress() const  { return (CEmptyPointerClass *)((void*)&m_Active); }
CEmptyPointerClass* CLight::GetStartRangeAttenuationLuaAddress() const  { return (CEmptyPointerClass *)((void*)&m_StartRangeAttenuation); }
CEmptyPointerClass* CLight::GetEndRangeAttenuationLuaAddress() const  { return (CEmptyPointerClass *)((void*)&m_EndRangeAttenuation); }