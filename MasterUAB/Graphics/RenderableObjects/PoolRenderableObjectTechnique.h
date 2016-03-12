#ifndef _POOLRENDERABLEOBJECTTECHNIQUE_H
#define _POOLRENDERABLEOBJECTTECHNIQUE_H

#include "Utils\Named.h"
#include "RenderableObjects\RenderableObjectTechnique.h"
#include <vector>
class CXMLTreeNode;
class CEffectTechnique;

class CPoolRenderableObjectTechnique : public CNamed 
{ 

private: 
	class CPoolRenderableObjectTechniqueElement  
	{  
		public:   
			CRenderableObjectTechnique m_RenderableObjectTechnique;  
			CRenderableObjectTechnique* m_OnRenderableObjectTechniqueManager;  

			CPoolRenderableObjectTechniqueElement(const std::string &Name, CEffectTechnique *EffectTechnique, CRenderableObjectTechnique *OnRenderableObjectTechniqueManager)
				: m_OnRenderableObjectTechniqueManager(OnRenderableObjectTechniqueManager)
				, m_RenderableObjectTechnique(Name, EffectTechnique)
			{
			}
	};  

	std::vector<CPoolRenderableObjectTechniqueElement *>    m_RenderableObjectTechniqueElements; 

public:  
	CPoolRenderableObjectTechnique(CXMLTreeNode &TreeNode);  
	virtual ~CPoolRenderableObjectTechnique();  
	void Destroy();  
	void AddElement(const std::string &Name, const std::string &TechniqueName, CRenderableObjectTechnique *ROTOnRenderableObjectTechniqueManager);  
	void Apply(); 
};

#endif