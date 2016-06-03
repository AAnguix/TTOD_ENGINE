#ifndef _ANIMATEDINSTANCEMODEL_H
#define _ANIMATEDINSTANCEMODEL_H

class CXMLTreeNode;
class CRenderManager;
class CMaterial;
class CRenderableVertexs;
class CCharacterCollider;
class CAnimatedCoreModel;
class CalCoreModel;
class CalHardwareModel;
class CalModel;
class CAnimatorController;

#include <vector>
#include "RenderableObjects\RenderableObject.h"

#define CCONTROLLER_HEIGHT 1.4f

class CAnimatedInstanceModel : public CRenderableObject 
{ 
	friend class CAnimatorController;

private:  
	CalModel* m_CalModel;
	CalHardwareModel* m_CalHardwareModel;
	std::vector<CMaterial *> m_Materials; 
	CRenderableVertexs* m_RenderableVertexs;
	
	int m_NumVertices;
	int m_NumFaces;  
	bool LoadVertexBuffer();
	void LoadMaterials();

	unsigned int m_lastTick; 
	float m_fpsDuration;
	int m_fpsFrames;
	int m_fps;
	bool m_bPaused;
	float m_leftAnimationTime;
	float m_blendTime;
	int m_currentAnimationId;

protected:
	CAnimatedCoreModel* m_AnimatedCoreModel;
	CCharacterCollider* m_CharacterCollider;

public:  
	CAnimatedInstanceModel(CXMLTreeNode &TreeNode);  
	CAnimatedInstanceModel(const std::string &Name, const std::string &ModelName, const Vect3f &Position, float Yaw, float Pitch, float Roll);  //Own 
	virtual ~CAnimatedInstanceModel();  
	void Initialize(CAnimatedCoreModel *AnimatedCoreModel);  
	void Render(CRenderManager* RenderManager);  
	void Update(float ElapsedTime);  
	void Destroy(); 

	CAnimatedCoreModel* GetAnimatedCoreModel() const { return m_AnimatedCoreModel; };
	void ExecuteAction(int Id, float DelayIn, float DelayOut, float WeightTarget=1.0f, bool AutoLock=true);  
	void BlendCycle(int Id, float Weight, float DelayIn);  
	void ClearCycle(int Id, float DelayOut);  
	bool IsCycleAnimationActive(int Id) const;  
	bool IsActionAnimationActive(int Id) const; 
	TRenderableObjectType GetClassType() const{ return TRenderableObjectType::ANIMATED_INSTANCE; };

	Mat44f GetBoneTransformationMatrix(const int BoneID) const;

	virtual CCharacterCollider* GetCharacterCollider() const;
	virtual void SetCharacterCollider(CCharacterCollider* CharacterCollider);
}; 

#endif