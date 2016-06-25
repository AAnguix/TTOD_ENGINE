#ifndef _LIGHTHMANAGER_H
#define _LIGHTHMANAGER_H

#include "Lights\Light.h"
#include "Utils\TemplatedVectorMapManager.h"

class CLightManager : public CTemplatedVectorMapManager<CLight> 
{  
private:
	std::string GetDefaultLightName(CLight::TLightType LightType);

protected:  
	std::string m_FileName;
	Vect4f m_AmbientLight;
	Vect4f m_FogParameters;
	Vect4f m_FogColor;

public:   
	CLightManager();   
	~CLightManager();   
	bool Load(const std::string &FileName);
	bool Reload();
	void Render(CRenderManager *RenderManager); 
	
	void AddLight(CLight::TLightType LightType);
	

	Vect4f GetAmbientLight() const {return m_AmbientLight;};
	Vect4f GetFogParameters() const {return m_FogParameters;};
	Vect4f GetFogColor() const {return m_FogColor;};

	CEmptyPointerClass* GetFogStartAddress() const {return (CEmptyPointerClass *)&m_FogParameters.x;}
	CEmptyPointerClass* GetFogEndAddress() const {return (CEmptyPointerClass *)&m_FogParameters.y;}
	CEmptyPointerClass* GetFogDensityAddress() const {return (CEmptyPointerClass *)&m_FogParameters.z;}
	
	CEmptyPointerClass* GetAmbientLightAddress(int Index){ return (CEmptyPointerClass *)&m_AmbientLight[Index]; }
	CEmptyPointerClass* GetFogColorAddress(int Index){ return (CEmptyPointerClass *)&m_FogColor[Index]; }
}; 

#endif