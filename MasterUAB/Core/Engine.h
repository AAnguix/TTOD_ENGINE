#ifndef _ENGINE_H
#define _ENGINE_H

//#include "Utils\SingletonPattern.h"

template <class T> 
class CSingleton
{
protected:
		static T* ms_Singleton;		///< Puntero a la variable de la propia clase.

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
	CEngine();

public:
	virtual ~CEngine();

	void Init();

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
	void SetRenderManager(CRenderManager *RenderManager) {m_RenderManager=RenderManager;}
};

#endif