#ifndef _EFFECTMANAGER_H
#define _EFFECTMANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "Effects\EffectTechnique.h"
#include "Effects\EffectParameters.h"
class CLight;

class CEffectManager : public CTemplatedMapManager<CEffectTechnique> 
{ 

private: 
	std::vector<CEffectTechnique *> l_EffectTechniqueVector; /*To list effects in LUA*/

	CTemplatedMapManager<CEffectVertexShader> m_VertexShaders;  
	CTemplatedMapManager<CEffectPixelShader> m_PixelShaders;
	CTemplatedMapManager<CEffectGeometryShader> m_GeometryShaders;
	std::string m_Filename; 

	//void Destroy();

public:  
	static CSceneEffectParameters m_SceneEffectParameters;  //Parámetros necesarios para renderizar una geometria
	static CAnimatedModelEffectParameters m_AnimatedModelEffectParameters;
	static CLightEffectParameters m_LightEffectParameters;
	static CMaterialEffectParameters m_MaterialEffectParameters;

	CEffectManager();						//matrices de vista, projection, mundo
	virtual ~CEffectManager();				//valores duplicados en la clase, y en la tarjeta video
											//una effecttechnique esta centrada en un pixel y un vertex shader
	void ReloadFile();
	void Reload();							
	void Load(const std::string &Filename);  
	CEffectVertexShader * GetVertexShader(const std::string &VertexShader);  
	CEffectPixelShader * GetPixelShader(const std::string &PixelShader);
	CEffectGeometryShader * GetGeometryShader(const std::string &GeometryShader);
	void SetSceneConstants();  
	void SetLightConstants(unsigned int IdLight, CLight *Light);  
	void SetLightsConstants(unsigned int MaxLights=MAX_LIGHTS_BY_SHADER);

	const std::vector<CEffectTechnique *> & GetLUAEffects();
}; 

#endif