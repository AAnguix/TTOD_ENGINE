#include "Effects\EffectPixelShader.h"
#include "Render\RenderManager.h"
#include "Engine\Engine.h"
#include "Log\Log.h"
#include "XML\XMLTreeNode.h"

void CEffectGeometryShader::Destroy()
{
	if (m_GeometryShader)
	{
		m_GeometryShader->Release();
		m_GeometryShader = 0;
	}
}

CEffectGeometryShader::~CEffectGeometryShader()
{
	Destroy();
}

CEffectGeometryShader::CEffectGeometryShader(const CXMLTreeNode &TreeNode)
: CEffectShader(TreeNode)
, m_GeometryShader(NULL)
{
	m_ShaderModel = TreeNode.GetPszProperty("shader_model", "gs_4_0");
	m_EntryPoint = TreeNode.GetPszProperty("entry_point", "GS");
}

bool CEffectGeometryShader::Load()
{
	CreateShaderMacro();

	ID3DBlob *l_GSBlob = NULL;
	bool l_Loaded = LoadShaderExtended(m_Name, m_Filename.c_str(), m_EntryPoint.c_str(), m_ShaderModel.c_str(), &l_GSBlob);

	if (!l_Loaded)
	{
		CEngine::GetSingleton().GetLogManager()->Log("Unable to load geometry shader " + m_Filename);
		return false;
	}
	CRenderManager *l_RenderManager = CEngine::GetSingleton().GetRenderManager();
	ID3D11Device *l_Device = l_RenderManager->GetContextManager()->GetDevice();
	HRESULT l_HR = l_Device->CreateGeometryShader(l_GSBlob->GetBufferPointer(), l_GSBlob->GetBufferSize(), NULL, &m_GeometryShader);
	l_GSBlob->Release();
	return CreateConstantBuffer();
}

void CEffectGeometryShader::SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer)
{
	ID3D11DeviceContext *l_DeviceContext = CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDeviceContext();
	ID3D11Buffer *l_ConstantBuffer = GetConstantBuffer(IdBuffer);

	if (l_ConstantBuffer != NULL)
	{
		l_DeviceContext->UpdateSubresource(l_ConstantBuffer, 0, NULL, ConstantBuffer, 0, 0);
		l_DeviceContext->GSSetConstantBuffers(IdBuffer, 1, &l_ConstantBuffer);
	}
}
