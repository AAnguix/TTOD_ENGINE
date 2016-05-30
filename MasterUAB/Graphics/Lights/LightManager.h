#ifndef _LIGHTHMANAGER_H
#define _LIGHTHMANAGER_H

#include "Lights\Light.h"
#include "Utils\TemplatedVectorMapManager.h"

class CLightManager : public CTemplatedVectorMapManager<CLight> 
{  

protected:  
	std::string m_FileName;
	Vect4f m_AmbientLight;
	Vect4f m_FogParameters;
	Vect4f m_FogColor;

public:   
	CLightManager();   
	~CLightManager();   
	void Load(const std::string &FileName);   
	void Render(CRenderManager *RenderManager); 
	void Reload(); 
	Vect4f GetAmbientLight() const {return m_AmbientLight;};
	Vect4f GetFogParameters() const {return m_FogParameters;};
	Vect4f GetFogColor() const {return m_FogColor;};

	CEmptyPointerClass* GetFogStartAddress() const {return (CEmptyPointerClass *)&m_FogParameters.x;}
	CEmptyPointerClass* GetFogEndAddress() const {return (CEmptyPointerClass *)&m_FogParameters.y;}
	CEmptyPointerClass* GetFogDensityAddress() const {return (CEmptyPointerClass *)&m_FogParameters.z;}
	//UAB_GET_VAR_ADDRESS(FogParameters);
	UAB_GET_VAR_ADDRESS(FogColor);
}; 

#endif