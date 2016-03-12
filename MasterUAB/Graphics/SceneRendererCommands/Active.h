#ifndef _ACTIVE_H
#define _ACTIVE_H

class CXMLTreeNode;

class CActive
{

protected:
	bool  m_Active; 
  
public:
	CActive(CXMLTreeNode &TreeNode);
	virtual ~CActive();
	bool GetActive();
	void SetActive(bool Value);
};

#endif
	




