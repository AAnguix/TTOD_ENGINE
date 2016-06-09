#include "RenderableObjects\MeshInstance.h"
#include "Engine.h"
#include "PhysXManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "StaticMeshes\StaticMesh.h"
#include "Utils\Named.h"
#include "Render\RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "Materials\Material.h"
#include "Components\Collider.h"
#include "Animation\AnimatedInstanceModel.h"
#include "RenderableObjects\LayerManager.h"
#include "Animation\AnimatedModelManager.h"
#include "Render\DebugRender.h"

/*Used to create a mesh instance when game is runing*/
CMeshInstance::CMeshInstance(const std::string &Name, const std::string &CoreName, const Vect3f &Position, float Yaw, float Pitch, float Roll)
:CRenderableObject(Name,Position,Yaw,Pitch,Roll)
,m_StaticMesh(nullptr)
,m_Parent(nullptr)
,m_ParentBoneId(-1)
,m_Collider(nullptr)
{
	m_StaticMesh = CEngine::GetSingleton().GetStaticMeshManager()->GetResource(CoreName);
}

/*Used to create a mesh instance when game starts*/
CMeshInstance::CMeshInstance(CXMLTreeNode &TreeNode)
:CRenderableObject(TreeNode)
,m_StaticMesh(nullptr)
,m_Parent(nullptr)
,m_ParentBoneId(-1)
,m_Collider(nullptr)
{
	std::string l_Core = TreeNode.GetPszProperty("core_name","");
	m_StaticMesh = CEngine::GetSingleton().GetStaticMeshManager()->GetResource(l_Core);
	
	if(m_StaticMesh!=nullptr)
	{
		std::string l_Name = TreeNode.GetPszProperty("name", "");
		std::string l_ActorType = TreeNode.GetPszProperty("actor_type", "");
		std::string l_Geometry = TreeNode.GetPszProperty("geometry", "");

		std::string l_ParentName = TreeNode.GetPszProperty("parent_name", "");
		std::string l_ParentLayer = TreeNode.GetPszProperty("parent_layer", "");
		std::string l_ParentBoneName = TreeNode.GetPszProperty("parent_bone", "");
		
		if (l_ParentName != "" && l_ParentLayer != "" && l_ParentBoneName != "")
		{
			m_Parent = (CAnimatedInstanceModel*)CEngine::GetSingleton().GetLayerManager()->GetResource(l_ParentLayer)->GetResource(l_ParentName);
			assert(m_Parent != nullptr);
			m_ParentBoneId = ((CAnimatedInstanceModel*)m_Parent)->GetAnimatedCoreModel()->GetBoneId(l_ParentBoneName);
		}

		CMaterial* l_PhysicsMat = m_StaticMesh->GetPhysxMaterial();
		std::string l_MaterialName = "";
		if(l_PhysicsMat!=nullptr)
			l_MaterialName=l_PhysicsMat->GetName();

		/*TODO Obtener propiedades del material*/
		if (l_MaterialName != "")
		{
			if (l_ActorType != "")
			{
				bool l_Trigger = TreeNode.GetBoolProperty("trigger", false);
				unsigned int l_Group = TreeNode.GetIntProperty("group", 0);
				float l_Density = TreeNode.GetFloatProperty("density", 1.0f);

				//bool l_AbleToBeTrigger = true;

				CEngine::GetSingleton().GetPhysXManager()->AddColliderComponent(m_Name+"_Collider", this);
				//CMaterial* m_PhysxMaterial = m_StaticMesh->GetPhysxMaterial();
				//CEngine::GetSingleton().GetPhysXManager()->RegisterMaterial(l_MaterialName, m_PhysxMaterial->GetStaticFriction(), m_PhysxMaterial->GetDynamicFriction(), m_PhysxMaterial->GetRestitution());
				
				//m_StaticMesh->GetPhysxMaterial()->SetPhysxPropertiesAddresses();
				
				//Quatf l_Rotation(0.0f, 0.0f, 0.0f, 1.0f);
				Quatf l_Rotation(m_Yaw, m_Pitch, m_Roll);

				if (l_ActorType == "static")
					CEngine::GetSingleton().GetPhysXManager()->CreateStaticActor(m_Name, m_StaticMesh->GetName(), m_Position, l_Rotation);
				else if (l_ActorType == "dynamic")
					CEngine::GetSingleton().GetPhysXManager()->CreateDynamicActor(m_Name, m_StaticMesh->GetName(), m_Position, l_Rotation, l_Density, false);
				else if (l_ActorType == "kinematic")
					CEngine::GetSingleton().GetPhysXManager()->CreateDynamicActor(m_Name, m_StaticMesh->GetName(), m_Position, l_Rotation, l_Density, true);
				/*if (l_Geometry == "plane")
				{
					assert(l_ActorType == "static");
					CEngine::GetSingleton().GetPhysXManager()->CreateRigidStaticPlane(l_Name, Vect3f(0.0f, 1.0f, 0.0f), 0.0f, l_MaterialName, m_Position, l_Rotation, l_Group);
				}
				else if (l_Geometry == "box")
				{
					if (l_ActorType == "static")
						CEngine::GetSingleton().GetPhysXManager()->CreateRigidStaticBox(l_Name, m_StaticMesh->GetBoundingBoxSize(), l_MaterialName, m_Position, l_Rotation, l_Group);
					else if (l_ActorType == "dynamic" || l_ActorType == "kinematic")
						CEngine::GetSingleton().GetPhysXManager()->CreateRigidDynamicBox(l_Name, m_StaticMesh->GetBoundingBoxSize(), l_MaterialName, m_Position, l_Rotation, 1, l_Density, l_IsKinematic);
				}
				else if (l_Geometry == "sphere")
				{
					if (l_ActorType == "static")
						CEngine::GetSingleton().GetPhysXManager()->CreateRigidStaticSphere(l_Name, m_StaticMesh->GetBoundingSphereRadius(), l_MaterialName, m_Position, l_Rotation, l_Group);
					else if (l_ActorType == "dynamic" || l_ActorType == "kinematic")
						CEngine::GetSingleton().GetPhysXManager()->CreateRigidDynamicSphere(l_Name, m_StaticMesh->GetBoundingSphereRadius(), l_MaterialName, m_Position, l_Rotation, l_Group, l_Density, l_IsKinematic);
				}
				else if (l_Geometry == "capsule")
				{
					if (l_ActorType == "static")
						CEngine::GetSingleton().GetPhysXManager()->CreateRigidStaticCapsule(l_Name, m_StaticMesh->GetCapsuleRadius(), m_StaticMesh->GetCapsuleHalfHeight(), l_MaterialName, m_Position, l_Rotation, l_Group);
					else if (l_ActorType == "kinematic" || l_ActorType == "dynamic")
						CEngine::GetSingleton().GetPhysXManager()->CreateRigidDynamicCapsule(l_Name, m_StaticMesh->GetCapsuleRadius(), m_StaticMesh->GetCapsuleHalfHeight(), l_MaterialName, m_Position, l_Rotation, l_Group, l_Density, l_IsKinematic);
				}
				else if (l_Geometry == "convex")
				{
					if (l_ActorType == "static")
						CEngine::GetSingleton().GetPhysXManager()->CreateRigidStaticConvexMesh(l_Name, m_StaticMesh->GetName(), m_StaticMesh->GetVertex(), l_MaterialName, m_Position, l_Rotation, l_Group);
					else if (l_ActorType == "kinematic" || l_ActorType == "dynamic")
						CEngine::GetSingleton().GetPhysXManager()->CreateRigidDynamicConvexMesh(l_Name, m_StaticMesh->GetName(), m_StaticMesh->GetVertex(), l_MaterialName, m_Position, l_Rotation, l_Group, l_Density, l_IsKinematic);
				}
				else if (l_Geometry == "triangle")
				{
					l_AbleToBeTrigger = false;
					if (l_ActorType == "static")
						CEngine::GetSingleton().GetPhysXManager()->CreateRigidStaticTriangleMesh(l_Name, m_StaticMesh->GetName(), m_StaticMesh->GetVertex(), m_StaticMesh->GetIndex(), l_MaterialName, m_Position, l_Rotation, l_Group);
					else if (l_ActorType == "kinematic")
						CEngine::GetSingleton().GetPhysXManager()->CreateRigidKinematicTriangleMesh(l_Name, m_StaticMesh->GetName(), m_StaticMesh->GetVertex(), m_StaticMesh->GetIndex(), l_MaterialName, m_Position, l_Rotation, l_Group, l_Density);
				}
				else assert(false);*/

				if (l_Trigger)
				{
					//assert(l_AbleToBeTrigger);
					CEngine::GetSingleton().GetPhysXManager()->SetShapeAsTrigger(m_StaticMesh->GetName());
				}
			}
		
		}
	}
}

CMeshInstance::~CMeshInstance()
{
}

static int b_Test = 0;
void CMeshInstance::Render(CRenderManager* RenderManager)
{
	if (m_StaticMesh == NULL)
		return;

	bool l_IsOutsideFrustum = false;

	#if ENABLE_FRUSTUM
	//if (!RenderManager->GetFrustum().BoxVisible(m_StaticMesh->GetBoundingBoxMax(), m_StaticMesh->GetBoundingBoxMin()))
	if (!RenderManager->GetFrustum().SphereVisible(GetTransform()*m_StaticMesh->GetBoundingSphereCenter(), m_StaticMesh->GetBoundingSphereRadius()))
		l_IsOutsideFrustum = true;
	#endif
	
	if (m_Visible && !l_IsOutsideFrustum)
	{
		if (m_Parent != nullptr && m_ParentBoneId != -1)
		{
			Mat44f l_BoneTransform = m_Parent->GetBoneTransformationMatrix(m_ParentBoneId);
			//Mat44f l_ParentTransform = m_Parent->ChildGetTransform(m_Parent->GetPitch(), m_Parent->GetYaw(), m_Parent->GetRoll());
			Mat44f l_ParentTransform = m_Parent->GetTransform();
			CContextManager* l_ContextManager = RenderManager->GetContextManager();
			
			l_ContextManager->SetWorldMatrix(l_BoneTransform*l_ParentTransform);
			l_ContextManager->Draw(RenderManager->GetDebugRender()->GetAxis());
			/*l_ContextManager->SetWorldMatrix(ChildGetTransform(m_Pitch, m_Yaw, m_Roll)*l_BoneTransform*l_ParentTransform);*/
			Mat44f l_RotX, l_RotY, l_RotZ, l_Translation;
			l_Translation.SetIdentity();
			l_Translation.Translate(m_Position);
			l_RotX.SetIdentity();
			l_RotX.RotByAngleX(m_Pitch);
			l_RotY.SetIdentity();
			l_RotY.RotByAngleY(m_Yaw);
			l_RotZ.SetIdentity();
			l_RotZ.RotByAngleZ(m_Roll);

			Mat44f l_Tranform;
			l_Tranform = l_RotX*l_RotY*l_RotZ*l_Translation;
			/*if (b_Test==0)
				l_Tranform = l_RotZ*l_RotY*l_RotX*l_Translation;
			else if (b_Test == 1)
				l_Tranform = l_RotZ*l_RotX*l_RotY*l_Translation;
			else if (b_Test == 2)*/
				
			/*else if (b_Test == 3)
				l_Tranform = l_RotX*l_RotZ*l_RotY*l_Translation;
			else if (b_Test == 4)
				l_Tranform = l_RotY*l_RotZ*l_RotX*l_Translation;
			else if (b_Test == 5)
				l_Tranform = l_RotY*l_RotX*l_RotZ*l_Translation;*/

			l_ContextManager->SetWorldMatrix(l_Tranform*l_BoneTransform*l_ParentTransform);
		}
		else
		{
			RenderManager->GetContextManager()->SetWorldMatrix(GetTransform());
		}
		m_StaticMesh->Render(RenderManager);
	}
}


void CMeshInstance::SetParent(CAnimatedInstanceModel* Parent, const std::string &BoneName)
{
	assert(Parent != nullptr);
	if (Parent != nullptr)
	{
		m_Parent = Parent;
		int l_ParentBoneId = m_Parent->GetAnimatedCoreModel()->GetBoneId(BoneName);
		if (l_ParentBoneId != -1)
			m_ParentBoneId = l_ParentBoneId;
	}
}

CCollider* CMeshInstance::GetCollider() const
{
	return m_Collider;
}

void CMeshInstance::SetCollider(CCollider* CCollider)
{
	m_Collider = CCollider;
}