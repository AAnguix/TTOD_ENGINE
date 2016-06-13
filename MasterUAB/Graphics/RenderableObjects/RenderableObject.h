#ifndef _RENDERABLEOBJECT_H
#define _RENDERABLEOBJECT_H

#include "Components\Component.h"
#include "Utils\3DElement.h"
class CRenderManager;
class CGameObject;

class CRenderableObject : public CComponent, public C3DElement
{

public:
	enum TRenderableObjectType
	{
		MESH_INSTANCE = 0,
		ANIMATED_INSTANCE,
		CINEMATIC,
		PARTICLE_EMITER
	};

	//CComponentManager* m_ComponentManager;
	//std::vector<CLUAComponent*> m_LuaComponents; /*Gameplay components*/

public:
	CRenderableObject(const CXMLTreeNode &XMLTreeNode);
	CRenderableObject(CGameObject *Owner, const std::string &Name, const Vect3f &Position, float Yaw, float Pitch, float Roll);
	virtual ~CRenderableObject();
	virtual void Update(float ElapsedTime);
	virtual void Render(CRenderManager *RM) = 0;

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