#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "Utils\Named.h"
#include "Utils\Utils.h"
#include <vector>

class CRenderableObjectTechnique;
class CTexture;
class CXMLTreeNode;
class CMaterialParameter;

class CMaterial : public CNamed { 

private:  
	std::vector<CTexture *>   m_Textures; 
	std::vector<CMaterialParameter*> m_Parameters;
	CRenderableObjectTechnique* m_RenderableObjectTechnique;
	unsigned int m_CurrentParameterData;
	unsigned int m_Offset;

	void Destroy(); 

public:  
	CMaterial(CXMLTreeNode &TreeNode);  
	virtual ~CMaterial();  
	virtual void Apply(CRenderableObjectTechnique* RenderableObjectTechnique=NULL);  
	UAB_GET_PROPERTY_POINTER(CRenderableObjectTechnique, RenderableObjectTechnique);
	void * GetNextParameterAddress(unsigned int NumBytes);
	UAB_GET_PROPERTY_REFERENCE(std::vector<CMaterialParameter *>, Parameters);
};

#endif
