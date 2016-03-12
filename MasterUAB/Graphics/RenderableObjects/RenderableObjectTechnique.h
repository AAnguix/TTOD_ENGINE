#ifndef _RENDERABLEOBJECTTECHNIQUE_H
#define _RENDERABLEOBJECTTECHNIQUE_H

#include "Utils\Named.h"
#include "Effects\EffectTechnique.h"

class CRenderableObjectTechnique : public CNamed 
{ 

private:  
	CEffectTechnique* m_EffectTechnique; 

public:  
	CRenderableObjectTechnique(const std::string &Name, CEffectTechnique *EffectTechnique);  
	void SetEffectTechnique(CEffectTechnique *EffectTechnique); 
	CEffectTechnique * GetEffectTechnique() const; 
};

#endif