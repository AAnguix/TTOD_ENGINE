#include "ResourceManager.h"
#include "Engine\Engine.h"
#include "resource.h"

CResourceManager::CResourceManager()
:m_CurrentCursor(0)
{
	
}
CResourceManager::~CResourceManager()
{
	m_Cursors.clear();
}

bool CResourceManager::Initialize()
{
	LoadGameResource(102);
	LoadGameResource(103);
	SetCurrentCursor(0);
	return true;
}

void CResourceManager::UpdateCursor()
{
	SetCursor(m_CurrentCursor);
}

void CResourceManager::SetCurrentCursor(unsigned int ID)
{
	if (ID <= m_Cursors.size())
		m_CurrentCursor = m_Cursors[ID];
}

bool CResourceManager::LoadGameResource(unsigned int ResourceID)
{
	HINSTANCE* l_Instance = CEngine::GetSingleton().GetHInstance();
	
	HCURSOR l_Cursor = HCURSOR(LoadImage(*l_Instance, MAKEINTRESOURCE(ResourceID), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE));
	m_CurrentCursor = l_Cursor;
	m_Cursors.push_back(l_Cursor);
	return false;
}