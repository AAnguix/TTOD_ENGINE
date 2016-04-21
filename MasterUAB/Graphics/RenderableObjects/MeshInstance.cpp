#include "RenderableObjects\MeshInstance.h"
#include "Engine.h"
#include "PhysXManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "StaticMeshes\StaticMesh.h"
#include "Utils\Named.h"
#include "Render\RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "Materials\Material.h"
#include "Components\PhysxComponent.h"
#include "Components\ComponentManager.h"

CMeshInstance::CMeshInstance(const std::string &Name, const std::string &CoreName)
{
	SetName(Name);
	m_StaticMesh = CEngine::GetSingleton().GetStaticMeshManager()->GetResource(CoreName);
}

CMeshInstance::CMeshInstance(CXMLTreeNode &TreeNode)
{
	std::string l_Name = TreeNode.GetPszProperty("name");
	m_Name=l_Name;
	m_Visible = TreeNode.GetBoolProperty("visible",true);
	std::string l_Core = TreeNode.GetPszProperty("core_name","");
	m_StaticMesh = CEngine::GetSingleton().GetStaticMeshManager()->GetResource(l_Core);
	std::string l_ActorType = TreeNode.GetPszProperty("actor_type","");
	std::string l_Geometry = TreeNode.GetPszProperty("geometry","");

	m_Position=TreeNode.GetVect3fProperty("pos",v3fZERO);
	SetYaw(TreeNode.GetFloatProperty("yaw",0.0f));
	SetPitch(TreeNode.GetFloatProperty("pitch",0.0f));
	SetRoll(TreeNode.GetFloatProperty("roll",0.0f));
	
	if(m_StaticMesh!=nullptr)
	{
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
				bool l_IsKinematic = false;
				if (l_ActorType == "kinematic")
					l_IsKinematic = true;
				float l_Density = TreeNode.GetFloatProperty("density", 1.0f);

				bool l_AbleToBeTrigger = true;

				CPhysxComponent::CreatePhysxComponent("Physyx", this, false);
				CEngine::GetSingleton().GetPhysXManager()->RegisterMaterial(l_MaterialName, 30.0f, 40.0f, 0.0f);

				//Quatf l_Rotation(0.0f, 0.0f, 0.0f, 1.0f);
				Quatf l_Rotation(m_Yaw, m_Pitch, m_Roll);

				if (l_Geometry == "plane")
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
						CEngine::GetSingleton().GetPhysXManager()->CreateRigidStaticConvexMesh(l_Name, m_StaticMesh->GetVertex(), l_MaterialName, m_Position, l_Rotation, l_Group);
					else if (l_ActorType == "kinematic" || l_ActorType == "dynamic")
						CEngine::GetSingleton().GetPhysXManager()->CreateRigidDynamicConvexMesh(l_Name, m_StaticMesh->GetVertex(), l_MaterialName, m_Position, l_Rotation, l_Group, l_Density, l_IsKinematic);
				}
				else if (l_Geometry == "triangle")
				{
					l_AbleToBeTrigger = false;
					if (l_ActorType == "static")
						CEngine::GetSingleton().GetPhysXManager()->CreateRigidStaticTriangleMesh(l_Name, m_StaticMesh->GetVertex(), m_StaticMesh->GetIndex(), l_MaterialName, m_Position, l_Rotation, l_Group);
					else if (l_ActorType == "kinematic")
						CEngine::GetSingleton().GetPhysXManager()->CreateRigidKinematicTriangleMesh(l_Name, m_StaticMesh->GetVertex(), m_StaticMesh->GetIndex(), l_MaterialName, m_Position, l_Rotation, l_Group, l_Density);
				}
				else assert(false);

				if (l_Trigger)
				{
					assert(l_AbleToBeTrigger);
					CEngine::GetSingleton().GetPhysXManager()->SetShapeAsTrigger(l_Name);
				}
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

	m_ComponentManager->Render(*RenderManager);

	//m_Position=CEngine::GetSingleton().GetPhysXManager()->GetActorPosition(m_Name); /*TODO VER SI FUNCIONA*/
	//Quatf q = Quatf(0.0f,0.0f,0.0f,1.0f);


	/*Set matrices acording Position,Yaw,Pitch,Roll*/
	if (m_Visible)
	{
		RenderManager->GetContextManager()->SetWorldMatrix(GetTransform());
		m_StaticMesh->Render(RenderManager);
	}
}
