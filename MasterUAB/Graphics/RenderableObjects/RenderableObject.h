#ifndef _RENDERABLEOBJECT_H
#define _RENDERABLEOBJECT_H

#include "Utils\3DElement.h"
#include "Utils\Named.h"
class CRenderManager;
class CAnimatorController;
class CScript;
class CAudioSource;

class CRenderableObject : public C3DElement, public CNamed
{

public:
	enum TRenderableObjectType
	{
		MESH_INSTANCE = 0,
		ANIMATED_INSTANCE,
		CINEMATIC,
		PARTICLE_EMITER
	};

protected:
	CAnimatorController* m_AnimatorController;
	CScript* m_Script;
	CAudioSource* m_AudioSource;

	//CComponentManager* m_ComponentManager;
	//std::vector<CLUAComponent*> m_LuaComponents; /*Gameplay components*/

public:
	CRenderableObject(const CXMLTreeNode &XMLTreeNode);
	CRenderableObject(const std::string &Name, const Vect3f &Position, float Yaw, float Pitch, float Roll);
	virtual ~CRenderableObject();
	virtual void Update(float ElapsedTime);
	virtual void Render(CRenderManager *RM) = 0;

	virtual CAnimatorController* GetAnimatorController() const;
	virtual CScript* GetScript() const;
	virtual CAudioSource* GetAudioSource() const;
	
	virtual void SetAudioSource(CAudioSource* AudioSource);
	virtual void SetScript(CScript* Script);
	virtual void SetAnimatorController(CAnimatorController* AnimatorController);

	/*virtual bool AddComponent(CComponent *Component);
	virtual void RemoveComponent(const std::string &ComponentName);
	virtual void RemoveComponents();

	CAnimatorController* GetAnimatorController() const;*/

	/*UAB_GET_PROPERTY_POINTER(CComponentManager,ComponentManager);
	
	virtual bool AddLuaComponent(CLUAComponent* LUAComponent);
	CLUAComponent* GetFirstLuaComponent() const;
	virtual void RemoveLuaComponents();*/

	virtual TRenderableObjectType GetClassType() const = 0;
	virtual CEmptyPointerClass* GetThisLuaAddress() const;

};

#endif