#include "Active.h"
#include "XML\XMLTreeNode.h"

CActive::CActive(CXMLTreeNode &TreeNode):m_Active(TreeNode.GetBoolProperty("active",true))
{

}

CActive::~CActive()
{

}

bool CActive::GetActive()
{
	return m_Active;
}

void CActive::SetActive(bool Value)
{
	m_Active=Value;
}


