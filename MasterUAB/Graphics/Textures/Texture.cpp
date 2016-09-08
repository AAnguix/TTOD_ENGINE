#include "Textures\Texture.h"
//#include <d3dx11tex.h>
#include "Engine\Engine.h"
#include "Render\RenderManager.h"
#include "Render\ContextManager.h"
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include "Log\Log.h"

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

bool CTexture::Load(const std::string &Filename, bool GuiTexture)
{  
	m_Name=Filename;
	m_GuiTexture = GuiTexture;
	return LoadFile();
} 

bool CTexture::LoadFileOld()
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

bool CTexture::LoadFile()
{
	ID3D11Device *l_Device = CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDevice();
	std::string l_Format = m_Name.substr(m_Name.find_last_of(".") + 1);

	
	
	std::wstring ws;
	ws.assign(m_Name.begin(), m_Name.end());

	HRESULT l_Hresult = 1;
	ID3D11DeviceContext *l_DeviceContext = CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDeviceContext();

	if (l_Format == "dds")
	{
		//if (!m_GuiTexture)
		l_Hresult = DirectX::CreateDDSTextureFromFile(l_Device, ws.c_str(), nullptr, &m_Texture);
		
		//DirectX::DDS_ALPHA_MODE l_AlphaMode;
		//l_Hresult = DirectX::CreateDDSTextureFromFileEx(l_Device, ws.c_str(), nullptr, D3D11_USAGE_DEFAULT, 0, 0, 0, 0.0f, nullptr, &m_Texture, &l_AlphaMode);
		
		//l_Hresult = DirectX::CreateDDSTextureFromFile(l_Device, l_DeviceContext, ws.c_str(), nullptr, &m_Texture, 0, &l_AlphaMode);
	}
	else if (l_Format == "png")
	{
		l_Hresult = DirectX::CreateWICTextureFromFile(l_Device, l_DeviceContext, ws.c_str(), nullptr, &m_Texture);
	}
	else assert(false);

	#ifdef _DEBUG
	if (FAILED(l_Hresult))
	{
		CEngine::GetSingleton().GetLogManager()->Log("Error loading texture " + m_Name);
		assert(false);
	}
	#endif

	D3D11_SAMPLER_DESC l_SampDesc;
	ZeroMemory(&l_SampDesc, sizeof(l_SampDesc));

	if (!m_GuiTexture)
	{
		l_SampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		l_SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		l_SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		l_SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		l_SampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		l_SampDesc.MinLOD = 0;
		l_SampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	}
	else
	{
		l_SampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		l_SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		l_SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		l_SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		l_SampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		l_SampDesc.MinLOD = 0;
		l_SampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		//1 No genere Bitmaping.
		//2. Cargarlo al tamaño real.
		//3. Que no haga filtro de puntos, lineal o isotropico.
		//4. Que haga clamp.
	}

	l_Hresult = l_Device->CreateSamplerState(&l_SampDesc, &m_SamplerState);
	
	return true;
}

void CTexture::Unload()
{
	if (m_SamplerState)
	{
		m_SamplerState->Release();
		m_SamplerState = nullptr;
	}
	if (m_Texture)
	{
		m_Texture->Release();
		m_Texture = nullptr;
	}
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
	return LoadFile();
}