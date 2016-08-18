#ifndef _EFFECTMANAGER_H
#define _EFFECTMANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "Effects\EffectTechnique.h"
#include "Effects\EffectParameters.h"
class CLight;

class CEffectManager : public CTemplatedMapManager<CEffectTechnique> 
{ 

private: 
	std::vector<CEffectTechnique *> l_EffectTechniqueVector;

	CTemplatedMapManager<CEffectVertexShader> m_VertexShaders;  
	CTemplatedMapManager<CEffectPixelShader> m_PixelShaders;
	CTemplatedMapManager<CEffectGeometryShader> m_GeometryShaders;
	std::string m_Filename; 
	//void Destroy();
	void UpdateLightBuffers();

	size_t m_EffectsStateCode; 

public:  
	static CSceneEffectParameters m_SceneEffectParameters;  
	static CAnimatedModelEffectParameters m_AnimatedModelEffectParameters;
	static CLightEffectParameters m_LightEffectParameters;
	static CMaterialEffectParameters m_MaterialEffectParameters;

	CEffectManager();						
	virtual ~CEffectManager();	

	size_t GetEffectsStateCode() const { return m_EffectsStateCode; }
											
	void Reload();							
	void Load(const std::string &Filename);  
	CEffectVertexShader * GetVertexShader(const std::string &VertexShader);  
	CEffectPixelShader * GetPixelShader(const std::string &PixelShader);
	CEffectGeometryShader * GetGeometryShader(const std::string &GeometryShader);
	void SetSceneConstants();  

	void SetLightsConstantsDefaultValues(unsigned int MaxLights = MAX_LIGHTS_BY_SHADER);
	void SetLightConstants(unsigned int IdLight, CLight *Light);  
	void SetLightsConstants(unsigned int MaxLights=MAX_LIGHTS_BY_SHADER);
	void SetBlackAndWhiteLightConstants(CLight *Light);

	const std::vector<CEffectTechnique *> & GetLUAEffects();
}; 

#endif