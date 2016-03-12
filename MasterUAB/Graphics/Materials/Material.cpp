#include "Textures\Texture.h"
#include "Textures\TextureManager.h"
#include "XML\XMLTreeNode.h"
#include "Material.h"
#include "Engine.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Materials\TemplatedMaterialParameter.h"
#include "Effects\EffectManager.h"

CMaterial::CMaterial(CXMLTreeNode &TreeNode) :
m_Textures(NULL),
m_RenderableObjectTechnique(NULL),
CNamed(TreeNode.GetPszProperty("name", "defaultMaterial")),
m_CurrentParameterData(0),
m_Offset(0)
{
	std::string l_RenderableObjectTechniqueName=TreeNode.GetPszProperty("renderable_object_technique", "");

	if(l_RenderableObjectTechniqueName!="")
		m_RenderableObjectTechnique=CEngine::GetSingleton().GetRenderableObjectTechniqueManager()->GetResource(l_RenderableObjectTechniqueName);	

	for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
	{
		CXMLTreeNode l_Element = TreeNode(i);

		if (l_Element.GetName() == std::string("texture"))
		{
			std::string l_Filename = l_Element.GetPszProperty("filename", "");
			
			CTexture *l_Texture = CEngine::GetSingleton().GetTextureManager()->GetTexture(l_Filename);
			if (l_Texture!=nullptr)
				m_Textures.push_back(l_Texture);
		}
		else if (l_Element.GetName() == std::string("parameter"))
		{
			std::string l_Type = l_Element.GetPszProperty("type");
			std::string l_Name = l_Element.GetPszProperty("name");
			CMaterialParameter* l_MaterialParameter;
	
			CMaterialParameter::TMaterialType l_MaterialType;

			if(l_Type=="float")
			{
				l_MaterialType = CMaterialParameter::FLOAT;
				l_MaterialParameter = new CTemplatedMaterialParameter<float>(this,l_Element,l_Element.GetFloatProperty("value",0.0f),l_MaterialType);
			
			}else if(l_Type=="vect2f")
			{
				l_MaterialType = CMaterialParameter::VECT2F;
				l_MaterialParameter = new CTemplatedMaterialParameter<Vect2f>(this,l_Element,l_Element.GetVect2fProperty("value",v2fZERO),l_MaterialType);
			}
			else if(l_Type=="vect3f")
			{
				l_MaterialType = CMaterialParameter::VECT3F;
				l_MaterialParameter = new CTemplatedMaterialParameter<Vect3f>(this,l_Element,l_Element.GetVect3fProperty("value",v3fZERO),l_MaterialType);
			}
			else if(l_Type=="vect4f")
			{
				l_MaterialType = CMaterialParameter::VECT4F;
				l_MaterialParameter = new CTemplatedMaterialParameter<Vect4f>(this,l_Element,l_Element.GetVect4fProperty("value",v4fZERO),l_MaterialType);
			}

			m_Parameters.push_back(l_MaterialParameter);
		}
	}
}

CMaterial::~CMaterial()
{
	for(size_t i=0;i<m_Parameters.size();++i)
	{
		CHECKED_DELETE(m_Parameters[i]);
	}
	Destroy();
}

void CMaterial::Apply(CRenderableObjectTechnique* RenderableObjectTechnique)
{
	for(size_t i = 0;i < m_Textures.size();++i)
	{
		m_Textures[i]->Activate(i);
	}

	for(size_t i = 0;i < m_Parameters.size();++i)
	{
		//CEffectManager::m_MaterialEffectParameters.m_RawData[i]=GetNextParameterAddress(sizeof(float));
		m_Parameters[i]->Apply();
	}
	CRenderableObjectTechnique *l_RenderableObjectTechnique=RenderableObjectTechnique;
	if(l_RenderableObjectTechnique==NULL)
		l_RenderableObjectTechnique=m_RenderableObjectTechnique;

	if(l_RenderableObjectTechnique != NULL)
		l_RenderableObjectTechnique->GetEffectTechnique()->SetConstantBuffer(3,&CEffectManager::m_MaterialEffectParameters);
}

void CMaterial::Destroy()
{	
	m_Textures.clear();
	m_Parameters.clear();
}

void * CMaterial::GetNextParameterAddress(unsigned int NumBytes)
{
	size_t l_Values = NumBytes/4;

	void* l_Return;
	if(m_Offset==0)
		l_Return = (void *)&CEngine::GetSingleton().GetEffectManager()->m_MaterialEffectParameters.m_RawData[m_CurrentParameterData].x;
	else if(m_Offset==1)
		l_Return = (void *)&CEngine::GetSingleton().GetEffectManager()->m_MaterialEffectParameters.m_RawData[m_CurrentParameterData].y;
	else if(m_Offset==2)
		l_Return = (void *)&CEngine::GetSingleton().GetEffectManager()->m_MaterialEffectParameters.m_RawData[m_CurrentParameterData].z;
	else if(m_Offset==3)
		l_Return = (void *)&CEngine::GetSingleton().GetEffectManager()->m_MaterialEffectParameters.m_RawData[m_CurrentParameterData].w;

	m_Offset+=l_Values;

	if(m_Offset>3)
	{
		m_Offset=m_Offset-4;
		m_CurrentParameterData+=1;
	}
	return l_Return;
}