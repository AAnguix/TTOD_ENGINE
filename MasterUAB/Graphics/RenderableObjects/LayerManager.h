#ifndef _LAYERMANAGER_H
#define _LAYERMANAGER_H

#include "RenderableObjectsManager.h"
class CAnimatedInstanceModel;

class CLayerManager : public CTemplatedVectorMapManager<CRenderableObjectsManager> 
{ 

private:  
	std::string m_Filename;  
	CRenderableObjectsManager *m_DefaultLayer;  
	CAnimatedInstanceModel* m_Player;

	CRenderableObjectsManager* GetLayer(CXMLTreeNode &Node);
	CRenderableObjectsManager* AddLayer(CXMLTreeNode &TreeNode); 

	bool AddElementToLayer(const std::string &Layer, CRenderableObject* RenderableObject);

public:  
	CLayerManager();  
	virtual ~CLayerManager();  

	CRenderableObject* AddMeshInstance(const std::string &Layer, std::string &CoreMeshName, const std::string &InstanceMeshName, const Vect3f &Position, float Yaw, float Pitch, float Roll);
	CRenderableObject* AddAnimatedInstanceModel(const std::string &Layer, const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position, float Yaw, float Pitch, float Roll);

	//void Destroy();  
	void Load(const std::string &Filename);  
	void Reload();  
	void Update(float ElapsedTime); 
	void Render(CRenderManager &RenderManager);  
	void Render(CRenderManager &RenderManager, const std::string &LayerName);
	
	/*Removes component's from one specific layer*/
	void RemoveLayerComponent(const std::string &LayerName, const std::string &ComponentName);
	void RemoveLayerComponents(const std::string &LayerName);
	
	/*Removes component's from all layers*/
	void RemoveComponent(const std::string &ComponentName);
	void RemoveComponents();

	void RemoveLayerLuaComponents(const std::string &LayerName);
	void RemoveLuaComponents();

	CAnimatedInstanceModel* GetPlayer() const{assert(m_Player != nullptr); return m_Player;};
	void SetPlayer(CAnimatedInstanceModel* Player){ m_Player = Player; };
};

#endif