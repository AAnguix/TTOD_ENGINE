#ifndef _LIGHT_H
#define _LIGHT_H

#include "Utils\3DElement.h"
#include "Utils\Named.h"
#include "Math\Color.h"
#include "Utils\Utils.h"
#include <vector>

class CRenderableObjectsManager;
class CRenderManager;
class CDynamicTexture;
class CTexture;

class CLight : public C3DElement, public CNamed 
{  
public:   
	enum TLightType    
	{    
		OMNI=0,    
		DIRECTIONAL,
		SPOT   
	};  

protected:   
	bool m_GenerateShadowMap;
	CDynamicTexture* m_ShadowMap;
	CDynamicTexture* m_BlackAndWhiteMap;
	CDynamicTexture* m_BlackAndWhiteBlurredMap;

	CTexture* m_ShadowMaskTexture;
	std::vector<CRenderableObjectsManager *> m_Layers;
	Mat44f m_ViewShadowMap, m_ProjectionShadowMap;
	
	CColor m_Color;
	TLightType m_Type; 
	
	bool m_Active;
	float m_Intensity;
	float m_StartRangeAttenuation;
	float m_EndRangeAttenuation;

public:   
	
	CLight(CXMLTreeNode &TreeNode);   
	CLight(const std::string &Name);   
	virtual ~CLight();  
	
	UAB_GET_SET_PROPERTY_REFERENCE(CColor, Color);  
	UAB_GET_SET_STANDARD_PROPERTY(float, StartRangeAttenuation);  
	UAB_GET_SET_STANDARD_PROPERTY(float, EndRangeAttenuation);
	UAB_GET_SET_STANDARD_PROPERTY(float, Intensity);  
	UAB_GET_SET_STANDARD_PROPERTY(TLightType, Type);  

	UAB_GET_PROPERTY_POINTER(CDynamicTexture, BlackAndWhiteBlurredMap);
	UAB_GET_PROPERTY_POINTER(CDynamicTexture, BlackAndWhiteMap);
	UAB_GET_PROPERTY_POINTER(CDynamicTexture, ShadowMap);
	UAB_GET_PROPERTY_POINTER(CTexture, ShadowMaskTexture);
	UAB_GET_PROPERTY_REFERENCE(Mat44f, ViewShadowMap);
	UAB_GET_PROPERTY_REFERENCE(Mat44f, ProjectionShadowMap);
	UAB_GET_PROPERTY_REFERENCE(std::vector<CRenderableObjectsManager *>, Layers);

	virtual void Render(CRenderManager *RenderManager); 
	static TLightType GetLightTypeByName(const std::string &StrLightType);

	bool GetGenerateShadowMap(){return m_GenerateShadowMap;};
	void SetGenerateShadowMap(bool Value){m_GenerateShadowMap=Value;};

	bool GetActive() const{return m_Active;};
	void SetActive(bool Active){m_Active=Active;};
	void SwichActive(){m_Active=!m_Active;};

	CEmptyPointerClass* GetIntensityLuaAddress() const;
	CEmptyPointerClass* GetColorLuaAddress() const;
	CEmptyPointerClass* GetActiveLuaAddress() const;
	CEmptyPointerClass* GetStartRangeAttenuationLuaAddress() const;
	CEmptyPointerClass* GetEndRangeAttenuationLuaAddress() const;

	virtual void SetShadowMap(CRenderManager &RenderManager) = 0;
	virtual void SetBlackAndWhiteMap(CRenderManager &RenderManager, bool Blurred) = 0;
}; 

#endif
