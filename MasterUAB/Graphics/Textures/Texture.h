#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <string>
#include "Utils\Named.h"
#include <d3d11.h>

class CTexture : public CNamed
{

protected:
	ID3D11ShaderResourceView* m_Texture;
	ID3D11SamplerState* m_SamplerState;
	
	int m_Width;
	int m_Height;
	bool m_GuiTexture;

	virtual bool LoadFileOld();
	virtual bool LoadFile();
	void Unload();

public:
	CTexture();
	virtual ~CTexture();
	bool Load(const std::string &Filename, bool GuiTexture); 
	void Activate(unsigned int StageId); 
	bool Reload();

	bool UsedInGui() const{ return m_GuiTexture; };
	int GetWidth(){return m_Width;};
	int GetHeight(){return m_Height;};
};

#endif
