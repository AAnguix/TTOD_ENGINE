#ifndef _LAYERMANAGER_H
#define _LAYERMANAGER_H

#include "RenderableObjectsManager.h"
class CAnimatedInstanceModel;

class CLayerManager : public CTemplatedVectorMapManager<CRenderableObjectsManager> 
{ 

private:  
	std::string m_Filename;  
	CRenderableObjectsManager     *m_DefaultLayer;  
	CAnimatedInstanceModel* m_Player;

	CRenderableObjectsManager * GetLayer(CXMLTreeNode &Node);  
	CRenderableObjectsManager * AddLayer(CXMLTreeNode &TreeNode); 

public:  
	CLayerManager();  
	virtual ~CLayerManager();  
	//void Destroy();  
	void Load(const std::string &Filename);  
	void Reload();  
	void Update(float ElapsedTime); 
	void Render(CRenderManager &RenderManager);  
	void Render(CRenderManager &RenderManager, const std::string &LayerName);
	CAnimatedInstanceModel* GetPlayer() const;
};

#endif