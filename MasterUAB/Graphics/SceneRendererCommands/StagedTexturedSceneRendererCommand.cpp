#include "StagedTexturedSceneRendererCommand.h"
#include "Textures\TextureManager.h"
#include "Render\RenderManager.h"
#include "CaptureFrameBufferTexture.h"
#include "CaptureStencilBufferTexture.h"
#include "XML\XMLTreeNode.h"
#include "Render\ContextManager.h"

CStagedTexturedSceneRendererCommand::CStagedTexturedSceneRendererCommand(CXMLTreeNode &TreeNode): CSceneRendererCommand(TreeNode)
{
	for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
	{
		CXMLTreeNode l_Node = TreeNode(i);

		if (l_Node.GetName() == std::string("texture"))
		{
			int l_StageID = l_Node.GetIntProperty("stage_id");
			std::string l_File = l_Node.GetPszProperty("file");
			bool l_Load = l_Node.GetBoolProperty("load_file",true);
			bool l_GuiTexture = l_Node.GetBoolProperty("gui_texture", false);
			
			CTexture* l_Texture = NULL;
			if(l_Load)
				l_Texture = CEngine::GetSingleton().GetTextureManager()->GetTexture(l_File,l_GuiTexture);
			else
				l_Texture = CEngine::GetSingleton().GetTextureManager()->GetResource(l_File);

			CStageTexture l_StagedTexture(l_StageID,l_Texture);
			m_StageTextures.push_back(l_StagedTexture);

		} 
		else if (l_Node.GetName() == std::string("dynamic_texture"))
		{
			std::string l_Name = l_Node.GetPszProperty("name");
			std::string l_MaterialName = l_Node.GetPszProperty("material", "");
			std::string l_Type = l_Node.GetPszProperty("format_type","");

			CMaterial* l_Material = CEngine::GetSingleton().GetMaterialManager()->GetResource(l_MaterialName);
			CDynamicTexture* l_DynamicTexture = (CDynamicTexture*)CEngine::GetSingleton().GetTextureManager()->GetResource(l_Name); 
			CDynamicTextureAndMaterial *l_DynamicAndMaterial; 
			
			if(l_DynamicTexture==NULL)
			{
				bool l_AsFrameBuffer=l_Node.GetBoolProperty("texture_width_as_frame_buffer",true);
				if(l_AsFrameBuffer)
				{
					bool l_CreateDepthScentilBuffer=l_Node.GetBoolProperty("create_depth_stencil_buffer");
					CContextManager* l_C = CEngine::GetSingleton().GetRenderManager()->GetContextManager();
					l_DynamicTexture = new CDynamicTexture(l_Name,l_C->GetFrameBufferWidth(),l_C->GetFrameBufferHeight(),l_CreateDepthScentilBuffer,CDynamicTexture::GetFormatTypeFromString(l_Type));
				}
				else l_DynamicTexture = new CDynamicTexture(TreeNode);

				if(CEngine::GetSingleton().GetTextureManager()->AddResource(l_Name,l_DynamicTexture))
				{
					l_DynamicAndMaterial = new CDynamicTextureAndMaterial(l_DynamicTexture,l_Material);
					m_DynamicTextures.push_back(l_DynamicAndMaterial);
				}

			} else 
			{
				l_DynamicAndMaterial = new CDynamicTextureAndMaterial(l_DynamicTexture,l_Material);
				m_DynamicTextures.push_back(l_DynamicAndMaterial);
			}	
		}
		else if (l_Node.GetName() == std::string("capture_texture"))
		{
			std::string l_Name = l_Node.GetPszProperty("name");
			CCaptureFrameBufferTexture* l_CaptureFrameBufferTexture = (CCaptureFrameBufferTexture*)CEngine::GetSingleton().GetTextureManager()->GetResource(l_Name); 

			if(l_CaptureFrameBufferTexture==NULL)
			{
				bool l_AsFrameBuffer = l_Node.GetBoolProperty("texture_width_as_frame_buffer",true);
				if(l_AsFrameBuffer)
				{
					CContextManager* l_C = CEngine::GetSingleton().GetRenderManager()->GetContextManager();
					l_CaptureFrameBufferTexture = new CCaptureFrameBufferTexture(l_Name,l_C->GetFrameBufferWidth(),l_C->GetFrameBufferHeight());
				}
				else
				{
					l_CaptureFrameBufferTexture = new CCaptureFrameBufferTexture(l_Node);
				}

				if(CEngine::GetSingleton().GetTextureManager()->AddResource(l_Name,l_CaptureFrameBufferTexture))
				{
					CStageTexture l_StagedTexture(0,l_CaptureFrameBufferTexture);
					m_StageTextures.push_back(l_StagedTexture);
				}

			} else {CStageTexture l_StagedTexture(0,l_CaptureFrameBufferTexture); m_StageTextures.push_back(l_StagedTexture);}
			
		}
		else if (l_Node.GetName() == std::string("capture_stencil_texture"))
		{
			std::string l_Name = l_Node.GetPszProperty("name");
			CCaptureStencilBufferTexture* l_CaptureStencilBufferTexture = (CCaptureStencilBufferTexture*)CEngine::GetSingleton().GetTextureManager()->GetResource(l_Name);

			if (l_CaptureStencilBufferTexture == NULL)
			{
				bool l_AsFrameBuffer = l_Node.GetBoolProperty("texture_width_as_frame_buffer", true);
				if (l_AsFrameBuffer)
				{
					CContextManager* l_C = CEngine::GetSingleton().GetRenderManager()->GetContextManager();
					l_CaptureStencilBufferTexture = new CCaptureStencilBufferTexture(l_Name, l_C->GetFrameBufferWidth(), l_C->GetFrameBufferHeight());
				}
				else
				{
					l_CaptureStencilBufferTexture = new CCaptureStencilBufferTexture(l_Node);
				}

				if (CEngine::GetSingleton().GetTextureManager()->AddResource(l_Name, l_CaptureStencilBufferTexture))
				{
					CStageTexture l_StagedTexture(0, l_CaptureStencilBufferTexture);
					m_StageTextures.push_back(l_StagedTexture);
				}

			}
			else { CStageTexture l_StagedTexture(0, l_CaptureStencilBufferTexture); m_StageTextures.push_back(l_StagedTexture); }

		}

	}

	CreateRenderTargetViewVector();
}

CStagedTexturedSceneRendererCommand::~CStagedTexturedSceneRendererCommand()
{
	
	for(size_t i=0;i<m_DynamicTextures.size();++i)
	{
		if(m_DynamicTextures[i])
		{
			delete m_DynamicTextures[i];
			m_DynamicTextures[i] = nullptr;
		}
	}
	m_StageTextures.clear();
	m_DynamicTextures.clear();
	m_RenderTargetViews.clear();
	
}
		
void CStagedTexturedSceneRendererCommand::CreateRenderTargetViewVector()
{
	for(size_t i=0;i<m_DynamicTextures.size();++i)
	{
		m_RenderTargetViews.push_back(m_DynamicTextures[i]->GetDynamicTexture()->GetRenderTargetView());
	}
}

void CStagedTexturedSceneRendererCommand::ActivateTextures() 
{
	for(size_t i=0;i<m_StageTextures.size();++i)
	{
		m_StageTextures[i].Activate(); 
	}
}

void CStagedTexturedSceneRendererCommand::AddStageTexture(int StageId, CTexture *Texture) 
{
	m_StageTextures.push_back(CStageTexture(StageId,Texture));
}

void CStagedTexturedSceneRendererCommand::Execute(CRenderManager &RenderManager) 
{

}
	
