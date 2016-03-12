#include "Textures\TextureManager.h"
#include "Utils.h"

CTextureManager::CTextureManager(){}

CTextureManager::~CTextureManager(){}

CTexture * CTextureManager::GetTexture(const std::string &Filename)
{
	bool l_WrongFormat = false;

	if (Filename.substr(Filename.find_last_of(".") + 1) == "jpg") 
	{
		l_WrongFormat = true;
	}

	//assert(!l_WrongFormat);

	if (l_WrongFormat)
	{
		return nullptr;
	}

	std::map<std::string, CTexture*>::iterator itMap;

	itMap=m_Resources.find(Filename);

	if(itMap != m_Resources.end())
	{
		return itMap->second;
	}
	else 
	{
		CTexture* l_Texture = new CTexture;
		l_Texture->Load(Filename);
		
		if(!AddResource(Filename, l_Texture))
		{	//Falla al añadirse
			CHECKED_DELETE(l_Texture);
		}
		 
		return l_Texture;
	}
}
 
void CTextureManager::Reload()
{
	std::map<std::string, CTexture*>::iterator itMap;
			
	for ( itMap = m_Resources.begin(); itMap != m_Resources.end(); itMap++)
	{
		itMap->second->Reload();
	}
}