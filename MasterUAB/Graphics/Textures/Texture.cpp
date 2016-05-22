#include "Textures\Texture.h"
//#include <d3dx11tex.h>
#include "Engine.h"
#include "Render\RenderManager.h"
#include "Render\ContextManager.h"

#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include <wrl.h>
#include "Log.h"

CTexture::CTexture() : CNamed("")
,m_SamplerState(NULL)
,m_Texture(NULL)
,m_Width(0)
,m_Height(0)
{
	
}

CTexture::~CTexture()
{
	Unload();
}

bool CTexture::Load(const std::string &Filename) 
{  
	m_Name=Filename;
	return LoadFileNew();
} 

bool CTexture::LoadFile()
{
	
	/*ID3D11Device *l_Device=CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDevice();  
	HRESULT l_HR=D3DX11CreateShaderResourceViewFromFile(l_Device, m_Name.c_str(), NULL, NULL, &m_Texture, NULL );  
	
	D3D11_SAMPLER_DESC l_SampDesc;  
	ZeroMemory(&l_SampDesc, sizeof(l_SampDesc));  
	l_SampDesc.Filter=D3D11_FILTER_MIN_MAG_MIP_LINEAR; 
	l_SampDesc.AddressU=D3D11_TEXTURE_ADDRESS_WRAP; 
	l_SampDesc.AddressV=D3D11_TEXTURE_ADDRESS_WRAP;  
	l_SampDesc.AddressW=D3D11_TEXTURE_ADDRESS_WRAP;  
	l_SampDesc.ComparisonFunc=D3D11_COMPARISON_NEVER;  
	l_SampDesc.MinLOD=0;  
	l_SampDesc.MaxLOD=D3D11_FLOAT32_MAX; 

	l_HR=l_Device->CreateSamplerState(&l_SampDesc, &m_SamplerState);  return !FAILED(l_HR); */
	return true;
}

bool CTexture::LoadFileNew()
{
	ID3D11Device *l_Device = CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDevice();
	
	std::string l_Format = m_Name.substr(m_Name.find_last_of(".") + 1);

	DirectX::DDS_ALPHA_MODE l_AlphaMode;
	
	std::wstring ws;
	ws.assign(m_Name.begin(), m_Name.end());

	HRESULT l_Hresult = 1;

	if (l_Format == "dds")
	{
		l_Hresult = DirectX::CreateDDSTextureFromFile(l_Device, ws.c_str(), nullptr, &m_Texture, 0, &l_AlphaMode);
	}
	else if (l_Format == "png")
	{
		ID3D11DeviceContext *l_DeviceContext = CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDeviceContext();
		l_Hresult = DirectX::CreateWICTextureFromFile(l_Device, l_DeviceContext, ws.c_str(), nullptr, &m_Texture, 0);
	}

	if (l_Hresult == ERROR_FILE_NOT_FOUND)
	{
		CEngine::GetSingleton().GetLogManager()->Log("File " + m_Name + " not found.");
	}

	if (FAILED(l_Hresult))
	{
		assert(false);
	}

	D3D11_SAMPLER_DESC l_SampDesc;
	ZeroMemory(&l_SampDesc, sizeof(l_SampDesc));
	l_SampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	l_SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	l_SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	l_SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	l_SampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	l_SampDesc.MinLOD = 0;
	l_SampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	l_Hresult = l_Device->CreateSamplerState(&l_SampDesc, &m_SamplerState);
	
	return true;
}

void CTexture::Unload()
{
	CHECKED_RELEASE(m_SamplerState);
	CHECKED_RELEASE(m_Texture);
}

void CTexture::Activate(unsigned int StageId) 
{  
	ID3D11DeviceContext *l_DeviceContext=CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDeviceContext();
	l_DeviceContext->PSSetSamplers(StageId, 1, &m_SamplerState);  
	l_DeviceContext->PSSetShaderResources(StageId, 1, &m_Texture); 
} 

bool CTexture::Reload() 
{  
	Unload();
	return LoadFileNew();
} 


