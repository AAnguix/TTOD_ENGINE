#ifndef _ENGINE_H
#define _ENGINE_H

//#include "Utils\SingletonPattern.h"
#include <string>
#include <vector>

#define LOG(Message) \
CEngine::GetSingleton().GetLogManager()->Log(Message);


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

class CGameObjectManager;
class CCameraControllerManager;
class CPhysXManager;
class CLayerManager;
class ISoundManager;
class CGUIManager;
class CAnimatorControllerManager;
class CLuabindManager;
class CScriptManager;

class CMaterialManager;
class CEffectManager;
class CTextureManager;
class CStaticMeshManager;
class CRenderManager;
class CAnimatedModelManager;
class CLightManager;
class CLog;
class CRenderableObjectTechniqueManager;
class CSceneRendererCommandManager;
class CDebugHelperImplementation;
class CParticleManager;
class CInputManagerImplementation;
class CGraphicsStats;
class CEmptyPointerClass;
class CLevel;
class CProfiler;

class CEngine : public CSingleton<CEngine>
{
friend class CSingleton<CEngine>;
private:
	/*Updated every frame*/
	CCameraControllerManager* m_CameraControllerManager;
	CPhysXManager* m_PhysXManager;
	CLayerManager* m_LayerManager;
	ISoundManager* m_SoundManager;
	CGUIManager* m_GUIManager;
	CAnimatorControllerManager* m_AnimatorControllerManager;
	CLuabindManager* m_LuabindManager;
	CScriptManager* m_ScriptManager;

	CGameObjectManager* m_GameObjectManager;
	CMaterialManager* m_MaterialManager;
	CEffectManager* m_EffectManager;
	CTextureManager* m_TextureManager;
	CStaticMeshManager* m_StaticMeshManager;
	CRenderManager* m_RenderManager;
	CAnimatedModelManager* m_AnimatedModelManager;
	CLightManager* m_LightManager;
	CLog* m_Log;
	CRenderableObjectTechniqueManager* m_RenderableObjectTechniqueManager;
	CSceneRendererCommandManager* m_SceneRendererCommandManager;
	CDebugHelperImplementation* m_DebugHelper;
	CParticleManager* m_ParticleSystemManager;
	CInputManagerImplementation* m_InputManager;
	CGraphicsStats* m_GraphicsStats;
	CProfiler* m_Profiler;

	std::string m_CurrentLevel;
	float m_TimeScale;
	bool m_Paused;

	std::vector<CLevel*> m_Levels;
	bool m_LoadingLevel;
	bool m_GuiLoaded;

	float m_SecsPerCnt;
	__int64 m_PrevTimeStamp;
	float m_TimeSinceStart;

	CEngine();

public:
	virtual ~CEngine();

	float GetRealTimeSinceStartup();
	void SetTimeScale(float TimeScale);
	void Pause(){ m_Paused = true; };
	bool IsPaused() const { return m_Paused; };
	CEmptyPointerClass* GetTimeScaleLuaAddress() const;
	CEmptyPointerClass* GetPausedLuaAddress() const;
	void Update(float ElapsedTime);
	
	void Initialize();
	void LoadLevelsCommonData();
	bool AddLevel(const std::string &Level);
	bool LoadLevel(const std::string &Level);
	bool UnloadLevel(const std::string &Level);
	bool LoadingLevel() const { return m_LoadingLevel; };
	std::string GetCurrentLevel() const { return m_CurrentLevel; };

	static void TerminateApplication(); /*Called from LUA when exit button is pressed*/

	CGameObjectManager* GetGameObjectManager() const;
	CMaterialManager* GetMaterialManager() const;
	CEffectManager* GetEffectManager() const;
	CTextureManager* GetTextureManager() const;
	//CRenderableObjectsManager* GetRenderableObjectsManager() const;
	CStaticMeshManager* GetStaticMeshManager() const;
	CRenderManager* GetRenderManager() const;
	CLuabindManager* GetLuabindManager() const;
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
	CAnimatorControllerManager* GetAnimatorControllerManager() const;
	CScriptManager* GetScriptManager() const;
	CProfiler* GetProfiler() const;

	void SetRenderManager(CRenderManager *RenderManager) {m_RenderManager=RenderManager;}
};

#endif