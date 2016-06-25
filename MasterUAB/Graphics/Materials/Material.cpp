#include "Material.h"
#include "Textures\TextureManager.h"
#include "XML\XMLTreeNode.h"
#include "Engine.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Materials\TemplatedMaterialParameter.h"
#include "Effects\EffectManager.h"
#include "PhysXManager.h"

CMaterial::CMaterial(CXMLTreeNode &TreeNode)
:m_Textures(NULL)
,m_RenderableObjectTechnique(NULL)
,CNamed(TreeNode.GetPszProperty("name", ""))
,m_CurrentParameterData(0)
,m_Offset(0)
,m_StaticFriction(0.0f)
,m_DynamicFriction(0.0f)
,m_Restitution(0.0f)
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
			bool l_GuiTexture = l_Element.GetBoolProperty("gui_texture", false);
			
			CTexture *l_Texture = CEngine::GetSingleton().GetTextureManager()->GetTexture(l_Filename, l_GuiTexture);
			if (l_Texture!=nullptr)
				m_Textures.push_back(l_Texture);
		}
		else if (l_Element.GetName() == std::string("parameter"))
		{
			std::string l_Type = l_Element.GetPszProperty("type");
			std::string l_Name = l_Element.GetPszProperty("name");

			if (l_Name == "static_friction")
			{
				m_StaticFriction = l_Element.GetFloatProperty("value", 0.0f);
			}
			else if (l_Name == "dynamic_friction")
			{
				m_DynamicFriction = l_Element.GetFloatProperty("value", 0.0f);
			}
			else if (l_Name == "restitution")
			{
				m_Restitution = l_Element.GetFloatProperty("value", 0.0f);
			}
			else
			{
				CMaterialParameter* l_MaterialParameter;
				CMaterialParameter::TMaterialType l_MaterialType;

				if (l_Type == "float")
				{
					l_MaterialType = CMaterialParameter::FLOAT;
					l_MaterialParameter = new CTemplatedMaterialParameter<float>(this, l_Element, l_Element.GetFloatProperty("value", 0.0f), l_MaterialType);

				}
				else if (l_Type == "vect2f")
				{
					l_MaterialType = CMaterialParameter::VECT2F;
					l_MaterialParameter = new CTemplatedMaterialParameter<Vect2f>(this, l_Element, l_Element.GetVect2fProperty("value", v2fZERO), l_MaterialType);
				}
				else if (l_Type == "vect3f")
				{
					l_MaterialType = CMaterialParameter::VECT3F;
					l_MaterialParameter = new CTemplatedMaterialParameter<Vect3f>(this, l_Element, l_Element.GetVect3fProperty("value", v3fZERO), l_MaterialType);
				}
				else if (l_Type == "vect4f")
				{
					l_MaterialType = CMaterialParameter::VECT4F;
					l_MaterialParameter = new CTemplatedMaterialParameter<Vect4f>(this, l_Element, l_Element.GetVect4fProperty("value", v4fZERO), l_MaterialType);
				}

				m_Parameters.push_back(l_MaterialParameter);
			}
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

void CMaterial::ChangeTexture(const std::string Texture, size_t Index)
{ 
	bool l_UsedInGui = m_Textures[Index]->UsedInGui();
	m_Textures[Index] = CEngine::GetSingleton().GetTextureManager()->GetTexture(Texture, l_UsedInGui);
}

CEmptyPointerClass* CMaterial::GetThisLuaAddress() const { return (CEmptyPointerClass *)((void*)this); }
CEmptyPointerClass* CMaterial::GetStaticFrictionLuaAddress() const { return (CEmptyPointerClass *)&m_StaticFriction; }
CEmptyPointerClass*CMaterial::GetDynamicFrictionLuaAddress() const { return (CEmptyPointerClass *)&m_DynamicFriction; }
CEmptyPointerClass*CMaterial::GetRestitutionLuaAddress() const { return (CEmptyPointerClass *)&m_Restitution; }
void CMaterial::ApplyLuaPhysxProperties()
{
	CEngine::GetSingleton().GetPhysXManager()->SetMaterialStaticFriction(m_Name,m_StaticFriction);
	CEngine::GetSingleton().GetPhysXManager()->SetMaterialDynamicFriction(m_Name, m_DynamicFriction);
	CEngine::GetSingleton().GetPhysXManager()->SetMaterialRestitution(m_Name, m_Restitution);
}