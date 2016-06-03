#ifndef _ANIMATORCONTROLLERMANAGER_H
#define _ANIMATORCONTROLLERMANAGER_H

#include <vector>

class CAnimatorController;
class CRenderableObject;

class CAnimatorControllerManager
{
private:
	std::vector<CAnimatorController*> m_Components;
	void UpdateComponents(float ElapsedTime);

public:
	CAnimatorControllerManager();
	virtual ~CAnimatorControllerManager();
	CAnimatorController* AddComponent(const std::string &Name, CRenderableObject* Owner);
	void RemoveComponents();
	void Update(float ElapsedTime);
};

#endif