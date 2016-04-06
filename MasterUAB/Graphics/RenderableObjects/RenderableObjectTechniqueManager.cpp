#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "XML\XMLTreeNode.h"
#include "Engine.h"
#include "Effects\EffectManager.h"

CRenderableObjectTechniqueManager::CRenderableObjectTechniqueManager()
{

}
CRenderableObjectTechniqueManager::~CRenderableObjectTechniqueManager()
{

}  

bool CRenderableObjectTechniqueManager::InsertRenderableObjectTechnique(CPoolRenderableObjectTechnique *PoolRenderableObjectTechniques, 
		const std::string &RenderableObjectTechniqueName, const std::string &TechniqueName)
{
	CRenderableObjectTechnique *l_ROTOnRenderableObjectTechniqueManager=GetResource(RenderableObjectTechniqueName);
	if(l_ROTOnRenderableObjectTechniqueManager==NULL)
	{
		l_ROTOnRenderableObjectTechniqueManager=new CRenderableObjectTechnique(RenderableObjectTechniqueName, CEngine::GetSingleton().GetEffectManager()->GetResource(TechniqueName));
		AddResource(l_ROTOnRenderableObjectTechniqueManager->GetName(), l_ROTOnRenderableObjectTechniqueManager);
	}
	PoolRenderableObjectTechniques->AddElement(RenderableObjectTechniqueName, TechniqueName, l_ROTOnRenderableObjectTechniqueManager);
	return true;
}

/*
void CRenderableObjectTechniqueManager::Destroy()
{

}*/ 

void CRenderableObjectTechniqueManager::Load(const std::string &Filename)
{
	m_Filename = Filename;

	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(Filename.c_str()))
	{
		CXMLTreeNode l_ROTechniques = l_XML["renderable_object_techniques"];

		if (l_ROTechniques.Exists())
		{
			for (int i = 0; i < l_ROTechniques.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_ROTechniques(i);

				if (l_Element.GetName() == std::string("pool_renderable_object_technique"))
				{
					CPoolRenderableObjectTechnique* l_PoolROTechnique = new CPoolRenderableObjectTechnique(l_Element);
					//Crea Pool vacía

					if (!m_PoolRenderableObjectTechniques.AddResource(l_PoolROTechnique->GetName(), l_PoolROTechnique))
					{
						CHECKED_DELETE(l_PoolROTechnique);
					}
					else //Rellena con las techniques
					{
						for (int i = 0; i < l_Element.GetNumChildren(); ++i)
						{
							CXMLTreeNode l_ROT = l_Element(i);

							if (l_ROT.GetName() == std::string("default_technique"))
							{
								std::string l_VertexType = l_ROT.GetPszProperty("vertex_type", "");
								std::string l_Name = l_VertexType;
								std::string l_Technique = l_ROT.GetPszProperty("technique", "");
								InsertRenderableObjectTechnique(l_PoolROTechnique, l_Name, l_Technique);
							}
							else if (l_ROT.GetName() == std::string("renderable_object_technique"))
							{
								std::string l_Name = l_ROT.GetPszProperty("name", "");
								std::string l_Technique = l_ROT.GetPszProperty("technique", "");
								InsertRenderableObjectTechnique(l_PoolROTechnique, l_Name, l_Technique);
							}
						}
					}
				}
			}
		}
		else { assert(false); }
	}
	else { assert(false); }
}

void CRenderableObjectTechniqueManager::Reload()
{
	m_PoolRenderableObjectTechniques.Destroy();
	Destroy();
	Load(m_Filename);
} 

CTemplatedMapManager<CPoolRenderableObjectTechnique> & CRenderableObjectTechniqueManager::GetPoolRenderableObjectTechniques()
{
	return m_PoolRenderableObjectTechniques;
} 