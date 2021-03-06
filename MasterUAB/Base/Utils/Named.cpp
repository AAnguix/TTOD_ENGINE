#include "Named.h"
#include "Utils\EmptyPointerClass.h"
#include "XML\XMLTreeNode.h"

CNamed::CNamed(const CXMLTreeNode &TreeNode)
:m_Name(TreeNode.GetPszProperty("name", ""))
{ 
}

CNamed::CNamed(const std::string &Name)
:m_Name(Name)
{
}

void CNamed::SetName(const std::string &Name)
{
	m_Name=Name;
}

const std::string &CNamed::GetName() const
{
	return m_Name;
}

CEmptyPointerClass* CNamed::GetNameLuaAddress() const { return (CEmptyPointerClass *)((void*)&m_Name); }