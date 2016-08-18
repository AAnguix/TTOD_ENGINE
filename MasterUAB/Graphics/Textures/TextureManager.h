#ifndef  _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "Textures\Texture.h"

class CTextureManager : public CTemplatedMapManager<CTexture>
{
private:
	bool SaveToFile(std::string& Path, ID3D11Resource* Texture);

public:
	CTextureManager();
	virtual ~CTextureManager();
	CTexture * GetTexture(const std::string &Filename, bool GuiTexture);
	void Reload();

	void SaveBlackAndWhiteTexturesToFile();
};

#endif

