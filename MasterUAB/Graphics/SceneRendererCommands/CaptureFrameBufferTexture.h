#ifndef _CAPTUREFRAMEBUFFERTEXTURE_H
#define _CAPTUREFRAMEBUFFERTEXTURE_H

#include "Textures\Texture.h"

class CCaptureFrameBufferTexture : public CTexture
{

protected:
	ID3D11Texture2D* m_DataTexture;

	void Init(const std::string &Name, unsigned int Width, unsigned int Height);
	void Unload();
	bool CreateSamplerState();

public:
	bool Reload();
	CCaptureFrameBufferTexture(const CXMLTreeNode &TreeNode);
	CCaptureFrameBufferTexture(const std::string &Name, unsigned int Width,unsigned int Height);
	virtual ~CCaptureFrameBufferTexture();
	bool Capture(unsigned int StageId);
};

#endif
	




