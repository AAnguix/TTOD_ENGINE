#include "Effects\EffectPixelShader.h"
#include "Render\RenderManager.h"
#include "Engine.h"
#include "Log.h"

void CEffectPixelShader::Destroy()
{
	if (m_PixelShader)
	{
		m_PixelShader->Release();
		m_PixelShader = 0;
	}
}
	
CEffectPixelShader::~CEffectPixelShader()
{
	Destroy();
}

CEffectPixelShader::CEffectPixelShader(const CXMLTreeNode &TreeNode)
: CEffectShader(TreeNode)
, m_PixelShader(NULL)
{
	
	m_ShaderModel = TreeNode.GetPszProperty("shader_model","ps_4_0");
	m_EntryPoint = TreeNode.GetPszProperty("entry_point","PS");
}

bool CEffectPixelShader::Load()
{
	CreateShaderMacro(); 

	ID3DBlob *l_PSBlob=NULL;  
	bool l_Loaded=LoadShader(m_Filename.c_str(), m_EntryPoint.c_str(), m_ShaderModel.c_str(), &l_PSBlob);
	
	if(!l_Loaded)  
	{
		CEngine::GetSingleton().GetLogManager()->Log("Unable to load pixel shader "+m_Filename);
		return false;  
	}
	CRenderManager *l_RenderManager=CEngine::GetSingleton().GetRenderManager();  
	ID3D11Device *l_Device=l_RenderManager->GetContextManager()->GetDevice();
	HRESULT l_HR=l_Device->CreatePixelShader(l_PSBlob->GetBufferPointer(), l_PSBlob->GetBufferSize(), NULL, &m_PixelShader);
	l_PSBlob->Release();  
	return CreateConstantBuffer();
}

void CEffectPixelShader::SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer)
{ 
	ID3D11DeviceContext *l_DeviceContext=CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDeviceContext();  
	ID3D11Buffer *l_ConstantBuffer=GetConstantBuffer(IdBuffer);

	if(l_ConstantBuffer!=NULL)
	{
		l_DeviceContext->UpdateSubresource(l_ConstantBuffer, 0, NULL, ConstantBuffer, 0, 0);  
		l_DeviceContext->PSSetConstantBuffers(IdBuffer, 1, &l_ConstantBuffer); 
	}
}
