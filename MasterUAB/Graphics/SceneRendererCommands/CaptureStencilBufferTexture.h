#ifndef _CAPTURESTENCILBUFFERTEXTURE_H
#define _CAPTURESTENCILBUFFERTEXTURE_H

#include "Textures\Texture.h"

class CCaptureStencilBufferTexture : public CTexture
{

protected:
	ID3D11Texture2D* m_DataTexture;

	void Init(const std::string &Name, unsigned int Width, unsigned int Height);
	void Unload();
	bool CreateSamplerState();

public:
	bool Reload();
	CCaptureStencilBufferTexture(const CXMLTreeNode &TreeNode);
	CCaptureStencilBufferTexture(const std::string &Name, unsigned int Width, unsigned int Height);
	virtual ~CCaptureStencilBufferTexture();
	bool Capture(unsigned int StageId);
	bool SaveToFile(std::string& Path);
};

#endif
	




