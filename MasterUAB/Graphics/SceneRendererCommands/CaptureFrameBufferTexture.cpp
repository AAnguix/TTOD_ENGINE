#include "CaptureFrameBufferTexture.h"
#include "Engine\Engine.h"
#include "Render\RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "Render\ContextManager.h"

void CCaptureFrameBufferTexture::Init(const std::string &Name, unsigned int Width, unsigned int Height)
{
	m_Width=Width;
	m_Height=Height;
	SetName(Name);

	CRenderManager* l_RenderManager=CEngine::GetSingleton().GetRenderManager();
	ID3D11Device *l_Device=l_RenderManager->GetContextManager()->GetDevice();
	ID3D11Texture2D *l_Buffer=NULL;
	
	l_RenderManager->GetContextManager()->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D),(LPVOID*)&l_Buffer);
	
	D3D11_TEXTURE2D_DESC l_Texture2DDescription;
	ZeroMemory(&l_Texture2DDescription, sizeof(l_Texture2DDescription));
	l_Texture2DDescription.Width=Width;
	l_Texture2DDescription.Height=Height;
	l_Texture2DDescription.BindFlags=D3D11_BIND_RENDER_TARGET |D3D11_BIND_SHADER_RESOURCE;
	l_Texture2DDescription.ArraySize=1;
	l_Texture2DDescription.MiscFlags=0;
	l_Texture2DDescription.MipLevels=1;
	l_Texture2DDescription.CPUAccessFlags=0;
	l_Texture2DDescription.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
	l_Texture2DDescription.SampleDesc.Count=1;
	l_Texture2DDescription.SampleDesc.Quality=0;
	l_Texture2DDescription.Usage=D3D11_USAGE_DEFAULT;
	
	HRESULT l_HR=l_Device->CreateTexture2D(&l_Texture2DDescription, NULL,&m_DataTexture);

	if(FAILED(l_HR))
		return;

	D3D11_SHADER_RESOURCE_VIEW_DESC DescRV;
	ZeroMemory(&DescRV, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	DescRV.Format=l_Texture2DDescription.Format;
	DescRV.ViewDimension=D3D11_SRV_DIMENSION_TEXTURE2D;
	DescRV.Texture2D.MipLevels=l_Texture2DDescription.MipLevels;
	DescRV.Texture2D.MostDetailedMip=0;
	l_HR=l_Device->CreateShaderResourceView(m_DataTexture, &DescRV, &m_Texture);
	
	if(FAILED(l_HR))
		return;
	CreateSamplerState();
}

void CCaptureFrameBufferTexture::Unload()
{
	m_DataTexture->Release();
	m_DataTexture = 0;
	CTexture::Unload();
}

bool CCaptureFrameBufferTexture::CreateSamplerState()
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

bool CCaptureFrameBufferTexture::Reload()
{
	Unload();
	Init(m_Name,m_Width,m_Height);
	return true;
}

CCaptureFrameBufferTexture::CCaptureFrameBufferTexture(const CXMLTreeNode &TreeNode) 
{
	Init(TreeNode.GetPszProperty("name"),TreeNode.GetIntProperty("width"),TreeNode.GetIntProperty("height"));
}

CCaptureFrameBufferTexture::CCaptureFrameBufferTexture(const std::string &Name, unsigned int Width, unsigned int Height)
{
	Init(Name,Width,Height);
}

CCaptureFrameBufferTexture::~CCaptureFrameBufferTexture()
{
	m_DataTexture->Release();
	m_DataTexture = 0;
}

bool CCaptureFrameBufferTexture::Capture(unsigned int StageId)
{
	CRenderManager* l_RenderManager=CEngine::GetSingleton().GetRenderManager();
	ID3D11Texture2D *l_Surface=NULL;
	HRESULT l_HR = l_RenderManager->GetContextManager()->GetSwapChain()->GetBuffer(StageId, __uuidof(ID3D11Texture2D ), reinterpret_cast< void** >(&l_Surface));
	
	if(FAILED(l_HR) || l_Surface==NULL || m_DataTexture==NULL)
		return false;

	l_RenderManager->GetContextManager()->GetDeviceContext()->CopyResource(m_DataTexture, l_Surface);
	l_Surface->Release();
	return true;
}




