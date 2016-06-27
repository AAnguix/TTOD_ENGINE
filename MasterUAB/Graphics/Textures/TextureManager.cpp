#include "Textures\TextureManager.h"
#include "Utils.h"
#include "Engine\Engine.h"
#include "Log\Log.h"

CTextureManager::CTextureManager(){}

CTextureManager::~CTextureManager(){}

CTexture * CTextureManager::GetTexture(const std::string &Filename, bool GuiTexture)
{
	bool l_WrongFormat = false;

	std::string l_Extension = Filename.substr(Filename.find_last_of(".") + 1);
	if (l_Extension == ".jpg" || l_Extension == ".tga")
		l_WrongFormat = true;

	if (l_WrongFormat)
	{
		assert(false);
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
		bool l_Loaded = l_Texture->Load(Filename, GuiTexture);
		if (l_Loaded)
		{
			if (!AddResource(Filename, l_Texture))
			{	//Falla al añadirse
				CHECKED_DELETE(l_Texture);
			}
		}
		else
		{
			CEngine::GetSingleton().GetLogManager()->Log("Can't load texture " + Filename);
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