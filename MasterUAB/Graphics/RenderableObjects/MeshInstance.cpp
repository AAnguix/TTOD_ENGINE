#include "RenderableObjects\MeshInstance.h"
#include "GameObject\GameObject.h"
#include "Engine\Engine.h"
#include "PhysXManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "StaticMeshes\StaticMesh.h"
#include "Utils\Named.h"
#include "Render\RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "Materials\Material.h"
#include "Animation\AnimatedInstanceModel.h"
#include "RenderableObjects\LayerManager.h"
#include "Animation\AnimatedModelManager.h"
#include "Render\DebugRender.h"
#include "Camera\CameraControllerManager.h"
#include "Log\Log.h"

/*Used to create a mesh instance when game is runing*/
CMeshInstance::CMeshInstance(CGameObject* Owner, const std::string &Name, const std::string &CoreName, const Vect3f &Position, float Yaw, float Pitch, float Roll)
:CRenderableObject(Owner, Name, Position, Yaw, Pitch, Roll)
,m_StaticMesh(nullptr)
,m_Parent(nullptr)
,m_ParentBoneId(-1)
,m_TemporalRenderbleObjectTechnique(NULL)
,m_Billboard(false)
{
	m_StaticMesh = CEngine::GetSingleton().GetStaticMeshManager()->GetResource(CoreName);
}

/*Used to create a mesh instance when game starts*/
CMeshInstance::CMeshInstance(CXMLTreeNode &TreeNode)
:CRenderableObject(TreeNode)
,m_StaticMesh(nullptr)
,m_Parent(nullptr)
,m_ParentBoneId(-1)
,m_TemporalRenderbleObjectTechnique(NULL)
,m_Billboard(false)
{
	std::string l_CoreName = TreeNode.GetPszProperty("core_name", "");
	m_StaticMesh = CEngine::GetSingleton().GetStaticMeshManager()->GetResource(l_CoreName);
	#ifdef _DEBUG
	if (m_StaticMesh == nullptr)
	{
		LOG("Error. "+l_CoreName+ " hasn't been loaded");
	}
	#endif
}

void CMeshInstance::GeneratePhysxActor(CXMLTreeNode &TreeNode)
{
	if (m_StaticMesh != nullptr)
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

		if (l_ActorType != "")
		{
			Vect3f Arma_Position;
			float Arma_Yaw;
			float Arma_Pitch;
			float Arma_Roll;

			if (l_ParentName != "" && l_ParentLayer != "" && l_ParentBoneName != "")
			{
				Mat44f l_BoneTransform = m_Parent->GetBoneTransformationMatrix(m_ParentBoneId);
				Mat44f l_ParentTransform = m_Parent->GetTransform();

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
				Mat44f l_MatrixResult = l_Tranform*l_BoneTransform*l_ParentTransform;

				Arma_Position = l_MatrixResult.GetWorldPos();
				Arma_Yaw = l_MatrixResult.GetYaw();
				Arma_Pitch = l_MatrixResult.GetPitch();
				Arma_Roll = l_MatrixResult.GetRoll() - 1.57f;
			}

			bool l_Trigger = TreeNode.GetBoolProperty("trigger", false);
			unsigned int l_Group = TreeNode.GetIntProperty("group", 0);
			float l_Density = TreeNode.GetFloatProperty("density", 1.0f);

			CPhysXManager* l_PhysxManager = CEngine::GetSingleton().GetPhysXManager();

			if (l_ParentName == "")
			{
				l_PhysxManager->AddColliderComponent(m_Name + "_Collider", m_Owner);
			}

			Quatf l_Rotation;
			Vect3f l_Position;
			if (l_ParentName != "")
			{
				l_Position = Arma_Position;
				l_Rotation = Quatf(Arma_Yaw, Arma_Pitch, Arma_Roll);
			}
			else
			{
				l_Position = m_Position;
				l_Rotation = Quatf(m_Yaw, m_Pitch, m_Roll);
			}

			if (l_ActorType == "static")
				l_PhysxManager->CreateStaticActor(m_Name, m_StaticMesh->GetName(), l_Position, l_Rotation);
			else if (l_ActorType == "dynamic")
				l_PhysxManager->CreateDynamicActor(m_Name, m_StaticMesh->GetName(), l_Position, l_Rotation, l_Density, false);
			else if (l_ActorType == "kinematic")
			{
				l_PhysxManager->CreateDynamicActor(m_Name, m_StaticMesh->GetName(), l_Position, l_Rotation, l_Density, true);
			}

			if (l_Trigger)
			{
				l_PhysxManager->ChangeShapeTriggerState(m_StaticMesh->GetName(), true);
			}
		}
	}
}

CMeshInstance::~CMeshInstance()
{
}

void CMeshInstance::Render(CRenderManager* RenderManager)
{
	if (m_StaticMesh == NULL)
		return;

	if (m_Enabled)
	{
		bool l_IsOutsideFrustum = false;

		#if ENABLE_FRUSTUM
			float l_ObjectRadious = m_StaticMesh->GetBoundingSphereRadius();
			if (!RenderManager->GetFrustum().CheckCube(m_Position, l_ObjectRadious))
				l_IsOutsideFrustum = true;
		#endif

		if (!l_IsOutsideFrustum)
		{
			CEngine::GetSingleton().GetLayerManager()->GetResource("solid")->IncrementObjects();

			if (m_Parent != nullptr && m_ParentBoneId != -1)
			{
				Mat44f l_BoneTransform = m_Parent->GetBoneTransformationMatrix(m_ParentBoneId);
				Mat44f l_ParentTransform = m_Parent->GetTransform();
				CContextManager* l_ContextManager = RenderManager->GetContextManager();

				/*l_ContextManager->SetWorldMatrix(l_BoneTransform*l_ParentTransform);
				l_ContextManager->Draw(RenderManager->GetDebugRender()->GetAxis());*/
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
				Mat44f l_Matrix = l_Tranform*l_BoneTransform*l_ParentTransform;
				Vect3f l_Po = l_Matrix.GetWorldPos();
				float l_Y = l_Matrix.GetYaw();
				float l_P = l_Matrix.GetPitch();
				float  l_R = l_Matrix.GetRoll();
				/*	LOG("ESTO EN C");
				LOG(l_Po);
				LOG(l_Y);
				LOG(l_P);
				LOG(l_R);*/

				l_ContextManager->SetWorldMatrix(l_Matrix);
			}
			else
			{
				if (m_Billboard)
				{
					Vect3f l_ObjectPosition = GetPosition();

					CCameraController* l_CurrentCamera = CEngine::GetSingleton().GetCameraControllerManager()->GetCurrentCameraController();
					Vect3f l_CurrentCameraPosition1 = l_CurrentCamera->GetPosition();
					Vect3f l_CurrentCameraPosition = RenderManager->GetCurrentCamera().GetPosition();
					// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
					float angle = atan2(l_ObjectPosition.x - l_CurrentCameraPosition.x, l_ObjectPosition.z - l_CurrentCameraPosition.z) * (180.0f / DirectX::XM_PI);
					//To radians
					float rotation = (float)angle * 0.0174532925f;

					DirectX::XMMATRIX l_WorldMatrix = DirectX::XMMatrixRotationY(rotation);
					DirectX::XMMATRIX l_TranslationMatrix = DirectX::XMMatrixTranslation(l_ObjectPosition.x, l_ObjectPosition.y, l_ObjectPosition.z);
					DirectX::XMMATRIX l_ResultingMatrix = DirectX::XMMatrixMultiply(l_WorldMatrix, l_TranslationMatrix);

					DirectX::XMFLOAT4X4  matrix;

					DirectX::XMStoreFloat4x4(&matrix, l_ResultingMatrix);
					Mat44f l_Result;

					l_Result.m00 = matrix._11;
					l_Result.m01 = matrix._12;
					l_Result.m02 = matrix._13;
					l_Result.m03 = matrix._14;

					l_Result.m10 = matrix._21;
					l_Result.m11 = matrix._22;
					l_Result.m12 = matrix._23;
					l_Result.m13 = matrix._24;

					l_Result.m20 = matrix._31;
					l_Result.m21 = matrix._32;
					l_Result.m22 = matrix._33;
					l_Result.m23 = matrix._34;

					l_Result.m30 = matrix._41;
					l_Result.m31 = matrix._42;
					l_Result.m32 = matrix._43;
					l_Result.m33 = matrix._44;

					RenderManager->GetContextManager()->SetWorldMatrix(l_Result);
				}
				else
				{
					RenderManager->GetContextManager()->SetWorldMatrix(GetTransform());
				}

			}
			if (!m_TemporalRenderbleObjectTechnique)
			{
				m_StaticMesh->Render(RenderManager);
			}
			else
			{
				m_StaticMesh->Render(RenderManager, m_TemporalRenderbleObjectTechnique);
			}
		}
	}
}

void CMeshInstance::SetTemporalRenderableObjectTechnique(CRenderableObjectTechnique* RenderableObjectTechnique)
{
	m_TemporalRenderbleObjectTechnique = RenderableObjectTechnique;
}



void CMeshInstance::SetParent(CAnimatedInstanceModel* Parent, const std::string &BoneName)
{
	m_Parent = Parent;
	if (Parent != nullptr)
	{
		int l_ParentBoneId = m_Parent->GetAnimatedCoreModel()->GetBoneId(BoneName);
		if (l_ParentBoneId != -1)
			m_ParentBoneId = l_ParentBoneId;
	}
	else { m_ParentBoneId = -1;}
}

