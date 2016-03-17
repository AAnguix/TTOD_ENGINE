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
	std::string l_Core = TreeNode.GetPszProperty("core_name","");
	m_StaticMesh = CEngine::GetSingleton().GetStaticMeshManager()->GetResource(l_Core);
	std::string l_ActorType = TreeNode.GetPszProperty("actor_type","");
	std::string l_Geometry = TreeNode.GetPszProperty("geometry","");
	bool l_Trigger = TreeNode.GetBoolProperty("trigger", false);

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
		if(l_MaterialName!="")
		{
			CPhysxComponent::CreatePhysxComponent("Physyx",this,false);
			CEngine::GetSingleton().GetPhysXManager()->RegisterMaterial(l_MaterialName,30.0f,40.0f,0.0f);
			float l_Density = 0.3f;
			//Quatf l_Rotation(0.0f, 0.0f, 0.0f, 1.0f);
			Quatf l_Rotation(m_Yaw, m_Pitch, m_Roll);
			
			if(l_ActorType=="static")
			{
				if(l_Geometry=="plane")
				{
					//CEngine::GetSingleton().GetPhysXManager()->CreateRigidStatic(l_Name, m_StaticMesh->GetBoundingBoxSize(), m_Position, l_Rotation, l_MaterialName);
					CEngine::GetSingleton().GetPhysXManager()->CreatePlane(l_Name,Vect3f(0.0f,1.0f,0.0f),0.0f,m_Position,l_Rotation,l_MaterialName);
					
				}else if(l_Geometry=="box")
				{
					CEngine::GetSingleton().GetPhysXManager()->CreateRigidStatic(l_Name,m_StaticMesh->GetBoundingBoxSize(),m_Position,l_Rotation,l_MaterialName);
				
				}else if(l_Geometry=="sphere")
				{
					CEngine::GetSingleton().GetPhysXManager()->CreateRigidStatic(l_Name,m_StaticMesh->GetBoundingSphereRadius(),m_Position,l_Rotation,l_MaterialName);
				}
			}
			else if (l_ActorType=="dynamic")
			{

				/*if(l_Geometry=="box")
					CEngine::GetSingleton().GetPhysXManager()->CreateRigidDynamicBox(l_Name,m_StaticMesh->GetBoundingBoxSize(),m_Position,l_Rotation,l_MaterialName,1,l_Density,false);
				else if(l_Geometry=="sphere")
					CEngine::GetSingleton().GetPhysXManager()->CreateRigidDynamicSphere(l_Name,m_StaticMesh->GetBoundingSphereRadius(),m_Position,l_Rotation,l_MaterialName,1,l_Density,false);
					*/
			}
			else if (l_ActorType=="kinematic")
			{
				/*if(l_Geometry=="box")
					CEngine::GetSingleton().GetPhysXManager()->CreateRigidDynamicBox(l_Name,m_StaticMesh->GetBoundingBoxSize(),m_Position,l_Rotation,l_MaterialName,1,l_Density,true);
				else if(l_Geometry=="sphere")
					CEngine::GetSingleton().GetPhysXManager()->CreateRigidDynamicSphere(l_Name,m_StaticMesh->GetBoundingSphereRadius(),m_Position,l_Rotation,l_MaterialName,1,l_Density,true);
				*/
			}
			else
			{
				//CEngine::GetSingleton().GetPhysXManager()->CreateRigidStatic(l_Name, m_StaticMesh->GetBoundingBoxSize(), m_Position, l_Rotation, l_MaterialName);
				//CEngine::GetSingleton().GetPhysXManager()->CreateSTBOX(l_Name, m_StaticMesh->GetBoundingBoxSize(), l_MaterialName, m_Position, l_Rotation, 1);
				CEngine::GetSingleton().GetPhysXManager()->CreateConvexMesh(m_StaticMesh->GetConvexMeshVertices(),l_Name,m_Position,l_Rotation,l_MaterialName);
				if (l_Trigger)
					CEngine::GetSingleton().GetPhysXManager()->SetShapeAsTrigger(l_Name);
			}
		}
	}
}

CMeshInstance::~CMeshInstance()
{
}

void CMeshInstance::Render(CRenderManager* RenderManager)
{
	if(m_StaticMesh==NULL)
		return;

	m_ComponentManager->Render(*RenderManager);

	//m_Position=CEngine::GetSingleton().GetPhysXManager()->GetActorPosition(m_Name); /*TODO VER SI FUNCIONA*/
	//Quatf q = Quatf(0.0f,0.0f,0.0f,1.0f);


	/*Set matrices acording Position,Yaw,Pitch,Roll*/
	RenderManager->GetContextManager()->SetWorldMatrix(GetTransform());
	m_StaticMesh->Render(RenderManager);
}
