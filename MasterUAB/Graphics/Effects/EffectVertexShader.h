#ifndef _EFFECTVERTEXSHADER_H
#define _EFFECTVERTEXSHADER_H

#include "Effects\EffectShader.h"

class CEffectVertexShader : public CEffectShader
{

protected:

	ID3D11VertexShader *m_VertexShader;
	ID3D11InputLayout *m_VertexLayout;
	std::string m_VertexType;

	void Destroy();
	
public:

	CEffectVertexShader(const CXMLTreeNode &TreeNode);
	virtual ~CEffectVertexShader();
	bool Load(size_t EffectsStateCode);
	void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer);
	
	UAB_GET_PROPERTY_POINTER(ID3D11VertexShader, VertexShader); 
	UAB_GET_PROPERTY_POINTER(ID3D11InputLayout, VertexLayout);
};

#endif