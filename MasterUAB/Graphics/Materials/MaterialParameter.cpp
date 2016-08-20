#include "Materials\MaterialParameter.h"
#include "XML\XMLTreeNode.h"

CMaterialParameter::CMaterialParameter(CMaterial *Material, CXMLTreeNode &TreeNode, CMaterialParameter::TMaterialType MaterialType) : CNamed(TreeNode.GetPszProperty("name"))
{
	m_MaterialType=MaterialType;
}

CMaterialParameter::CMaterialParameter(CMaterial *Material, const std::string &Name, CMaterialParameter::TMaterialType MaterialType) : CNamed(Name)
{
	m_MaterialType = MaterialType;
}

CMaterialParameter::~CMaterialParameter()
{

}


