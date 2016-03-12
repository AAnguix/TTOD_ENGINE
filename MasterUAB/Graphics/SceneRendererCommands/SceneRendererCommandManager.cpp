#include "SceneRendererCommandManager.h"
#include "Engine.h"
#include "DrawQuadRendererCommand.h"
#include "ClearSceneRendererCommand.h"
#include "SetMatricesSceneRendererCommand.h"
#include "SetDepthStencilStateSceneRendererCommand.h"
#include "RenderLayerSceneRendererCommand.h"
#include "RenderDebugLayerSceneRendererCommand.h"
#include "CaptureFrameBufferSceneRendererCommand.h"

#include "SetRenderTargetSceneRendererCommand.h"
#include "UnsetRenderTargetSceneRendererCommand.h"

#include "SetPoolRenderableObjectsTechniqueSceneRendererCommand.h"

#include "DeferredShadingSceneRendererCommand.h"

#include "EnableAlphaBlendSceneRendererCommand.h"
#include "DisableAlphaBlendSceneRendererCommand.h"

#include "SetLightConstantsSceneRendererCommand.h"

#include "PresentSceneRendererCommand.h"

#include "RenderGUISceneRendererCommand.h"
#include "RenderDebugGUISceneRendererCommand.h"

#include "RenderDebugLightsSceneRendererCommand.h"
#include "RenderDebugGridSceneRendererCommand.h"

#include "RenderDebugShadowMapsSceneRendererCommand.h"
#include "GenerateShadowMapsSceneRendererCommand.h"

#include "ApplyFiltersSceneRendererCommand.h"

#include "Utils.h"

#include <sstream>

CSceneRendererCommandManager::CSceneRendererCommandManager()
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
			for (int i = 0; i < l_Commands.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Commands(i);

				int l_Size=m_ResourcesVector.size();
				std::ostringstream ostr;
				ostr << l_Size;
				std::string l_S="_"+ostr.str();

				if (l_Element.GetName() == std::string("set_depth_stencil_state"))
				{
					CSetDepthStencilStateSceneRendererCommand* l_SetDSS = new CSetDepthStencilStateSceneRendererCommand(l_Element);
					if(!AddResource(l_SetDSS->GetName()+l_S,l_SetDSS))
					{
						CHECKED_DELETE(l_SetDSS);
					}
				}
				else if(l_Element.GetName() == std::string("set_pool_renderable_objects_technique"))
				{
					CSetPoolRenderableObjectsTechniqueSceneRendererCommand* l_PROTechnique = new CSetPoolRenderableObjectsTechniqueSceneRendererCommand(l_Element);
					if(!AddResource(l_PROTechnique->GetName()+l_S,l_PROTechnique))
					{
						CHECKED_DELETE(l_PROTechnique);
					}
				}
				else if(l_Element.GetName() == std::string("generate_shadow_maps"))
				{
					CGenerateShadowMapsSceneRendererCommand* l_GenerateShadowMap = new CGenerateShadowMapsSceneRendererCommand(l_Element);
					if(!AddResource(l_GenerateShadowMap->GetName()+l_S,l_GenerateShadowMap))
					{
						CHECKED_DELETE(l_GenerateShadowMap);
					}
				}
				else if(l_Element.GetName() == std::string("set_render_target"))
				{
					CSetRenderTargetSceneRendererCommand* l_SetRTarget = new CSetRenderTargetSceneRendererCommand(l_Element);
					if(!AddResource(l_SetRTarget->GetName()+l_S,l_SetRTarget))
					{
						CHECKED_DELETE(l_SetRTarget);
					}
				}
				else if(l_Element.GetName() == std::string("unset_render_target"))
				{
					CUnsetRenderTargetSceneRendererCommand* l_UnsetRTarget = new CUnsetRenderTargetSceneRendererCommand(l_Element);
					if(!AddResource(l_UnsetRTarget->GetName()+l_S,l_UnsetRTarget))
					{
						CHECKED_DELETE(l_UnsetRTarget);
					}
				}
				else if(l_Element.GetName() == std::string("set_matrices"))
				{
					CSetMatricesSceneRendererCommand* l_SetMatricesCommand = new CSetMatricesSceneRendererCommand(l_Element);
					if(!AddResource(l_SetMatricesCommand->GetName()+l_S,l_SetMatricesCommand))
					{
						CHECKED_DELETE(l_SetMatricesCommand);
					}
				}
				else if(l_Element.GetName() == std::string("clear"))
				{
					CClearSceneRendererCommand* l_ClearCommand = new CClearSceneRendererCommand(l_Element);
					if(!AddResource(l_ClearCommand->GetName()+l_S,l_ClearCommand))
					{
						CHECKED_DELETE(l_ClearCommand);
					}
				}
				else if(l_Element.GetName() == std::string("enable_alpha_blend"))
				{
					CEnableAlphaBlendSceneRendererCommand* l_EnableAB = new CEnableAlphaBlendSceneRendererCommand(l_Element);
					if(!AddResource(l_EnableAB->GetName()+l_S,l_EnableAB))
					{
						CHECKED_DELETE(l_EnableAB);
					}
				}
				else if(l_Element.GetName() == std::string("disable_alpha_blend"))
				{
					CDisableAlphaBlendSceneRendererCommand* l_DisableAB = new CDisableAlphaBlendSceneRendererCommand(l_Element);
					if(!AddResource(l_DisableAB->GetName()+l_S,l_DisableAB))
					{
						CHECKED_DELETE(l_DisableAB);
					}
				}
				else if(l_Element.GetName() == std::string("render_layer"))
				{
					CRenderLayerSceneRendererCommand* l_RenderLayerCommand = new CRenderLayerSceneRendererCommand(l_Element);
					if(!AddResource(l_RenderLayerCommand->GetName()+l_S,l_RenderLayerCommand))
					{
						CHECKED_DELETE(l_RenderLayerCommand);
					}
				}
				else if(l_Element.GetName() == std::string("render_debug_layer"))
				{
					CRenderDebugLayerSceneRendererCommand* l_RenderDebugLayerCommand = new CRenderDebugLayerSceneRendererCommand(l_Element);
					if(!AddResource(l_RenderDebugLayerCommand->GetName()+l_S,l_RenderDebugLayerCommand))
					{
						CHECKED_DELETE(l_RenderDebugLayerCommand);
					}
				}
				else if(l_Element.GetName() == std::string("render_debug_grid"))
				{
					CRenderDebugGridSceneRendererCommand* l_RenderDebugGrid = new CRenderDebugGridSceneRendererCommand(l_Element);
					if(!AddResource(l_RenderDebugGrid->GetName()+l_S,l_RenderDebugGrid))
					{
						CHECKED_DELETE(l_RenderDebugGrid);
					}
				}
				else if(l_Element.GetName() == std::string("render_gui"))
				{
					CRenderGUISceneRendererCommand* l_RenderGUI = new CRenderGUISceneRendererCommand(l_Element);
					if(!AddResource(l_RenderGUI->GetName()+l_S,l_RenderGUI))
					{
						CHECKED_DELETE(l_RenderGUI);
					}
				}
				else if(l_Element.GetName() == std::string("render_debug_gui"))
				{
					CRenderDebugGUISceneRendererCommand* l_RenderDebugGUI = new CRenderDebugGUISceneRendererCommand(l_Element);
					if(!AddResource(l_RenderDebugGUI->GetName()+l_S,l_RenderDebugGUI))
					{
						CHECKED_DELETE(l_RenderDebugGUI);
					}
				}
				else if(l_Element.GetName() == std::string("render_debug_lights"))
				{
					CRenderDebugLightsSceneRendererCommand* l_RenderDebugLights = new CRenderDebugLightsSceneRendererCommand(l_Element);
					if(!AddResource(l_RenderDebugLights->GetName()+l_S,l_RenderDebugLights))
					{
						CHECKED_DELETE(l_RenderDebugLights);
					}
				}
				else if(l_Element.GetName() == std::string("render_debug_shadowmap"))
				{
					CRenderDebugShadowMapsSceneRendererCommand* l_RenderDebugShadowmap = new CRenderDebugShadowMapsSceneRendererCommand(l_Element);
					if(!AddResource(l_RenderDebugShadowmap->GetName()+l_S,l_RenderDebugShadowmap))
					{
						CHECKED_DELETE(l_RenderDebugShadowmap);
					}
				}
				else if(l_Element.GetName() == std::string("render_draw_quad"))
				{
					CDrawQuadRendererCommand* l_DrawQuadCommand = new CDrawQuadRendererCommand(l_Element);
					if(!AddResource(l_DrawQuadCommand->GetName()+l_S,l_DrawQuadCommand))
					{
						CHECKED_DELETE(l_DrawQuadCommand);
					}
				}

				else if(l_Element.GetName() == std::string("render_deferred_shading"))
				{
					CDeferredShadingSceneRendererCommand* l_DeferredShading = new CDeferredShadingSceneRendererCommand(l_Element);
					if(!AddResource(l_DeferredShading->GetName()+l_S,l_DeferredShading))
					{
						CHECKED_DELETE(l_DeferredShading);
					}
				}
				else if(l_Element.GetName() == std::string("capture_frame_buffer"))
				{
					CCaptureFrameBufferSceneRendererCommand* l_CaptureFrameCommand = new CCaptureFrameBufferSceneRendererCommand(l_Element);
					if(!AddResource(l_CaptureFrameCommand->GetName()+l_S,l_CaptureFrameCommand))
					{
						CHECKED_DELETE(l_CaptureFrameCommand);
					}
				}
				else if(l_Element.GetName() == std::string("set_light_constants"))
				{
					CSetLightConstantsSceneRendererCommand* l_SetLightConstants = new CSetLightConstantsSceneRendererCommand(l_Element);
					if(!AddResource(l_SetLightConstants->GetName()+l_S,l_SetLightConstants))
					{
						CHECKED_DELETE(l_SetLightConstants);
					}
				}
				else if(l_Element.GetName() == std::string("present"))
				{
					CPresentSceneRendererCommand* l_PresentCommand = new CPresentSceneRendererCommand(l_Element);
					if(!AddResource(l_PresentCommand->GetName()+l_S,l_PresentCommand))
					{
						CHECKED_DELETE(l_PresentCommand);
					}
				}
				else if(l_Element.GetName() == std::string("apply_filters"))
				{
					CApplyFiltersSceneRendererCommand* l_ApplyFilters = new CApplyFiltersSceneRendererCommand(l_Element);
					if(!AddResource(l_ApplyFilters->GetName()+l_S,l_ApplyFilters))
					{
						CHECKED_DELETE(l_ApplyFilters);
					}
				}

			}

			return true;
		}
	}

	return false;
}  

bool CSceneRendererCommandManager::Reload()
{
	Destroy();
	return Load(m_Filename);
}

void CSceneRendererCommandManager::Execute(CRenderManager &RenderManager)
{
	for(size_t i=0;i<m_ResourcesVector.size();++i)
	{
		if(m_ResourcesVector[i]->GetActive())
			m_ResourcesVector[i]->Execute(RenderManager);
	}
}   