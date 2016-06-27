#include "PoolRenderableObjectTechnique.h"
#include "XML\XMLTreeNode.h"
#include "Engine\Engine.h"
#include "Effects\EffectManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique(CXMLTreeNode &TreeNode) : CNamed(TreeNode.GetPszProperty("name",""))
{
}  

CPoolRenderableObjectTechnique::~CPoolRenderableObjectTechnique()
{
	Destroy();
}

void CPoolRenderableObjectTechnique::Destroy()
{
	for(size_t i=0;i<m_RenderableObjectTechniqueElements.size();++i)
	{
		delete m_RenderableObjectTechniqueElements[i];
		m_RenderableObjectTechniqueElements[i] = nullptr;
	}
}

//CPoolRenderableObjectTechnique::CPoolRenderableObjectTechniqueElement::CPoolRenderableObjectTechniqueElement(const std::string &Name, CEffectTechnique *EffectTechnique, CRenderableObjectTechnique *OnRenderableObjectTechniqueManager)
//{

//}

void CPoolRenderableObjectTechnique::AddElement(const std::string &Name, const std::string &TechniqueName, CRenderableObjectTechnique *ROTOnRenderableObjectTechniqueManager)
{
	CPoolRenderableObjectTechniqueElement* l_Element = new CPoolRenderableObjectTechniqueElement(
	Name,
	CEngine::GetSingleton().GetEffectManager()->GetResource(TechniqueName),
	ROTOnRenderableObjectTechniqueManager);

	if(l_Element)
	{
		m_RenderableObjectTechniqueElements.push_back(l_Element);
	} 
}

void CPoolRenderableObjectTechnique::Apply()
{
	for(size_t i=0;i<m_RenderableObjectTechniqueElements.size();++i)
	{
		m_RenderableObjectTechniqueElements[i]->m_OnRenderableObjectTechniqueManager->SetEffectTechnique(
			m_RenderableObjectTechniqueElements[i]->m_RenderableObjectTechnique.GetEffectTechnique()
		);
	}
}