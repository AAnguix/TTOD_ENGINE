#include "Materials\MaterialParameter.h"
#include "XML\XMLTreeNode.h"

CMaterialParameter::CMaterialParameter(CMaterial *Material, CXMLTreeNode &TreeNode, CMaterialParameter::TMaterialType MaterialType) : CNamed(TreeNode.GetPszProperty("name"))
{
	m_MaterialType=MaterialType;
}

CMaterialParameter::~CMaterialParameter()
{

}


