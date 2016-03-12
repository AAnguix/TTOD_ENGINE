#include "Engine.h"
#include "Materials\MaterialManager.h"
#include "Effects\EffectManager.h"
#include "Textures\TextureManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "ScriptManager\ScriptManager.h"
#include "Animation\AnimatedModelManager.h"
#include "Lights\LightManager.h"
#include "Camera\CameraControllerManager.h"
#include "Render\RenderManager.h"
#include "Log.h"
#include "PhysXManager.h"
#include "RenderableObjects\LayerManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "SceneRendererCommands\SceneRendererCommandManager.h"
#include "DebugHelperImplementation.h"

CEngine::CEngine()
: m_RenderManager(NULL)
{
	m_CameraControllerManager = new CCameraControllerManager;
	m_EffectManager = new CEffectManager;
	m_MaterialManager = new CMaterialManager;
	m_TextureManager = new CTextureManager;

	m_StaticMeshManager = new CStaticMeshManager;
	//m_RenderableObjectsManager = new CRenderableObjectsManager;
	m_ScriptManager = new CScriptManager;
	m_AnimatedModelManager = new CAnimatedModelManager;
	m_LightManager = new CLightManager;
	m_Log = new CLog;
	m_PhysXManager = CPhysXManager::CreatePhysxManager();
	m_LayerManager = new CLayerManager();
	m_RenderableObjectTechniqueManager = new CRenderableObjectTechniqueManager;
	m_SceneRendererCommandManager = new CSceneRendererCommandManager;
	m_DebugHelper = new CDebugHelperImplementation;
}

CEngine::~CEngine()
{
	{CHECKED_DELETE(m_DebugHelper);}
	{CHECKED_DELETE(m_SceneRendererCommandManager);}
	{CHECKED_DELETE(m_RenderableObjectTechniqueManager);}
	{CHECKED_DELETE(m_LayerManager);}
	{CHECKED_DELETE(m_PhysXManager);}
	{CHECKED_DELETE(m_Log);}
	{CHECKED_DELETE(m_LightManager);}
	{CHECKED_DELETE(m_AnimatedModelManager);}
	{CHECKED_DELETE(m_ScriptManager);}
	//{CHECKED_DELETE(m_RenderableObjectsManager);}
	{CHECKED_DELETE(m_StaticMeshManager);}
	{CHECKED_DELETE(m_TextureManager);}
	{CHECKED_DELETE(m_MaterialManager);}
	{CHECKED_DELETE(m_EffectManager);}
	{CHECKED_DELETE(m_CameraControllerManager);}
}

void CEngine::Init()
{

}

CEffectManager* CEngine::GetEffectManager() const
{
	return m_EffectManager;
}

CMaterialManager* CEngine::GetMaterialManager() const
{
	return m_MaterialManager;
}
	
CTextureManager* CEngine::GetTextureManager() const
{
	return m_TextureManager;
}

/*
CRenderableObjectsManager* CEngine::GetRenderableObjectsManager() const
{
	return m_RenderableObjectsManager;
}*/

CStaticMeshManager* CEngine::GetStaticMeshManager() const
{
	return m_StaticMeshManager;
}

CScriptManager* CEngine::GetScriptManager() const
{
	return m_ScriptManager;
}

CAnimatedModelManager* CEngine::GetAnimatedModelManager() const
{
	return m_AnimatedModelManager;
}

CLightManager* CEngine::GetLightManager() const
{
	return m_LightManager;
}

CCameraControllerManager* CEngine::GetCameraControllerManager() const
{
	return m_CameraControllerManager;
}

CLog* CEngine::GetLogManager() const
{
	return m_Log;
}

CPhysXManager* CEngine::GetPhysXManager() const
{
	return m_PhysXManager;
}

CLayerManager* CEngine::GetLayerManager() const
{
	return m_LayerManager;
}

CRenderableObjectTechniqueManager* CEngine::GetRenderableObjectTechniqueManager() const
{
	return m_RenderableObjectTechniqueManager;
}

CSceneRendererCommandManager* CEngine::GetSceneRendererCommandManager() const
{
	return m_SceneRendererCommandManager;
}

CDebugHelperImplementation* CEngine::GetDebugHelper() const
{
	return m_DebugHelper;
}

CRenderManager* CEngine::GetRenderManager() const
{
	return m_RenderManager;
}