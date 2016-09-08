#ifndef _EFFECTGEOMETRYSHADER_H
#define _EFFECTGEOMETRYSHADER_H

#include "Effects\EffectShader.h"

class CEffectGeometryShader : public CEffectShader
{

protected:

	ID3D11GeometryShader *m_GeometryShader;

	void Destroy();

public:

	CEffectGeometryShader(const CXMLTreeNode &TreeNode);
	virtual ~CEffectGeometryShader();
	bool Load(size_t EffectsStateCode);
	void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer);

	ID3D11GeometryShader* GetGeometryShader() const { return m_GeometryShader; }
};

#endif