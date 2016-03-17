#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "Engine.h"

CEffectTechnique::CEffectTechnique(CXMLTreeNode &TreeNode)
: CNamed("")
, m_VertexShader(NULL)
, m_PixelShader(NULL)
{
	m_Name=TreeNode.GetPszProperty("name", "");
	m_VertexShaderName=TreeNode.GetPszProperty("vertex_shader", "");
	m_PixelShaderName=TreeNode.GetPszProperty("pixel_shader", "");
	m_GeometryShaderName = TreeNode.GetPszProperty("geometry_shader", "");

	m_VertexShader = CEngine::GetSingleton().GetEffectManager()->GetVertexShader(m_VertexShaderName);
	m_PixelShader = CEngine::GetSingleton().GetEffectManager()->GetPixelShader(m_PixelShaderName);
	m_GeometryShader = CEngine::GetSingleton().GetEffectManager()->GetGeometryShader(m_GeometryShaderName);
	//Refresh();
}
	
CEffectTechnique::~CEffectTechnique()
{
	
}

void CEffectTechnique::Refresh()
{
	m_VertexShader->Load();
	m_PixelShader->Load();
	m_GeometryShader->Load();
	
	//m_VertexShader = CEngine::GetSingleton().GetEffectManager()->GetVertexShader(m_VertexShaderName);
	//m_PixelShader = CEngine::GetSingleton().GetEffectManager()->GetPixelShader(m_PixelShaderName);
}

void CEffectTechnique::SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer)
{
	m_VertexShader->SetConstantBuffer(IdBuffer,ConstantBuffer);
	m_PixelShader->SetConstantBuffer(IdBuffer,ConstantBuffer);
	if (m_GeometryShader != NULL)
		m_GeometryShader->SetConstantBuffer(IdBuffer, ConstantBuffer);
}