#include "Textures\TextureManager.h"
#include "Utils.h"
#include "Engine\Engine.h"
#include "Log\Log.h"
#include <ScreenGrab.h>
#include "Render\RenderManager.h"
#include "Lights\LightManager.h"
#include "Textures\DynamicTexture.h"

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

bool CTextureManager::SaveToFile(std::string& Path, ID3D11Resource* Texture)
{
	ID3D11DeviceContext *l_DeviceContext = CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDeviceContext();
	std::wstring l_WStr(Path.begin(), Path.end());
	HRESULT l_Result = DirectX::SaveDDSTextureToFile(l_DeviceContext, Texture, l_WStr.c_str());
	if (FAILED(l_Result))
		return false;
	return true;
}

void CTextureManager::SaveBlackAndWhiteTexturesToFile()
{
	CLightManager* l_LManager = CEngine::GetSingleton().GetLightManager();
	for (size_t i = 0; i <l_LManager->GetResourcesVector().size(); ++i)
	{
		ID3D11Texture2D* l_TextureToSave = l_LManager->GetResourceById(i)->GetBlackAndWhiteMap()->GetRenderTargetTexture();
		ID3D11Texture2D* l_TextureToSave2 = l_LManager->GetResourceById(i)->GetShadowMap()->GetRenderTargetTexture();
		//ID3D11Texture2D* l_TextureToSave = ((CDynamicTexture*)CEngine::GetSingleton().GetTextureManager()->GetResource("DiffuseMapTexture"))->GetRenderTargetTexture();
		std::string l_Path = "./Data/Light"+i+std::string("_BlackAndWhiteTexture.dds");
		CEngine::GetSingleton().GetTextureManager()->SaveToFile(l_Path, l_TextureToSave);
		CEngine::GetSingleton().GetTextureManager()->SaveToFile("./Data/Light" + i + std::string("_ShadowMapTexture.dds"), l_TextureToSave2);
	}
}