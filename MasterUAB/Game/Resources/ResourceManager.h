#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include <string>
#include <vector>
#include <Windows.h>

class CResourceManager 
{

private:
	std::string m_Filename;
	std::vector<HCURSOR> m_Cursors;
	HCURSOR m_CurrentCursor;

	bool LoadGameResource(unsigned int ResourceID);
	
public:
	CResourceManager();
	virtual ~CResourceManager();
	bool Initialize();

	void UpdateCursor();
	void SetCurrentCursor(unsigned int ID);
	
};

#endif