#ifndef _EFFECTTECHNIQUE_H
#define _EFFECTTECHNIQUE_H

#include "Effects\EffectVertexShader.h"
#include "Effects\EffectPixelShader.h"
#include "Effects\EffectGeometryShader.h"
#include "Utils\Utils.h"

class CEffectTechnique : public CNamed 
{ 

private:  

	CEffectVertexShader	*m_VertexShader;    
	CEffectPixelShader	*m_PixelShader; 
	CEffectGeometryShader *m_GeometryShader;
	std::string	m_VertexShaderName;  
	std::string	m_PixelShaderName; 
	std::string m_GeometryShaderName;

public:  
	CEffectTechnique(CXMLTreeNode &TreeNode);  
	virtual ~CEffectTechnique();  

	UAB_GET_PROPERTY_POINTER(CEffectVertexShader, VertexShader);  
	UAB_GET_PROPERTY_POINTER(CEffectPixelShader, PixelShader);
	UAB_GET_PROPERTY_POINTER(CEffectGeometryShader, GeometryShader);
	void Refresh();

	/*No utilizarlo en la medida de lo posible*/
	void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer);

};

#endif