#ifndef _ANIMATEDINSTANCEMODEL_H
#define _ANIMATEDINSTANCEMODEL_H

class CXMLTreeNode;
class CRenderManager;
class CMaterial;
class CRenderableVertexs;

#include "Animation\AnimatedCoreModel.h"

#include <vector>
#include "cal3d\hardwaremodel.h"
#include "cal3d\model.h"
#include "RenderableObjects\RenderableObject.h"
#include "Animation\AnimatorController.h"

#define CCONTROLLER_HEIGHT 1.4f

class CAnimatedInstanceModel : public CRenderableObject 
{ 

private:  
	CalModel   *m_CalModel;
	CAnimatedCoreModel  *m_AnimatedCoreModel;
	CalHardwareModel  *m_CalHardwareModel;
	std::vector<CMaterial *> m_Materials; 
	CRenderableVertexs  *m_RenderableVertexs;
	CAnimatorController* m_AnimatorController;

	int m_NumVertices;
	int m_NumFaces;  
	bool LoadVertexBuffer();
	void LoadMaterials();

	unsigned int m_lastTick; //new
	float m_fpsDuration;
	int m_fpsFrames;
	int m_fps;
	bool m_bPaused;
	float m_leftAnimationTime;
	float m_blendTime;
	int m_currentAnimationId;

public:  
	CAnimatedInstanceModel(CXMLTreeNode &TreeNode);  
	CAnimatedInstanceModel(const std::string &Name);  //Own 
	virtual ~CAnimatedInstanceModel();  
	void Initialize(CAnimatedCoreModel *AnimatedCoreModel);  
	void Render(CRenderManager* RenderManager);  
	void Update(float ElapsedTime);  
	void Destroy(); 

	CAnimatorController* GetAnimatorController() const { return m_AnimatorController;};
	void ExecuteAction(int Id, float DelayIn, float DelayOut, float WeightTarget=1.0f, bool AutoLock=true);  
	void BlendCycle(int Id, float Weight, float DelayIn);  
	void ClearCycle(int Id, float DelayOut);  
	bool IsCycleAnimationActive(int Id) const;  
	bool IsActionAnimationActive(int Id) const; 
	TRenderableObjectType GetClassType() const{ return TRenderableObjectType::ANIMATED_INSTANCE; };
}; 

#endif