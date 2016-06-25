#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "Engine.h"
#include "XML\XMLTreeNode.h"

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
	/*std::string l_VertexShaderName = m_VertexShader->GetName();
	std::string l_PixelShaderName = m_PixelShader->GetName();
	std::string l_GeometryShaderName = m_GeometryShader->GetName();*/

	m_VertexShader->Reload();
	m_PixelShader->Reload();
	if (m_GeometryShader != nullptr)
		m_GeometryShader->Reload();

	/*m_VertexShader = CEngine::GetSingleton().GetEffectManager()->GetVertexShader(m_VertexShaderName);
	m_PixelShader = CEngine::GetSingleton().GetEffectManager()->GetPixelShader(m_PixelShaderName);
	m_GeometryShader = CEngine::GetSingleton().GetEffectManager()->GetGeometryShader(m_GeometryShaderName);*/
}

void CEffectTechnique::SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer)
{
	m_VertexShader->SetConstantBuffer(IdBuffer,ConstantBuffer);
	m_PixelShader->SetConstantBuffer(IdBuffer,ConstantBuffer);
	if (m_GeometryShader != nullptr)
		m_GeometryShader->SetConstantBuffer(IdBuffer, ConstantBuffer);
}