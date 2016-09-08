#ifndef _EFFECTTECHNIQUE_H
#define _EFFECTTECHNIQUE_H

#include "Utils\Named.h"

#include "Effects\EffectVertexShader.h"
#include "Effects\EffectPixelShader.h"
#include "Effects\EffectGeometryShader.h"

class CEffectTechnique : public CNamed 
{ 

private:  
	CEffectVertexShader* m_VertexShader;    
	CEffectPixelShader* m_PixelShader; 
	CEffectGeometryShader* m_GeometryShader;
	std::string	m_VertexShaderName;  
	std::string	m_PixelShaderName; 
	std::string m_GeometryShaderName;

public:  
	CEffectTechnique(CXMLTreeNode &TreeNode);  
	virtual ~CEffectTechnique();  

	CEffectVertexShader* GetVertexShader() const { return m_VertexShader; }
	CEffectPixelShader* GetPixelShader() const { return m_PixelShader; }
	CEffectGeometryShader* GetGeometryShader() const { return m_GeometryShader; }
	void Refresh();

	/*Use as many as posible*/
	void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer);
};

#endif