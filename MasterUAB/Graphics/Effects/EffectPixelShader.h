#ifndef _EFFECTPIXELSHADER_H
#define _EFFECTPIXELSHADER_H

#include "Effects\EffectShader.h"

class CEffectPixelShader : public CEffectShader
{

protected:

	ID3D11PixelShader *m_PixelShader;

	void Destroy();
	
public:

	CEffectPixelShader(const CXMLTreeNode &TreeNode);
	virtual ~CEffectPixelShader();
	bool Load(size_t EffectsStateCode);
	void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer);

	UAB_GET_PROPERTY_POINTER(ID3D11PixelShader, PixelShader);  
};

#endif