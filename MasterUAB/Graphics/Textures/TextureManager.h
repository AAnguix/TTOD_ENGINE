#ifndef  _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "Textures\Texture.h"

class CTextureManager : public CTemplatedMapManager<CTexture>
{
public:
	CTextureManager();
	virtual ~CTextureManager();
	CTexture * GetTexture(const std::string &Filename);  
	void Reload();
};

#endif

