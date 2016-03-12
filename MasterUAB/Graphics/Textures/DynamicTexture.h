#ifndef _DYNAMICTEXTURE_H
#define _DYNAMICTEXTURE_H

#include "Textures\Texture.h"
#include <D3D11.h>

class CDynamicTexture : public CTexture 
{ 

public:
	enum TFormatType
	{
		R8G8B8A8_UNORM=0,
		R32_FLOAT,
		R32G32B32A32_FLOAT
	};

protected:  
	TFormatType  m_FormatType;

	bool  m_CreateDepthStencilBuffer;

	ID3D11Texture2D   *m_RenderTargetTexture;  
	ID3D11RenderTargetView  *m_RenderTargetView;  
	ID3D11Texture2D   *m_DepthStencilBuffer;  
	ID3D11DepthStencilView  *m_DepthStencilView;  
		
	void Unload();  
	void Init();  
	virtual bool CreateSamplerState(); 
	bool LoadFile();
	
public:  
	CDynamicTexture(const std::string &Name, int Width, int Height, bool CreateDepthStencilBuffer, TFormatType FormatType);  
	CDynamicTexture(const CXMLTreeNode &TreeNode);  
	~CDynamicTexture(); 
		
	bool Reload();    
		
	ID3D11RenderTargetView* GetRenderTargetView(){return m_RenderTargetView;}; 
	ID3D11DepthStencilView* GetDepthStencilView(){return m_DepthStencilView;};

	static DXGI_FORMAT GetD3D11FormatType(TFormatType FormatType);

	static TFormatType GetFormatTypeFromString(const std::string &Format);
}; 

#endif
	

