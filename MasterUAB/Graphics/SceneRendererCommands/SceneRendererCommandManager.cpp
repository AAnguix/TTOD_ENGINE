#include "SceneRendererCommands\SceneRendererCommandManager.h"
#include "XML\XMLTreeNode.h"
#include <sstream>

/*Commands*/
#include "SceneRendererCommands\DrawQuadRendererCommand.h"
#include "SceneRendererCommands\ClearSceneRendererCommand.h"
#include "SceneRendererCommands\SetMatricesSceneRendererCommand.h"
#include "SceneRendererCommands\SetDepthStencilStateSceneRendererCommand.h"
#include "SceneRendererCommands\RenderLayerSceneRendererCommand.h"
#include "SceneRendererCommands\RenderDebugLayerSceneRendererCommand.h"
#include "SceneRendererCommands\CaptureFrameBufferSceneRendererCommand.h"
#include "SceneRendererCommands\CaptureStencilBufferSceneRendererCommand.h"
#include "SceneRendererCommands\SetRenderTargetSceneRendererCommand.h"
#include "SceneRendererCommands\UnsetRenderTargetSceneRendererCommand.h"
#include "SceneRendererCommands\SetPoolRenderableObjectsTechniqueSceneRendererCommand.h"
#include "SceneRendererCommands\DeferredShadingSceneRendererCommand.h"
#include "SceneRendererCommands\EnableAlphaBlendSceneRendererCommand.h"
#include "SceneRendererCommands\DisableAlphaBlendSceneRendererCommand.h"
#include "SceneRendererCommands\EnableAdditiveAlphaBlendSceneRendererCommand.h"
#include "SceneRendererCommands\SetLightConstantsSceneRendererCommand.h"
#include "SceneRendererCommands\PresentSceneRendererCommand.h"
#include "SceneRendererCommands\RenderGUISceneRendererCommand.h"
#include "SceneRendererCommands\RenderDebugGUISceneRendererCommand.h"
#include "SceneRendererCommands\RenderDebugLightsSceneRendererCommand.h"
#include "SceneRendererCommands\RenderDebugGridSceneRendererCommand.h"
#include "SceneRendererCommands\RenderDebugShadowMapsSceneRendererCommand.h"
#include "SceneRendererCommands\RenderDebugBlackAndWhiteMapsSceneRendererCommand.h"
#include "SceneRendererCommands\GenerateShadowMapsSceneRendererCommand.h"
#include "SceneRendererCommands\GenerateBlackAndWhiteMapsSceneRendererCommand.h"
#include "SceneRendererCommands\ApplyFiltersSceneRendererCommand.h"
#include "Utils.h"
#include "Log\Log.h"

CSceneRendererCommandManager::CSceneRendererCommandManager()
:m_Filename("")
{
}

CSceneRendererCommandManager::~CSceneRendererCommandManager()
{
}

bool CSceneRendererCommandManager::Load(const std::string &Filename)
{
	m_Filename=Filename;

	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(Filename.c_str()))
	{
		CXMLTreeNode l_Commands = l_XML["scene_renderer_commands"];

		if (l_Commands.Exists())
		{
			Destroy();

			for (int i = 0; i < l_Commands.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Commands(i);
				std::string l_CommandName = l_Element.GetName();
				CSceneRendererCommand* l_Command = nullptr;

				if (l_CommandName == std::string("set_depth_stencil_state"))
				{
					l_Command = new CSetDepthStencilStateSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("set_pool_renderable_objects_technique"))
				{
					l_Command = new CSetPoolRenderableObjectsTechniqueSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("generate_shadow_maps"))
				{
					l_Command = new CGenerateShadowMapsSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("generate_black_and_white_maps"))
				{
					l_Command = new CGenerateBlackAndWhiteMapsSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("set_render_target"))
				{
					l_Command = new CSetRenderTargetSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("unset_render_target"))
				{
					l_Command = new CUnsetRenderTargetSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("set_matrices"))
				{
					l_Command = new CSetMatricesSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("clear"))
				{
					l_Command = new CClearSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("enable_alpha_blend"))
				{
					l_Command = new CEnableAlphaBlendSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("enable_additive_alpha_blend"))
				{
					l_Command = new CEnableAdditiveAlphaBlendSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("disable_alpha_blend"))
				{
					l_Command = new CDisableAlphaBlendSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("render_layer"))
				{
					l_Command = new CRenderLayerSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("render_debug_layer"))
				{
					l_Command = new CRenderDebugLayerSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("render_debug_grid"))
				{
					l_Command = new CRenderDebugGridSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("render_gui"))
				{
					l_Command = new CRenderGUISceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("render_debug_gui"))
				{
					l_Command = new CRenderDebugGUISceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("render_debug_lights"))
				{
					l_Command = new CRenderDebugLightsSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("render_debug_shadowmap"))
				{
					l_Command = new CRenderDebugShadowMapsSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("render_debug_black_and_white_map"))
				{
					l_Command = new CRenderDebugBlackAndWhiteMapsSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("render_draw_quad"))
				{
					l_Command = new CDrawQuadRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("render_deferred_shading"))
				{
					l_Command = new CDeferredShadingSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("capture_frame_buffer"))
				{
					l_Command = new CCaptureFrameBufferSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("capture_stencil_buffer"))
				{
					l_Command = new CCaptureStencilBufferSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("set_light_constants"))
				{
					l_Command = new CSetLightConstantsSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("present"))
				{
					l_Command = new CPresentSceneRendererCommand(l_Element);
				}
				else if (l_CommandName == std::string("apply_filters"))
				{
					l_Command = new CApplyFiltersSceneRendererCommand(l_Element);
				}
				else
				{
					if (l_CommandName != "comment")
					{
						#ifdef _DEBUG
							CEngine::GetSingleton().GetLogManager()->Log(l_CommandName + " command doesn't exists");
						#endif
						assert(false);
					}
				}

				if (l_Command != nullptr)
				{
					if (!AddResource(l_Command->GetName() + "_" + std::to_string(m_ResourcesVector.size()), l_Command))
					{
						CHECKED_DELETE(l_Command);
					}
				}
			}

			#ifdef _DEBUG
				CEngine::GetSingleton().GetLogManager()->Log(m_Filename+" loaded");
			#endif

			return true;
		}
		else
		{
			assert(false);
			return false;
		}
	}

	#ifdef _DEBUG
		CEngine::GetSingleton().GetLogManager()->Log("Unable to load scene renderer commands file " + m_Filename);
	#endif
	return false;
}  

bool CSceneRendererCommandManager::Reload()
{
	Destroy();
	bool l_Result = Load(m_Filename);
	#ifdef _DEBUG
		CEngine::GetSingleton().GetLogManager()->Log("SceneRendererCommandManager reloaded");
	#endif
	return l_Result;
}

bool CSceneRendererCommandManager::Execute(CRenderManager &RenderManager) const
{
	if (m_ResourcesVector.size() == 0)
	{
		#ifdef _DEBUG 
			LOG("There aren't any renderer commands"); 
		#endif
		return false;
	}

	for(size_t i=0;i<m_ResourcesVector.size();++i)
	{
		if (m_ResourcesVector[i]->GetActive()){ m_ResourcesVector[i]->Execute(RenderManager); }
	}
	return true;
}

void CSceneRendererCommandManager::ActivateCommand(unsigned int Index, bool Activate)
{
	if (Index < m_ResourcesVector.size()) { m_ResourcesVector[Index]->SetActive(Activate); }
}

const std::vector<CSceneRendererCommand *> & CSceneRendererCommandManager::GetLUASceneRendererCommands()
{
	return m_ResourcesVector;
}