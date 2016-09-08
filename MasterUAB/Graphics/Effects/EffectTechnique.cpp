#include "Effects\EffectTechnique.h"
#include "XML\XMLTreeNode.h"

#include "Effects\EffectManager.h"
#include "Engine\Engine.h"

CEffectTechnique::CEffectTechnique(CXMLTreeNode &TreeNode)
:CNamed(TreeNode)
,m_VertexShaderName(TreeNode.GetPszProperty("vertex_shader", ""))
,m_PixelShaderName(TreeNode.GetPszProperty("pixel_shader", ""))
,m_GeometryShaderName(TreeNode.GetPszProperty("geometry_shader", ""))
{
	m_VertexShader = CEngine::GetSingleton().GetEffectManager()->GetVertexShader(m_VertexShaderName);
	m_PixelShader = CEngine::GetSingleton().GetEffectManager()->GetPixelShader(m_PixelShaderName);
	m_GeometryShader = CEngine::GetSingleton().GetEffectManager()->GetGeometryShader(m_GeometryShaderName);
}
	
CEffectTechnique::~CEffectTechnique()
{
}

void CEffectTechnique::Refresh()
{
	m_VertexShader->Reload();
	m_PixelShader->Reload();
	if (m_GeometryShader != nullptr)
		m_GeometryShader->Reload();
}

void CEffectTechnique::SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer)
{
	m_VertexShader->SetConstantBuffer(IdBuffer,ConstantBuffer);
	m_PixelShader->SetConstantBuffer(IdBuffer,ConstantBuffer);
	if (m_GeometryShader != nullptr)
		m_GeometryShader->SetConstantBuffer(IdBuffer, ConstantBuffer);
}