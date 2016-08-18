#ifndef _RENDERABLEOBJECTSMANAGER_H
#define _RENDERABLEOBJECTSMANAGER_H

#include "RenderableObjects\RenderableObject.h"
#include "Utils\TemplatedVectorMapManager.h"
class CRenderManager;
class CGameObject;

class CRenderableObjectsManager : public CTemplatedVectorMapManager<CRenderableObject>, public CNamed
{

private:
	std::string m_Filename;
	size_t m_ObjectsInsideFrustrum;

public:
	CRenderableObjectsManager();
	virtual ~CRenderableObjectsManager();
	//void Reload();
	void Update(float ElapsedTime);
	void Render(CRenderManager *RM);
	bool RemoveRenderableObject(const std::string& RenderableObjectName);

	void RemoveRenderableObjectFromContainers(const std::string& RenderableObjectName);

	void AddParticleSystemInstance(const std::string &ParticleSystemTypeName, const std::string &ParticleSystemInstanceName, CGameObject* GameObject, const Vect3f &Position, float Yaw, float Pitch, float Roll);
	size_t GetObjectsInsideFrustrum() const{ return m_ObjectsInsideFrustrum; };
	void IncrementObjects();
};

#endif