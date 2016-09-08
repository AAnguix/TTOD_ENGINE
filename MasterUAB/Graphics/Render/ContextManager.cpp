#include "Render\ContextManager.h"
#include "Vertex\RenderableVertexs.h"
#include "Engine\Engine.h"
#include <ScreenGrab.h>
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Effects\Effect.h"
#include "Effects\EffectVertexShader.h"
#include "Effects\EffectPixelShader.h"
#include "Effects\EffectGeometryShader.h"

//#pragma comment(lib,"d3d11.lib")

CContextManager::CContextManager()
:m_D3DDevice(nullptr)
,m_DeviceContext(nullptr)
,m_SwapChain(nullptr)
,m_RenderTargetView(nullptr)
,m_DepthStencil(nullptr)
,m_DepthStencilView(nullptr)
,m_D3DDebug(nullptr)
,m_Height(0)
,m_Width(0)
,m_NumViews(0)
,m_StencilTexture(nullptr)
,m_FullScreenEnabled(false)
,m_CurrentRenderTargetViews(nullptr)
,m_CurrentDepthStencilView(nullptr)
,m_AlphaBlendState(nullptr)
,m_AdditiveAlphaBlendState(nullptr)
,m_Viewport()
,m_VideoCardMemory(0)
,m_VSyncEnabled(false)
,m_NumDisplayModes(0)
,m_DisplayModeList(nullptr)
{

	for (int i = 0; i < RS_COUNT; ++i)
	{
		m_RasterizerSates[i] = nullptr;
	}
	for (int i = 0; i < DSS_COUNT; ++i)
	{
		m_DepthStencilStates[i] = nullptr;
	}
	/*
	for (int i = 0; i < BLEND_COUNT; ++i)
	{
	m_BlendStates[i] = nullptr;
	}*/
	m_AlphaBlendState = nullptr;

}

void CContextManager::Shutdown()
{
	if (m_DisplayModeList)
	{
		delete[] m_DisplayModeList;
		m_DisplayModeList = nullptr;
	}

	if (m_SwapChain)
	{
		m_SwapChain->SetFullscreenState(false, nullptr);
	}

	CHECKED_RELEASE(m_StencilTexture);

	for (int i = 0; i < RS_COUNT; ++i)
	{
		CHECKED_RELEASE(m_RasterizerSates[i]);
	}

	CHECKED_RELEASE(m_DepthStencilView);
	for (int i = 0; i < DSS_COUNT; ++i)
	{
		CHECKED_RELEASE(m_DepthStencilStates[i]);
	}
	CHECKED_RELEASE(m_DepthStencil);

	CHECKED_RELEASE(m_DeviceContext);
	/*
	for (int i = 0; i < BLEND_COUNT; ++i)
	{
	CHECKED_RELEASE(m_BlendStates[i]);
	}*/
	CHECKED_RELEASE(m_AlphaBlendState);

	CHECKED_RELEASE(m_RenderTargetView);

	if (m_DeviceContext)
	{
		m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	}

	if (m_D3DDebug)
		m_D3DDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

	CHECKED_RELEASE(m_D3DDebug);

	CHECKED_RELEASE(m_D3DDevice);
	
	CHECKED_RELEASE(m_SwapChain);
}

DXGI_MODE_DESC CContextManager::GetDxgiModeDesc(unsigned int ScreenWidth, unsigned int ScreenHeight)
{
	for (size_t i = 0; i < m_NumDisplayModes; ++i)
	{
		if (m_DisplayModeList[i].Width == ScreenWidth && m_DisplayModeList[i].Height == ScreenHeight)
			return m_DisplayModeList[i];
	}
	assert(false);
	return m_DisplayModeList[0];
}

bool CContextManager::Initialize(HWND Hwnd, unsigned int ScreenWidth, unsigned int ScreenHeight, bool FullScreen, bool VSync, bool DebugMode)
{
	m_VSyncEnabled = VSync;

	HRESULT l_Result;

	IDXGIFactory* l_Factory;
	IDXGIAdapter* l_Adapter;
	IDXGIOutput* l_AdapterOutput;

	unsigned int l_Numerator=0, l_Denominator=0;
	DXGI_ADAPTER_DESC l_AdapterDesc;
	
	l_Result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&l_Factory);
	if (FAILED(l_Result)){return false;}

	l_Result = l_Factory->EnumAdapters(0, &l_Adapter);
	if (FAILED(l_Result)){ return false; }
	
	l_Result = l_Adapter->EnumOutputs(0, &l_AdapterOutput);
	if (FAILED(l_Result)){ return false; }

	l_Result = l_AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &m_NumDisplayModes, NULL);
	if (FAILED(l_Result)){ return false; }

	m_DisplayModeList = new DXGI_MODE_DESC[m_NumDisplayModes];
	if (!m_DisplayModeList){ return false; }

	l_Result = l_AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &m_NumDisplayModes, m_DisplayModeList);
	if (FAILED(l_Result)){ return false; }

	for (unsigned int i = 0; i<m_NumDisplayModes; ++i)
	{
		if (m_DisplayModeList[i].Width == (unsigned int)ScreenWidth)
		{
			if (m_DisplayModeList[i].Height == (unsigned int)ScreenHeight)
			{
				l_Numerator = m_DisplayModeList[i].RefreshRate.Numerator;
				l_Denominator = m_DisplayModeList[i].RefreshRate.Denominator;
				l_Result = S_OK;
			}
		}
	}
	if (FAILED(l_Result)){ return false; }


	l_Result = l_Adapter->GetDesc(&l_AdapterDesc);
	if (FAILED(l_Result)){ return false; }

	m_VideoCardMemory = (int)(l_AdapterDesc.DedicatedVideoMemory / 1024 / 1024);

	size_t l_StringLength;
	
	int l_Error = wcstombs_s(&l_StringLength, m_VideoCardDescription, 128, l_AdapterDesc.Description, 128);
	if (l_Error != 0) {	return false; }

	l_AdapterOutput->Release();
	l_AdapterOutput = 0;
	l_Adapter->Release();
	l_Adapter = 0;
	
	DXGI_SWAP_CHAIN_DESC l_SwapChainDesc;
	ZeroMemory(&l_SwapChainDesc, sizeof(l_SwapChainDesc));
	l_SwapChainDesc.BufferCount = 1;
	l_SwapChainDesc.BufferDesc.Width = ScreenWidth;
	l_SwapChainDesc.BufferDesc.Height = ScreenHeight;
	l_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	if (VSync)
	{
		l_SwapChainDesc.BufferDesc.RefreshRate.Numerator = l_Numerator;
		l_SwapChainDesc.BufferDesc.RefreshRate.Denominator = l_Denominator;
	}
	else
	{
		l_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		l_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	l_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	l_SwapChainDesc.OutputWindow = Hwnd;
	l_SwapChainDesc.SampleDesc.Count = 1;
	l_SwapChainDesc.SampleDesc.Quality = 0;
	if (FullScreen)
	{
		l_SwapChainDesc.Windowed = false;
		m_FullScreenEnabled = true;
	}
	else l_SwapChainDesc.Windowed = true;
	
	l_SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	l_SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	//l_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	l_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL l_FeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT l_NumFeatureLevels = ARRAYSIZE(l_FeatureLevels);


	int l_Flags = 0;
	#if _DEBUG && DebugMode
		l_Flags = D3D11_CREATE_DEVICE_DEBUG;
	#else
		l_Flags = 0;
	#endif

	l_Result = (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, l_Flags, l_FeatureLevels, l_NumFeatureLevels,
	D3D11_SDK_VERSION, &l_SwapChainDesc, &m_SwapChain, &m_D3DDevice, NULL, &m_DeviceContext));
	if (FAILED(l_Result)){ return false; }

	/*if (FullScreen)
	{
		SetFullScreen(true, ScreenWidth, ScreenHeight);
		m_FullScreenEnabled = true;
	}*/
	/*
	#if _DEBUG
	HRESULT hr = m_D3DDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_D3DDebug));
	if (FAILED(hr))
	return hr;
	#endif*/

	//Alt+Enter
	//l_Result = m_SwapChain->GetParent(__uuidof(IDXGIFactory), (void **)&l_Factory);
	//if (FAILED(l_Result)){return false;}

	l_Result = l_Factory->MakeWindowAssociation(Hwnd, DXGI_MWA_NO_ALT_ENTER);
	// Prevent DXGI from responding to an alt-enter sequence.
	if (FAILED(l_Result)){ return false; }

	l_Factory->Release();
	l_Factory = 0;

	m_Viewport.Width = (FLOAT)ScreenWidth;
	m_Viewport.Height = (FLOAT)ScreenHeight;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &m_Viewport);

	return true;
}

/*Returns by reference the video card name/memory*/
void CContextManager::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_VideoCardDescription);
	memory = m_VideoCardMemory;
	return;
}

void CContextManager::SetFullScreen(bool Value, int ScreenWidth, int ScreenHeight)
{
	if (m_SwapChain)
	{
		if ((m_FullScreenEnabled && !Value) || (!m_FullScreenEnabled && Value))
		{
			DXGI_MODE_DESC l_NewTargetParameters = GetDxgiModeDesc(ScreenWidth, ScreenHeight);
			m_SwapChain->SetFullscreenState(Value, NULL);
			m_SwapChain->ResizeTarget(&l_NewTargetParameters);
			m_FullScreenEnabled = !m_FullScreenEnabled;
		}
	}
}

DXGI_FORMAT CContextManager::GetDepthResourceFormat(DXGI_FORMAT DepthFormat)
{
	DXGI_FORMAT l_Resformat;
	switch (DepthFormat)
	{
	case DXGI_FORMAT::DXGI_FORMAT_D16_UNORM:
		l_Resformat = DXGI_FORMAT::DXGI_FORMAT_R16_TYPELESS;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:
		l_Resformat = DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT:
		l_Resformat = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		l_Resformat = DXGI_FORMAT::DXGI_FORMAT_R32G8X24_TYPELESS;
		break;
	}

	return l_Resformat;
}

DXGI_FORMAT CContextManager::GetDepthShaderResourceViewFormat(DXGI_FORMAT Depthformat)
{
	DXGI_FORMAT l_Srvformat;
	switch (Depthformat)
	{
	case DXGI_FORMAT::DXGI_FORMAT_D16_UNORM:
		l_Srvformat = DXGI_FORMAT::DXGI_FORMAT_R16_FLOAT;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:
		l_Srvformat = DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT:
		l_Srvformat = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		l_Srvformat = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
		break;
	}
	return l_Srvformat;
}

bool CContextManager::ResizeBuffers(HWND hWnd, unsigned int Width, unsigned int Height)
{
	if (m_SwapChain)
	{
		if (m_DeviceContext)
			m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		/*for(int i=0;i<MAX_RENDER_TARGETS;++i)
		CHECKED_RELEASE(m_CurrentRenderTargetViews[i]);*/
		CHECKED_RELEASE(m_RenderTargetView);
		CHECKED_RELEASE(m_DepthStencil);
		CHECKED_RELEASE(m_DepthStencilView);

		//m_DeviceContext->ClearState();
		//m_DeviceContext->FinishCommandList();

		HRESULT l_Hr = m_SwapChain->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, 0);
		assert(!FAILED(l_Hr));

		bool l_Result = CreateBackBuffer(hWnd, Width, Height);
		
		return l_Result;
	}
	return false;
}

bool CContextManager::CreateBackBuffer(HWND hWnd, unsigned int Width, unsigned int Height)
{
	HRESULT l_Result;

	m_Width = Width;
	m_Height = Height;

	ID3D11Texture2D *pBackBuffer;
	l_Result = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(l_Result)){ return false; }
		
	l_Result = m_D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_RenderTargetView);
	if (FAILED(l_Result)){ return false; }
	pBackBuffer->Release();
	pBackBuffer = 0;

	DXGI_FORMAT l_Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DXGI_FORMAT l_Resformat = GetDepthResourceFormat(l_Format);
	DXGI_FORMAT l_Srvformat = GetDepthShaderResourceViewFormat(l_Format);

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = Width;
	descDepth.Height = Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = l_Resformat;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	l_Result = m_D3DDevice->CreateTexture2D(&descDepth, NULL, &m_DepthStencil);
	if (FAILED(l_Result)){ return false; }

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = l_Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	l_Result = m_D3DDevice->CreateDepthStencilView(m_DepthStencil, &descDSV, &m_DepthStencilView);
	if (FAILED(l_Result)){ return false; }

	SetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
	
	/*D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvd.Format = l_Srvformat;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = descDepth.MipLevels;
	srvd.Texture2D.MostDetailedMip = 0;
	
	hr = m_D3DDevice->CreateShaderResourceView(m_DepthStencil, &srvd, &m_StencilTexture);*/

	return true;
}

bool CContextManager::SaveStencilBufferToFile()
{
	std::string Path = "./Data/StencilBuffer.dds";
	std::wstring l_WStr(Path.begin(), Path.end());

	HRESULT l_Result = DirectX::SaveDDSTextureToFile(m_DeviceContext, m_DepthStencil, l_WStr.c_str());
	if (FAILED(l_Result))
		return false;
	return true;
}


class CDebugCEffect : public CEffect
{
public:
	bool Load(ID3D11Device *Device)
	{
		const char vs[] = {
			0x44, 0x58, 0x42, 0x43, 0x34, 0x4f, 0xb6, 0xe4, 0xbc, 0xb2, 0xdf, 0x47, 0x4e, 0x87, 0x8b, 0x7d, 0x80, 0xfc, 0x58, 0x1d, 0x01, 0x00, 0x00, 0x00, 0x60, 0x05, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x34
			, 0x00, 0x00, 0x00, 0x0c, 0x02, 0x00, 0x00, 0x5c, 0x02, 0x00, 0x00, 0xb0, 0x02, 0x00, 0x00, 0xe4, 0x04, 0x00, 0x00, 0x52, 0x44, 0x45, 0x46, 0xd0, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x4c
			, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x04, 0xfe, 0xff, 0x00, 0x01, 0x00, 0x00, 0x9c, 0x01, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x43, 0x6f, 0x6e, 0x73, 0x74, 0x61, 0x6e, 0x74, 0x42
			, 0x75, 0x66, 0x66, 0x65, 0x72, 0x00, 0xab, 0x3c, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c
			, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x14, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x01, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40
			, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x14, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x14
			, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x01, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54
			, 0x01, 0x00, 0x00, 0xd0, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x01, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x10
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79, 0x01, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x8c
			, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x5f, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x00, 0x03, 0x00, 0x03, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6d
			, 0x5f, 0x56, 0x69, 0x65, 0x77, 0x00, 0x6d, 0x5f, 0x50, 0x72, 0x6f, 0x6a, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x00, 0x6d, 0x5f, 0x42, 0x61, 0x73, 0x65, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x00, 0x01
			, 0x00, 0x03, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x5f, 0x43, 0x61, 0x6d, 0x65, 0x72, 0x61, 0x52, 0x69, 0x67, 0x68, 0x74, 0x56, 0x65, 0x63, 0x74
			, 0x6f, 0x72, 0x00, 0x6d, 0x5f, 0x43, 0x61, 0x6d, 0x65, 0x72, 0x61, 0x55, 0x70, 0x56, 0x65, 0x63, 0x74, 0x6f, 0x72, 0x00, 0x6d, 0x5f, 0x44, 0x65, 0x62, 0x75, 0x67, 0x52, 0x65, 0x6e, 0x64, 0x65
			, 0x72, 0x53, 0x63, 0x61, 0x6c, 0x65, 0x00, 0x00, 0x00, 0x03, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4d, 0x69, 0x63, 0x72, 0x6f, 0x73, 0x6f, 0x66, 0x74
			, 0x20, 0x28, 0x52, 0x29, 0x20, 0x48, 0x4c, 0x53, 0x4c, 0x20, 0x53, 0x68, 0x61, 0x64, 0x65, 0x72, 0x20, 0x43, 0x6f, 0x6d, 0x70, 0x69, 0x6c, 0x65, 0x72, 0x20, 0x36, 0x2e, 0x33, 0x2e, 0x39, 0x36
			, 0x30, 0x30, 0x2e, 0x31, 0x36, 0x33, 0x38, 0x34, 0x00, 0xab, 0xab, 0x49, 0x53, 0x47, 0x4e, 0x48, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x07, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03
			, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x50, 0x4f, 0x53, 0x49, 0x54, 0x49, 0x4f, 0x4e, 0x00, 0x43, 0x4f, 0x4c, 0x4f, 0x52, 0x00, 0xab, 0x4f, 0x53, 0x47, 0x4e, 0x4c
			, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f
			, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x53, 0x56, 0x5f, 0x50, 0x4f
			, 0x53, 0x49, 0x54, 0x49, 0x4f, 0x4e, 0x00, 0x43, 0x4f, 0x4c, 0x4f, 0x52, 0x00, 0xab, 0xab, 0x53, 0x48, 0x44, 0x52, 0x2c, 0x02, 0x00, 0x00, 0x40, 0x00, 0x01, 0x00, 0x8b, 0x00, 0x00, 0x00, 0x59
			, 0x00, 0x00, 0x04, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x5f, 0x00, 0x00, 0x03, 0x72, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5f, 0x00, 0x00, 0x03, 0xf2
			, 0x10, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x67, 0x00, 0x00, 0x04, 0xf2, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x03, 0xf2, 0x20, 0x10, 0x00, 0x01
			, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x08, 0x72, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x12, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06
			, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x05, 0x82, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x40, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x11
			, 0x00, 0x00, 0x08, 0x12, 0x00, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x22, 0x00, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x42, 0x00, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x82, 0x00, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x12, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x22, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x42, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x82, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x12, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x22, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x42, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x82, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x38
			, 0x00, 0x00, 0x08, 0xf2, 0x20, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x1e, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x3e
			, 0x00, 0x00, 0x01, 0x53, 0x54, 0x41, 0x54, 0x74, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		const int vs_size = 1376;

		const char ps[] = {
			0x44, 0x58, 0x42, 0x43, 0x21, 0x6f, 0x65, 0x10, 0x36, 0xef, 0xfb, 0xc2, 0x96, 0x87, 0xee, 0x91, 0xfe, 0xb7, 0x7e, 0x15, 0x01, 0x00, 0x00, 0x00, 0xd0, 0x01, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x34
			, 0x00, 0x00, 0x00, 0x8c, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x14, 0x01, 0x00, 0x00, 0x54, 0x01, 0x00, 0x00, 0x52, 0x44, 0x45, 0x46, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x04, 0xff, 0xff, 0x00, 0x01, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x4d, 0x69, 0x63, 0x72, 0x6f, 0x73, 0x6f, 0x66, 0x74
			, 0x20, 0x28, 0x52, 0x29, 0x20, 0x48, 0x4c, 0x53, 0x4c, 0x20, 0x53, 0x68, 0x61, 0x64, 0x65, 0x72, 0x20, 0x43, 0x6f, 0x6d, 0x70, 0x69, 0x6c, 0x65, 0x72, 0x20, 0x36, 0x2e, 0x33, 0x2e, 0x39, 0x36
			, 0x30, 0x30, 0x2e, 0x31, 0x36, 0x33, 0x38, 0x34, 0x00, 0xab, 0xab, 0x49, 0x53, 0x47, 0x4e, 0x4c, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03
			, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x53, 0x56, 0x5f, 0x50, 0x4f, 0x53, 0x49, 0x54, 0x49, 0x4f, 0x4e, 0x00, 0x43, 0x4f, 0x4c, 0x4f, 0x52, 0x00, 0xab, 0xab, 0x4f
			, 0x53, 0x47, 0x4e, 0x2c, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x53, 0x56, 0x5f, 0x54, 0x61, 0x72, 0x67, 0x65, 0x74, 0x00, 0xab, 0xab, 0x53, 0x48, 0x44, 0x52, 0x38, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x0e
			, 0x00, 0x00, 0x00, 0x62, 0x10, 0x00, 0x03, 0xf2, 0x10, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x03, 0xf2, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x05, 0xf2
			, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x1e, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x01, 0x53, 0x54, 0x41, 0x54, 0x74, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		const int ps_size = 464;



		HRESULT l_HR = Device->CreateVertexShader(vs, vs_size, NULL, &m_VertexShader);
		assert(l_HR == S_OK);

		D3D11_INPUT_ELEMENT_DESC l_Layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		l_HR = Device->CreateInputLayout(l_Layout, 2, vs, vs_size, &m_VertexLayout);
		assert(l_HR == S_OK);


		l_HR = Device->CreatePixelShader(ps, ps_size, NULL, &m_PixelShader);
		assert(l_HR == S_OK);


		D3D11_BUFFER_DESC l_BufferDescription = {};

		l_BufferDescription.Usage = D3D11_USAGE_DEFAULT;
		l_BufferDescription.ByteWidth = sizeof(CSceneEffectParameters);
		l_BufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		l_BufferDescription.CPUAccessFlags = 0;
		assert(sizeof(CSceneEffectParameters) % 16 == 0 && "CEffectParameters' size must be a multiple of 16!!");
		l_HR = Device->CreateBuffer(&l_BufferDescription, nullptr, &m_ConstantBuffer);
		assert(l_HR == S_OK);

		return true;
	}
};

CEffect *s_DebugEffect;

CContextManager::~CContextManager()
{
	CHECKED_DELETE(s_DebugEffect);
}

bool CContextManager::InitStates()
{
	bool l_Result;
	CDebugCEffect *l_DebugEffect = new CDebugCEffect();
	l_DebugEffect->Load(m_D3DDevice);
	s_DebugEffect = l_DebugEffect;

	l_Result = InitRasterizerStates();
	if (!l_Result){ return false; }
	l_Result = InitDepthStencilStates();
	if (!l_Result){ return false; }
	l_Result = InitBlendingStates();
	if (!l_Result){ return false; }

	m_DeviceContext->RSSetState(m_RasterizerSates[RS_SOLID]);

	return true;
}

bool CContextManager::InitRasterizerStates()
{
	HRESULT l_Result;

	D3D11_RASTERIZER_DESC l_WireframeDesc;
	ZeroMemory(&l_WireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	l_WireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	l_WireframeDesc.CullMode = D3D11_CULL_NONE;
	l_WireframeDesc.FrontCounterClockwise = false;

	l_Result = m_D3DDevice->CreateRasterizerState(&l_WireframeDesc, &m_RasterizerSates[RS_WIREFRAME]);
	if (FAILED(l_Result)){ return false; }

	D3D11_RASTERIZER_DESC l_SolidDesc;
	ZeroMemory(&l_SolidDesc, sizeof(D3D11_RASTERIZER_DESC));
	l_SolidDesc.FillMode = D3D11_FILL_SOLID;
	l_SolidDesc.CullMode = D3D11_CULL_NONE;
	l_SolidDesc.FrontCounterClockwise = false;

	l_Result = m_D3DDevice->CreateRasterizerState(&l_SolidDesc, &m_RasterizerSates[RS_SOLID]);
	if (FAILED(l_Result)){ return false; }

	D3D11_RASTERIZER_DESC l_FrontalCulling;
	ZeroMemory(&l_FrontalCulling, sizeof(D3D11_RASTERIZER_DESC));
	l_FrontalCulling.FillMode = D3D11_FILL_SOLID;
	l_FrontalCulling.CullMode = D3D11_CULL_FRONT;
	l_FrontalCulling.FrontCounterClockwise = false;

	l_Result = m_D3DDevice->CreateRasterizerState(&l_FrontalCulling, &m_RasterizerSates[RS_CULL_FRONT]);
	if (FAILED(l_Result)){ return false; }

	D3D11_RASTERIZER_DESC l_BackCulling;
	ZeroMemory(&l_BackCulling, sizeof(D3D11_RASTERIZER_DESC));
	l_BackCulling.FillMode = D3D11_FILL_SOLID;
	l_BackCulling.CullMode = D3D11_CULL_BACK;
	l_BackCulling.FrontCounterClockwise = false;

	l_Result = m_D3DDevice->CreateRasterizerState(&l_BackCulling, &m_RasterizerSates[RS_CULL_BACK]);
	if (FAILED(l_Result)){ return false; }

	return true;
}
bool CContextManager::InitDepthStencilStates()
{
	HRESULT l_Result;
	D3D11_DEPTH_STENCIL_DESC l_DepthOnDesc = {};
	l_DepthOnDesc.DepthEnable = true;
	l_DepthOnDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	l_DepthOnDesc.DepthFunc = D3D11_COMPARISON_LESS;
	l_Result = m_D3DDevice->CreateDepthStencilState(&l_DepthOnDesc, &m_DepthStencilStates[DSS_DEPTH_ON]);
	if (FAILED(l_Result)){ return false; }

	D3D11_DEPTH_STENCIL_DESC l_DepthOffDesc = {};
	l_DepthOffDesc.DepthEnable = false;
	l_DepthOffDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	l_DepthOffDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	l_Result = m_D3DDevice->CreateDepthStencilState(&l_DepthOffDesc, &m_DepthStencilStates[DSS_OFF]);
	if (FAILED(l_Result)){ return false; }

	D3D11_DEPTH_STENCIL_DESC l_DepthTestDesc = {};
	l_DepthTestDesc.DepthEnable = true;
	l_DepthTestDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	l_DepthTestDesc.DepthFunc = D3D11_COMPARISON_LESS;

	l_Result = m_D3DDevice->CreateDepthStencilState(&l_DepthTestDesc, &m_DepthStencilStates[DSS_DEPTH_TEST]);
	if (FAILED(l_Result)){ return false; }
	
	return true;
}
bool CContextManager::InitBlendingStates()
{
	HRESULT l_Result;
	D3D11_BLEND_DESC l_AlphablendDesc;
	ZeroMemory(&l_AlphablendDesc, sizeof(D3D11_BLEND_DESC));
	l_AlphablendDesc.RenderTarget[0].BlendEnable = true;
	l_AlphablendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	l_AlphablendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	l_AlphablendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	l_AlphablendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	l_AlphablendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	l_AlphablendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	l_AlphablendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	l_Result = m_D3DDevice->CreateBlendState(&l_AlphablendDesc, &m_AlphaBlendState);
	if (FAILED(l_Result)){ return false; }

	D3D11_BLEND_DESC l_AdditiveAlphablendDesc;
	ZeroMemory(&l_AdditiveAlphablendDesc, sizeof(D3D11_BLEND_DESC));
	/*l_AdditiveAlphablendDesc.RenderTarget[0].BlendEnable = true;
	l_AdditiveAlphablendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	l_AdditiveAlphablendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	l_AdditiveAlphablendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	l_AdditiveAlphablendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	l_AdditiveAlphablendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	l_AdditiveAlphablendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	l_AdditiveAlphablendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;*/
	l_AdditiveAlphablendDesc.RenderTarget[0].BlendEnable = true;
	l_AdditiveAlphablendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	l_AdditiveAlphablendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	l_AdditiveAlphablendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	l_AdditiveAlphablendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	l_AdditiveAlphablendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	l_AdditiveAlphablendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	l_AdditiveAlphablendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	l_Result = m_D3DDevice->CreateBlendState(&l_AdditiveAlphablendDesc, &m_AdditiveAlphaBlendState);
	if (FAILED(l_Result)){ return false; }

	D3D11_BLEND_DESC l_SolidBlending = {};
	l_SolidBlending.RenderTarget[0].BlendEnable = false;
	l_SolidBlending.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	l_SolidBlending.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	l_SolidBlending.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	l_SolidBlending.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	l_SolidBlending.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	l_SolidBlending.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	l_SolidBlending.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	l_Result = m_D3DDevice->CreateBlendState(&l_SolidBlending, &m_BlendStates[BLEND_SOLID]);
	if (FAILED(l_Result)){ return false; }

	D3D11_BLEND_DESC l_ClassicBlending = {};
	l_ClassicBlending.RenderTarget[0].BlendEnable = true;
	l_ClassicBlending.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	l_ClassicBlending.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	l_ClassicBlending.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	l_ClassicBlending.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	l_ClassicBlending.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	l_ClassicBlending.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	l_ClassicBlending.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	l_Result = m_D3DDevice->CreateBlendState(&l_ClassicBlending, &m_BlendStates[BLEND_CLASSIC]);
	if (FAILED(l_Result)){ return false; }

	D3D11_BLEND_DESC l_PremultBlending = {};
	l_PremultBlending.RenderTarget[0].BlendEnable = true;
	l_PremultBlending.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	l_PremultBlending.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	l_PremultBlending.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	l_PremultBlending.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	l_PremultBlending.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	l_PremultBlending.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	l_PremultBlending.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	l_Result = m_D3DDevice->CreateBlendState(&l_PremultBlending, &m_BlendStates[BLEND_PREMULT]);
	if (FAILED(l_Result)){ return false; }

	D3D11_BLEND_DESC l_DeferredShadingBlendState;
	ZeroMemory(&l_DeferredShadingBlendState, sizeof(D3D11_BLEND_DESC));
	l_DeferredShadingBlendState.RenderTarget[0].BlendEnable = true;
	l_DeferredShadingBlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	l_DeferredShadingBlendState.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	l_DeferredShadingBlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	l_DeferredShadingBlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	l_DeferredShadingBlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	l_DeferredShadingBlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	l_DeferredShadingBlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	l_Result = m_D3DDevice->CreateBlendState(&l_DeferredShadingBlendState, &m_BlendStates[BLEND_DEFERRED_SHADING]);
	if (FAILED(l_Result)){ return false; }

	return true;
}

void CContextManager::Draw(CRenderableVertexs* _VerticesToRender, ERasterizedState _RS, EDepthStencilState _DSS, EBlendState _BS)
{
	m_DeviceContext->RSSetState(m_RasterizerSates[RS_SOLID]); //m_DeviceContext->RSSetState(m_RasterizerSates[_RS]);
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilStates[_DSS], 0);
	Vect4f v(1, 1, 1, 1);
	m_DeviceContext->OMSetBlendState(m_BlendStates[_BS], &v.x, 0xffffffff);

	s_DebugEffect->UpdateParameters(m_DeviceContext, &CEffectManager::m_SceneEffectParameters);
	s_DebugEffect->SetActive(m_DeviceContext);

	CRenderManager* l_RenderManager = CEngine::GetSingleton().GetRenderManager();
	CEffectTechnique* l_ETechnique = CEngine::GetSingleton().GetRenderableObjectTechniqueManager()->GetResource("MV_POSITION_COLOR_VERTEX")->GetEffectTechnique();
	_VerticesToRender->Render(l_RenderManager, l_ETechnique, (void *)&CEffectManager::m_SceneEffectParameters);
}

void CContextManager::DrawIndexed(CRenderableVertexs* _VerticesToRender, ERasterizedState _RS, EDepthStencilState _DSS, EBlendState _BS)
{
	m_DeviceContext->RSSetState(m_RasterizerSates[RS_SOLID]); //m_DeviceContext->RSSetState(m_RasterizerSates[_RS]);
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilStates[_DSS], 0);
	Vect4f v(1, 1, 1, 1);
	m_DeviceContext->OMSetBlendState(m_BlendStates[_BS], &v.x, 0xffffffff);

	s_DebugEffect->UpdateParameters(m_DeviceContext, &CEffectManager::m_SceneEffectParameters);
	s_DebugEffect->SetActive(m_DeviceContext);

	CRenderManager* l_RenderManager = CEngine::GetSingleton().GetRenderManager();
	CEffectTechnique* l_ETechnique = CEngine::GetSingleton().GetRenderableObjectTechniqueManager()->GetResource("MV_POSITION_COLOR_VERTEX")->GetEffectTechnique();

	_VerticesToRender->RenderIndexed(l_RenderManager, l_ETechnique, (void *)&CEffectManager::m_SceneEffectParameters);
}


void CContextManager::BeginRender(CColor backgroundColor)
{
	m_Viewport.Width = (FLOAT)m_Width;
	m_Viewport.Height = (FLOAT)m_Height;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &m_Viewport);

	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, &backgroundColor.x);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	//Set Render States
	m_DeviceContext->RSSetState(m_RasterizerSates[RS_SOLID]); //m_DeviceContext->RSSetState(m_RasterizerSates[_RS]);
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilStates[DSS_DEPTH_ON], 0);
	float v = 1;
	m_DeviceContext->OMSetBlendState(m_AlphaBlendState, &v, 0xffffffff);
}

void CContextManager::EndRender()
{
	m_SwapChain->Present(1, 0);
}

void CContextManager::Clear(bool RenderTarget, bool DepthStencil)
{
	float color[4];
	color[0] = 1.0;
	color[1] = 1.0;
	color[2] = 1.0;
	color[3] = 1.0;

	if (RenderTarget)
	{
		for (int i = 0; i < m_NumViews; ++i)
			m_DeviceContext->ClearRenderTargetView(m_CurrentRenderTargetViews[i], color);
	}

	if (DepthStencil)
	{
		assert(m_CurrentDepthStencilView);
		m_DeviceContext->ClearDepthStencilView(m_CurrentDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void CContextManager::Present()
{
	//First parameter: 0 = VSync not enabled.
	//First parameter: 1 = VSync enabled.
	if (m_VSyncEnabled)
		m_SwapChain->Present(1, 0);
	else m_SwapChain->Present(0, 0);
}

void CContextManager::SetMatrices(const CCamera& Camera)
{
	CEffectManager::m_SceneEffectParameters.m_View = Camera.GetView();
	CEffectManager::m_SceneEffectParameters.m_Projection = Camera.GetProjection();

	CEffectManager::m_SceneEffectParameters.m_InverseView = Camera.GetView();
	CEffectManager::m_SceneEffectParameters.m_InverseView.Invert();
	CEffectManager::m_SceneEffectParameters.m_InverseProjection = Camera.GetProjection();
	CEffectManager::m_SceneEffectParameters.m_InverseProjection.Invert();
	CEffectManager::m_SceneEffectParameters.m_CameraPosition = Camera.GetPosition();

	CEffectManager::m_SceneEffectParameters.m_CameraProjectionInfo = Vect4f(Camera.GetZNear(), Camera.GetZFar(), Camera.GetFOV(), Camera.GetAspectRatio());
}

void CContextManager::SetTimeParameters(float ElapsedTime)
{
	CEngine &l_Engine = CEngine::GetSingleton();
	float l_TimeSinceRun = l_Engine.GetRealTimeSinceStartup();
	CEffectManager::m_SceneEffectParameters.m_Times = Vect4f(ElapsedTime, l_TimeSinceRun, (float)m_Width, (float)m_Height);
}

D3D11_VIEWPORT CContextManager::GetViewPort()
{
	return m_Viewport;
}

void CContextManager::SetDefaultViewPort()
{
	D3D11_VIEWPORT l_Viewport;
	l_Viewport.Width = m_Viewport.Width;
	l_Viewport.Height = m_Viewport.Height;
	l_Viewport.MinDepth = 0.0f;
	l_Viewport.MaxDepth = 1.0f;
	l_Viewport.TopLeftX = 0.0f;
	l_Viewport.TopLeftY = 0.0f;
	m_DeviceContext->RSSetViewports(1, &l_Viewport);
}

void CContextManager::SetRenderTargets(int NumViews, ID3D11RenderTargetView *const*RenderTargetViews, ID3D11DepthStencilView *DepthStencilView)
{
	m_NumViews = NumViews;
	m_CurrentRenderTargetViews = RenderTargetViews;
	m_CurrentDepthStencilView = DepthStencilView;
	m_DeviceContext->OMSetRenderTargets(m_NumViews, RenderTargetViews, DepthStencilView);
}

void CContextManager::UnsetRenderTargets()
{
	SetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
}

void CContextManager::UnsetColorRenderTarget()
{
	SetRenderTargets(0, NULL, m_DepthStencilView);
}

void CContextManager::SetAlphaBlendState(ID3D11BlendState* AlphaBlendState)
{
	Vect4f l_BlendFactors(1, 1, 1, 1);
	m_DeviceContext->OMSetBlendState(AlphaBlendState, &l_BlendFactors.x, 0xffffffff);
}

void CContextManager::EnableDeferredShadingBlendState()
{
	m_DeviceContext->OMSetBlendState(m_BlendStates[BLEND_DEFERRED_SHADING], NULL, 0xffffffff);
}

void CContextManager::EnableAlphaBlendState()
{
	m_DeviceContext->OMSetBlendState(m_AlphaBlendState, NULL, 0xffffffff);
}

void CContextManager::EnableAdditiveAlphaBlendState()
{
	m_DeviceContext->OMSetBlendState(m_AdditiveAlphaBlendState, NULL, 0xffffffff);
}

void CContextManager::DisableAlphaBlendState()
{
	m_DeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
}

void CContextManager::SetRasterizerState(ERasterizedState RasterizerState)
{
	m_DeviceContext->RSSetState(m_RasterizerSates[RasterizerState]);
}

void CContextManager::SetDepthStencilState(EDepthStencilState DepthStencilState)
{
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilStates[DepthStencilState], 0);
}