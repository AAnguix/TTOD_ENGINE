#ifndef _STAGEDTEXTUREDSCENERENDERERCOMMAND_H
#define _STAGEDTEXTUREDSCENERENDERERCOMMAND_H

#include "SceneRendererCommand.h"
#include <vector>
#include <D3D11.h>
#include "Textures\Texture.h"
#include "Materials\MaterialManager.h"
#include "Textures\DynamicTexture.h"
#include "Engine\Engine.h"
//class CDynamicTexture;

class CStagedTexturedSceneRendererCommand : public CSceneRendererCommand 
{ 

protected:  
	class CStageTexture  
	{  
		public:   int m_StageId;   
		CTexture *m_Texture;  
		CStageTexture(int StageId, CTexture *Texture)   
		{    
			m_StageId=StageId;    
			m_Texture=Texture;   
		}   
		void Activate()
		{
			if(m_Texture!=NULL)
				m_Texture->Activate(m_StageId);
		}
	};

	class CDynamicTextureAndMaterial
	{  
		private:      
			CDynamicTexture *m_DynamicTexture;  
			CMaterial* m_Material;

		public:
			CDynamicTextureAndMaterial(CDynamicTexture* DynamicTexture, CMaterial* Material)
			{
				m_DynamicTexture = DynamicTexture;
				m_Material = Material;
			}
			CDynamicTexture* GetDynamicTexture() const{return m_DynamicTexture;};
			CMaterial* GetMaterial() const{return m_Material;};
       
			~CDynamicTextureAndMaterial(){}
	};
		
	std::vector<CStageTexture>    m_StageTextures;  
	std::vector<CDynamicTextureAndMaterial *>   m_DynamicTextures;
	std::vector<ID3D11RenderTargetView *>   m_RenderTargetViews;

public:  
	CStagedTexturedSceneRendererCommand(CXMLTreeNode &TreeNode);  
	virtual ~CStagedTexturedSceneRendererCommand();
		
	void CreateRenderTargetViewVector();  
	void ActivateTextures();  
	void AddStageTexture(int StageId, CTexture *Texture);  
	virtual void Execute(CRenderManager &RenderManager) = 0; 
}; 

#endif
	
