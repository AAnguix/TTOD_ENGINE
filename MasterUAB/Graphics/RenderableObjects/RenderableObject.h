#ifndef _RENDERABLEOBJECT_H
#define _RENDERABLEOBJECT_H

#include "Utils\3DElement.h"
#include "Utils\Named.h"
#include "Utils\Utils.h"

class CComponentManager;
class CComponent;
class CRenderManager;

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
	CComponentManager* m_ComponentManager;

public:
	CRenderableObject(const CXMLTreeNode &XMLTreeNode);
	CRenderableObject(const std::string &Name, const Vect3f &Position, float Yaw, float Pitch, float Roll);
	virtual ~CRenderableObject();
	virtual void Update(float ElapsedTime);
	virtual void Render(CRenderManager *RM) = 0;
	virtual bool AddComponent(CComponent *Component);
	UAB_GET_PROPERTY_POINTER(CComponentManager,ComponentManager);
	virtual TRenderableObjectType GetClassType() const = 0;

};

#endif