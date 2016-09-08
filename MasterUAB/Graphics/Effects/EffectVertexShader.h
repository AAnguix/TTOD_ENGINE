#ifndef _EFFECTVERTEXSHADER_H
#define _EFFECTVERTEXSHADER_H

#include "Effects\EffectShader.h"

class CEffectVertexShader : public CEffectShader
{

protected:

	ID3D11VertexShader* m_VertexShader;
	ID3D11InputLayout* m_VertexLayout;
	std::string m_VertexType;

	void Destroy();
	
public:

	CEffectVertexShader(const CXMLTreeNode &TreeNode);
	virtual ~CEffectVertexShader();
	bool Load(size_t EffectsStateCode);
	void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer);
	
	ID3D11VertexShader* GetVertexShader() const { return m_VertexShader; }
	ID3D11InputLayout* GetVertexLayout() const { return m_VertexLayout; }
};

#endif