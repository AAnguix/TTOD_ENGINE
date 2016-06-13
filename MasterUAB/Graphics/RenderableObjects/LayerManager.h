#ifndef _LAYERMANAGER_H
#define _LAYERMANAGER_H

#include "RenderableObjectsManager.h"
class CAnimatedInstanceModel;
class CGameObject;
class CMeshInstance;

class CLayerManager : public CTemplatedVectorMapManager<CRenderableObjectsManager> 
{ 

private:  
	std::string m_Filename;  
	CRenderableObjectsManager *m_DefaultLayer;  

	CRenderableObjectsManager* GetLayer(CXMLTreeNode &Node);
	CRenderableObjectsManager* AddLayer(CXMLTreeNode &TreeNode); 

	bool AddElementToLayer(const std::string &Layer, CRenderableObject* RenderableObject);

public:  
	CLayerManager();  
	virtual ~CLayerManager();  

	/*CRenderableObject* AddMeshInstance(const std::string &Layer, std::string &CoreMeshName, const std::string &InstanceMeshName, const Vect3f &Position, float Yaw, float Pitch, float Roll);
	CRenderableObject* AddAnimatedInstanceModel(const std::string &Layer, const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position, float Yaw, float Pitch, float Roll);
*/
	CMeshInstance* AddMeshComponent(const std::string &Layer, std::string &CoreMeshName, const std::string &InstanceMeshName, CGameObject* Owner, const Vect3f &Position, float Yaw, float Pitch, float Roll);
	CAnimatedInstanceModel* AddAnimatedComponent(const std::string &Layer, const std::string &CoreModelName, const std::string &InstanceModelName, CGameObject* Owner, const Vect3f &Position, float Yaw, float Pitch, float Roll);

	//void Destroy();  
	void Load(const std::string &Filename);  
	void Reload();  
	void Update(float ElapsedTime); 
	void Render(CRenderManager &RenderManager);  
	void Render(CRenderManager &RenderManager, const std::string &LayerName);
};

#endif