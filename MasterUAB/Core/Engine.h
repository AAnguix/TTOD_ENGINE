#ifndef _ENGINE_H
#define _ENGINE_H

//#include "Utils\SingletonPattern.h"
#include <string>

template <class T> 
class CSingleton
{
protected:
	static T* ms_Singleton; /*Pointer to member var*/

	CSingleton()
	{
	}

public:
	virtual ~CSingleton()
	{  
	}
	static T & GetSingleton()
	{	
		return *GetSingletonPtr();
	}
	static T * GetSingletonPtr()
	{ 
		if(ms_Singleton==0)
			ms_Singleton=new T;
		return ms_Singleton;
	}
};

template<class T> T* CSingleton<T>::ms_Singleton = 0;


class CMaterialManager;
class CEffectManager;
class CTextureManager;
//class CRenderableObjectsManager;
class CStaticMeshManager;
class CRenderManager;
class CScriptManager;
class CAnimatedModelManager;
class CLightManager;
class CCameraControllerManager;
class CLog;
class CPhysXManager;
class CLayerManager;
class CRenderableObjectTechniqueManager;
class CSceneRendererCommandManager;
class CDebugHelperImplementation;
class CParticleManager;
class CInputManagerImplementation;
class CGUIManager;
class ISoundManager;
class CGraphicsStats;

class CEngine : public CSingleton<CEngine>
{
friend class CSingleton<CEngine>;
private:

	CMaterialManager* m_MaterialManager;
	CEffectManager* m_EffectManager;
	CTextureManager* m_TextureManager;
	//CRenderableObjectsManager* m_RenderableObjectsManager;
	CStaticMeshManager* m_StaticMeshManager;
	CRenderManager* m_RenderManager;
	CScriptManager* m_ScriptManager;
	CAnimatedModelManager* m_AnimatedModelManager;
	CLightManager* m_LightManager;
	CCameraControllerManager* m_CameraControllerManager;
	CLog* m_Log;
	CPhysXManager* m_PhysXManager;
	CLayerManager* m_LayerManager;
	CRenderableObjectTechniqueManager* m_RenderableObjectTechniqueManager;
	CSceneRendererCommandManager* m_SceneRendererCommandManager;
	CDebugHelperImplementation* m_DebugHelper;
	CParticleManager* m_ParticleSystemManager;
	CInputManagerImplementation* m_InputManager;
	CGUIManager* m_GUIManager;
	ISoundManager* m_SoundManager;
	CGraphicsStats* m_GraphicsStats;
	CEngine();

public:
	virtual ~CEngine();

	void Init();
	void LoadLevel(const std::string& Level);

	static void TerminateApplication(); /*Called from LUA when exit button is pressed*/

	CMaterialManager* GetMaterialManager() const;
	CEffectManager* GetEffectManager() const;
	CTextureManager* GetTextureManager() const;
	//CRenderableObjectsManager* GetRenderableObjectsManager() const;
	CStaticMeshManager* GetStaticMeshManager() const;
	CRenderManager* GetRenderManager() const;
	CScriptManager* GetScriptManager() const;
	CAnimatedModelManager* GetAnimatedModelManager() const;
	CLightManager* GetLightManager() const;
	CCameraControllerManager* GetCameraControllerManager() const;
	CLog* GetLogManager() const;
	CPhysXManager* GetPhysXManager() const;
	CLayerManager* GetLayerManager() const;
	CRenderableObjectTechniqueManager* GetRenderableObjectTechniqueManager() const;
	CSceneRendererCommandManager* GetSceneRendererCommandManager() const;
	CDebugHelperImplementation* GetDebugHelper() const;
	CParticleManager* GetParticleSystemManager() const;
	CInputManagerImplementation* GetInputManager() const;
	CGUIManager* GetGUIManager() const;
	ISoundManager* GetSoundManager() const;
	CGraphicsStats* GetGraphicsStats() const;
	void SetRenderManager(CRenderManager *RenderManager) {m_RenderManager=RenderManager;}
};

#endif