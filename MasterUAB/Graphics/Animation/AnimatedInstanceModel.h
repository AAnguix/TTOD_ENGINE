#ifndef _ANIMATEDINSTANCEMODEL_H
#define _ANIMATEDINSTANCEMODEL_H

class CXMLTreeNode;
class CGameObject;
class CRenderManager;
class CRenderableVertexs;
class CAnimatedCoreModel;
class CalCoreModel;
class CalHardwareModel;
class CalModel;
class CAnimatorController;
class CRenderableObjectTechnique;

#include <vector>
#include "RenderableObjects\RenderableObject.h"
#include "Materials\TemplatedMaterialParameter.h"
#include "Materials\Material.h"

#define CCONTROLLER_HEIGHT 1.4f

class CAnimatedInstanceModel : public CRenderableObject 
{ 
	friend class CAnimatorController;

private:  
	CalModel* m_CalModel;
	CalHardwareModel* m_CalHardwareModel;
	std::vector<CMaterial *> m_Materials; 

	/*Materials used to add render parameters to this instance without modifying the core materials*/
	std::vector<CMaterial *> m_TemporalMaterials;

	CRenderableVertexs* m_RenderableVertexs;
	CRenderableObjectTechnique* m_TemporalRenderbleObjectTechnique;
	
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

public:  
	CAnimatedInstanceModel(CXMLTreeNode &TreeNode);  
	CAnimatedInstanceModel(CGameObject* Owner, const std::string &Name, const std::string &ModelName, const Vect3f &Position, float Yaw, float Pitch, float Roll);  //Own 
	virtual ~CAnimatedInstanceModel();  
	void Initialize(CAnimatedCoreModel *AnimatedCoreModel);  
	void Render(CRenderManager* RenderManager);
	void SetTemporalRenderableObjectTechnique(CRenderableObjectTechnique* RenderableObjectTechnique);
	std::vector<CMaterial*> CreateCopyMaterialsFromCore();
	/*template<typename T>
	void SetMaterialParameterValue(unsigned int MaterialIndex, const std::string &MaterialParameterName, T Value)
	{
		std::vector<CMaterialParameter*> l_MaterialParameters = m_TemporalMaterials[MaterialIndex]->GetParameters();
		for (size_t i = 0; i < l_MaterialParameters.size(); ++i)
		{
			if (l_MaterialParameters[i]->GetName() == MaterialParameterName)
			{
				CTemplatedMaterialParameter<T> *l_MaterialParameterPtr = static_cast<CTemplatedMaterialParameter<T> *>(l_MaterialParameters[i]);
				if (l_MaterialParameters[i]->GetMaterialType() == CMaterialParameter::FLOAT)
				{
					float l_Value = dynamic_cast<float>(T);
					l_MaterialParameterPtr->SetValue(3.0);
				}
			}
		}
	}*/
	
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
}; 

#endif