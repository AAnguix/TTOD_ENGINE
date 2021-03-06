#include "DynamicTexture.h"
#include "Engine\Engine.h"
#include "Render\RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "Render\ContextManager.h"

CDynamicTexture::CDynamicTexture(const std::string &Name, int Width, int Height, bool CreateDepthStencilBuffer, TFormatType FormatType) : m_RenderTargetTexture(0) , m_RenderTargetView(0), m_DepthStencilBuffer(0), m_DepthStencilView(0)   
{
	SetName(Name);
	m_Width=Width;
	m_Height=Height;
	m_CreateDepthStencilBuffer=CreateDepthStencilBuffer;
	m_FormatType = FormatType;

	Init();
}

CDynamicTexture::CDynamicTexture(const CXMLTreeNode &TreeNode)
:m_RenderTargetTexture(nullptr)
,m_RenderTargetView(nullptr)
,m_DepthStencilBuffer(nullptr)
,m_DepthStencilView(nullptr)
{
	SetName(TreeNode.GetPszProperty("name"));
	m_Width=TreeNode.GetIntProperty("width");
	m_Height=TreeNode.GetIntProperty("height");
	m_CreateDepthStencilBuffer=TreeNode.GetBoolProperty("create_depth_stencil_buffer");
	m_FormatType = GetFormatTypeFromString(TreeNode.GetPszProperty("format_type"));

	Init();
}

CDynamicTexture::~CDynamicTexture()
{
	Unload();
}

void CDynamicTexture::Unload()
{
	if (m_DepthStencilView)
	{
		m_DepthStencilView->Release();
		m_DepthStencilView = nullptr;
	}

	if (m_DepthStencilBuffer)
	{
		m_DepthStencilBuffer->Release();
		m_DepthStencilBuffer = nullptr;
	}

	if (m_RenderTargetView)
	{
		m_RenderTargetView->Release();
		m_RenderTargetView = nullptr;
	}

	if (m_RenderTargetTexture)
	{
		m_RenderTargetTexture->Release();
		m_RenderTargetTexture = nullptr;
	}
}

void CDynamicTexture::Init()
{
	ID3D11Device *l_Device=CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDevice();
	D3D11_TEXTURE2D_DESC l_TextureDescription;
	ZeroMemory(&l_TextureDescription, sizeof(D3D11_TEXTURE2D_DESC));
	l_TextureDescription.Width=m_Width;
	l_TextureDescription.Height=m_Height;
	l_TextureDescription.MipLevels=1;

	l_TextureDescription.ArraySize=1;
	//l_TextureDescription.Format=DXGI_FORMAT_R32G32B32A32_FLOAT;
	l_TextureDescription.Format=GetD3D11FormatType(m_FormatType);
	l_TextureDescription.SampleDesc.Count=1;
	l_TextureDescription.Usage=D3D11_USAGE_DEFAULT;
	l_TextureDescription.BindFlags=D3D11_BIND_RENDER_TARGET |D3D11_BIND_SHADER_RESOURCE;
	l_TextureDescription.CPUAccessFlags=0;
	l_TextureDescription.MiscFlags=0;

	HRESULT l_HR=l_Device->CreateTexture2D(&l_TextureDescription, NULL,&m_RenderTargetTexture);
	assert(!FAILED(l_HR));
	

	D3D11_RENDER_TARGET_VIEW_DESC l_RenderTargetViewDescription;
	l_RenderTargetViewDescription.Format=l_TextureDescription.Format;
	l_RenderTargetViewDescription.ViewDimension=D3D11_RTV_DIMENSION_TEXTURE2D;
	l_RenderTargetViewDescription.Texture2D.MipSlice=0;
	
	l_HR=l_Device->CreateRenderTargetView(m_RenderTargetTexture,&l_RenderTargetViewDescription, &m_RenderTargetView);
	assert(!FAILED(l_HR));
	
	
	D3D11_SHADER_RESOURCE_VIEW_DESC l_ShaderResourceViewDescription;
	l_ShaderResourceViewDescription.Format=l_TextureDescription.Format;
	l_ShaderResourceViewDescription.ViewDimension=D3D11_SRV_DIMENSION_TEXTURE2D;
	l_ShaderResourceViewDescription.Texture2D.MostDetailedMip=0;
	l_ShaderResourceViewDescription.Texture2D.MipLevels=1;
	
	l_HR=l_Device->CreateShaderResourceView(m_RenderTargetTexture,
	&l_ShaderResourceViewDescription, &m_Texture);
	assert(!FAILED(l_HR));
	
	if(m_CreateDepthStencilBuffer)
	{
		//Textura para el Buffer de profundidad
		D3D11_TEXTURE2D_DESC l_DepthBufferDescription;
		ZeroMemory(&l_DepthBufferDescription, sizeof(D3D11_TEXTURE2D_DESC));
		l_DepthBufferDescription.Width=m_Width;
		l_DepthBufferDescription.Height=m_Height;
		l_DepthBufferDescription.MipLevels=1;
		l_DepthBufferDescription.ArraySize=1;
		l_DepthBufferDescription.Format=DXGI_FORMAT_D24_UNORM_S8_UINT;
		l_DepthBufferDescription.SampleDesc.Count=1;
		l_DepthBufferDescription.SampleDesc.Quality=0;
		l_DepthBufferDescription.Usage=D3D11_USAGE_DEFAULT;
		l_DepthBufferDescription.BindFlags=D3D11_BIND_DEPTH_STENCIL;
		l_DepthBufferDescription.CPUAccessFlags=0;
		l_DepthBufferDescription.MiscFlags=0;
		l_HR=l_Device->CreateTexture2D(&l_DepthBufferDescription, NULL,&m_DepthStencilBuffer);
		assert(!FAILED(l_HR));

		D3D11_DEPTH_STENCIL_VIEW_DESC l_DepthStencilViewDescription;
		ZeroMemory(&l_DepthStencilViewDescription,sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		l_DepthStencilViewDescription.Format=DXGI_FORMAT_D24_UNORM_S8_UINT;
		l_DepthStencilViewDescription.ViewDimension=D3D11_DSV_DIMENSION_TEXTURE2D;
		l_DepthStencilViewDescription.Texture2D.MipSlice=0;
		l_HR=l_Device->CreateDepthStencilView(m_DepthStencilBuffer,
		&l_DepthStencilViewDescription, &m_DepthStencilView);
		assert(!FAILED(l_HR));
	}

	CreateSamplerState();
}

bool CDynamicTexture::CreateSamplerState()
{
	ID3D11Device *l_Device=CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDevice();
	D3D11_SAMPLER_DESC l_SampDesc;
	ZeroMemory(&l_SampDesc, sizeof(l_SampDesc));
	l_SampDesc.Filter=D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	l_SampDesc.AddressU=D3D11_TEXTURE_ADDRESS_CLAMP;
	l_SampDesc.AddressV=D3D11_TEXTURE_ADDRESS_CLAMP;
	l_SampDesc.AddressW=D3D11_TEXTURE_ADDRESS_CLAMP;
	l_SampDesc.ComparisonFunc=D3D11_COMPARISON_NEVER;
	l_SampDesc.MinLOD=0;
	l_SampDesc.MaxLOD=D3D11_FLOAT32_MAX;
	HRESULT l_HR=l_Device->CreateSamplerState(&l_SampDesc, &m_SamplerState);
	return !FAILED(l_HR);
}

bool CDynamicTexture::LoadFile()
{
	return true;
}

		
bool CDynamicTexture::Reload()    
{
	Unload();
	Init();
	return true;
}

DXGI_FORMAT CDynamicTexture::GetD3D11FormatType(TFormatType FormatType)
{
	switch(FormatType)
	{
		case R8G8B8A8_UNORM  : return DXGI_FORMAT_R8G8B8A8_UNORM;
		case R32_FLOAT : return DXGI_FORMAT_R32_FLOAT; 
		case R32G32B32A32_FLOAT : return DXGI_FORMAT_R32G32B32A32_FLOAT;
		default : return DXGI_FORMAT_R8G8B8A8_UNORM;
	}
}; 

CDynamicTexture::TFormatType CDynamicTexture::GetFormatTypeFromString(const std::string &Format)
{	
	if (Format == "R32_FLOAT") 
		return CDynamicTexture::R32_FLOAT;
	else if (Format == "R32G32B32A32_FLOAT") 
		return CDynamicTexture::R32G32B32A32_FLOAT;
	else if (Format == "R8G8B8A8_UNORM") 
		return CDynamicTexture::R8G8B8A8_UNORM;
	else 
		return CDynamicTexture::R8G8B8A8_UNORM;
};
