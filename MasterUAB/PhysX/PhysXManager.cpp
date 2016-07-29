#include "PhysXManager.h"
#include <PxPhysicsAPI.h>

#include <sstream>
#include "Utils\FileUtils.h"
#include "GameObject\LuaGameObjectHandle.h"
#include "GameObject\GameObject.h"
#include "Components\Collider.h"
#include "Components\CharacterCollider.h"
#include "RenderableObjects\MeshInstance.h"
#include "Animation\AnimatedInstanceModel.h"
#include "Engine\Engine.h"
#include "Log\Log.h"
#include "Materials\Material.h"

class CPhysxManagerImplementation;

#define PHYSX_UPDATE_STEP (1.0f/30.0f)

inline physx::PxVec3 CastVec(const Vect3f& v)
{
	return physx::PxVec3(v.x, v.y, v.z);
}

inline Vect3f CastVec(const physx::PxVec3& v)
{
	return Vect3f(v.x, v.y, v.z);
}

inline Vect3f CastVec(const physx::PxExtendedVec3& v)
{
	return Vect3f((float)v.x, (float)v.y, (float)v.z);
}

inline physx::PxQuat CastQuat(const Quatf& q)
{
	return physx::PxQuat(q.x, q.y, q.z, q.w);
}

inline Quatf CastQuat(const physx::PxQuat& q)
{
	return Quatf(q.x, q.y, q.z, q.w);
}

CPhysXManager::CPhysXManager()
{
	m_LeftOverSeconds = 0.0f;
}

CPhysXManager::~CPhysXManager()
{
	
}

CPhysXManager::Groups CPhysXManager::GetGroup(const std::string &Group)
{
	if (Group == "GROUP1")
		return GROUP1;
	else if (Group == "GROUP2")
		return GROUP2;
	else if (Group == "GROUP3")
		return GROUP3;
	else if (Group == "GROUP4")
		return GROUP4;
	else if (Group == "GROUP5")
		return GROUP5;
	else if (Group == "GROUP6")
		return GROUP6;
	else if (Group == "GROUP7")
		return GROUP7;
	else if (Group == "GROUP8")
		return GROUP8;
	else 
	{
		return GROUP1;
	}
}

/*Adds colliders from LUA*/
CCollider* CPhysXManager::AddColliderComponent(const std::string &Name, CLuaGameObjectHandle* Owner)
{
	bool l_Found = false;
	CCollider* l_Collider = nullptr;
	for (size_t i = 0; i < m_ColliderComponents.size(); ++i)
	{
		if (m_ColliderComponents[i]->GetName() == Name)
		{
			l_Found = true;
			i = m_ColliderComponents.size();
		}
	}
	if (!l_Found)
	{
		CGameObject* l_GObject = Owner->GetGameObject();
		l_Collider = new CCollider(Name, l_GObject);
		l_GObject->SetCollider(l_Collider);
		m_ColliderComponents.push_back(l_Collider);
	}
	else
	{
		assert(false);
		delete(l_Collider); l_Collider = NULL;
	}

	return l_Collider;
}

/*Adds colliders from C++*/
CCollider* CPhysXManager::AddColliderComponent(const std::string &Name, CGameObject* Owner)
{
	bool l_Found = false;
	CCollider* l_Collider = nullptr;
	for (size_t i = 0; i < m_ColliderComponents.size(); ++i)
	{
		if (m_ColliderComponents[i]->GetName() == Name)
		{
			l_Found = true;
			i = m_ColliderComponents.size();
		}
	}
	if (!l_Found)
	{
		l_Collider = new CCollider(Name, Owner);
		Owner->SetCollider(l_Collider);
		m_ColliderComponents.push_back(l_Collider);
	}
	else
	{
		assert(false);
		delete(l_Collider); l_Collider = NULL;
	}

	return l_Collider;
}


void CPhysXManager::RemoveColliderComponent(CGameObject *Owner)
{
	for (size_t i = 0; i < m_ColliderComponents.size(); ++i)
	{
		if (m_ColliderComponents[i]->GetOwner() == Owner)
		{
			delete m_ColliderComponents[i];
			m_ColliderComponents.erase(m_ColliderComponents.begin() + i);
			RemoveActor(Owner->GetName());
		}
	}
}

CCharacterCollider* CPhysXManager::AddCharacterColliderComponent(const std::string &Name, CLuaGameObjectHandle* Owner, float Height, float Radius, float Density)
{
	bool l_Found = false;
	CCharacterCollider* l_CharacterCollider = nullptr;
	for (size_t i = 0; i < m_CharacterColliderComponents.size(); ++i)
	{
		if (m_CharacterColliderComponents[i]->GetName() == Name)
		{
			l_Found = true;
			i = m_CharacterColliderComponents.size();
		}
	}
	if (!l_Found)
	{
		CGameObject* l_GObject = Owner->GetGameObject();
		l_CharacterCollider = new CCharacterCollider(Name, l_GObject);
		l_GObject->SetCharacterCollider(l_CharacterCollider);
		m_CharacterColliderComponents.push_back(l_CharacterCollider);
		CMaterial* l_PhysxMaterial = l_CharacterCollider->GetPhysxMaterial();

		Vect3f l_Position = l_GObject->GetRenderableObject()->GetPosition();
		CreateCharacterController(Owner->GetName(), Height, Radius, Density, l_Position, l_PhysxMaterial->GetName(), l_PhysxMaterial->GetStaticFriction(), l_PhysxMaterial->GetDynamicFriction(), l_PhysxMaterial->GetRestitution());
	}
	else
	{
		#ifdef _DEBUG
			CEngine::GetSingleton().GetLogManager()->Log("Unable to create component " + Name + ". It's already created");
		#endif
		delete(l_CharacterCollider); 
		l_CharacterCollider = NULL;
	}

	return l_CharacterCollider;
}


void CPhysXManager::RemoveCharacterColliderComponent(CGameObject *Owner)
{
	for (size_t i = 0; i < m_CharacterColliderComponents.size(); ++i)
	{
		if (m_CharacterColliderComponents[i]->GetOwner() == Owner)
		{
			delete m_CharacterColliderComponents[i];
			m_CharacterColliderComponents.erase(m_CharacterColliderComponents.begin() + i);
			RemoveActor(Owner->GetName());
		}
	}
}


physx::PxMaterial* CPhysXManager::RegisterMaterial(const std::string &Name, float StaticFriction, float DynamicFriction, float Restitution)
{
	auto it = m_Materials.find(Name);

	if (it != m_Materials.end())
	{
		it->second->setStaticFriction(StaticFriction);
		it->second->setDynamicFriction(DynamicFriction);
		it->second->setRestitution(Restitution);
	}

	m_Materials[Name] = m_PhysX->createMaterial(StaticFriction, DynamicFriction, Restitution);
	return m_Materials[Name];
}

physx::PxMaterial* CPhysXManager::GetMaterial(const std::string &MaterialName)
{
	auto it = m_Materials.find(MaterialName);

	if (it != m_Materials.end())
	{
		return it->second;
	}

	return nullptr;
}

void CPhysXManager::RegisterActor(const std::string &ActorName, physx::PxShape* Shape, physx::PxRigidActor* Body, Vect3f Position, Quatf Orientation)
{
	Body->attachShape(*Shape);
	m_Scene->addActor(*Body);
	Body->userData = (void*)AddActor(ActorName, Position, Orientation, Body);
}

void CPhysXManager::RegisterActor(const std::string &ActorName, physx::PxShape* Shape, physx::PxRigidBody* Body, Vect3f Position, Quatf Orientation, float Density, bool IsKinematic)
{
	Body->attachShape(*Shape);
	physx::PxRigidBodyExt::updateMassAndInertia(*Body, Density);
	Body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, IsKinematic);

	Body->userData = (void*)AddActor(ActorName, Position, Orientation, Body);
	m_Scene->addActor(*Body);
}

size_t CPhysXManager::AddActor(const std::string &ActorName, const Vect3f &Position, const Quatf &Orientation, physx::PxActor* Actor)
{
	#ifdef USE_PHYSX_DEBUG
		CheckMapAndVectors();
	#endif

	size_t index = m_Actors.size();

	Actor->userData = (void*)index;

	m_ActorIndexs[ActorName] = index;
	m_ActorNames.push_back(ActorName);
	m_ActorPositions.push_back(Position);
	m_ActorOrientations.push_back(Orientation);
	m_Actors.push_back(Actor);

	return m_ActorIndexs[ActorName];
}

bool CPhysXManager::CreateStaticActor(const std::string &ActorName, const std::string &ShapeName, const Vect3f &Position, const Quatf &Orientation)
{
	physx::PxShape* l_Shape = GetShape(ShapeName);
	assert(l_Shape != nullptr);
	if (l_Shape != nullptr)
	{
		physx::PxRigidStatic* l_Body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(Position), CastQuat(Orientation)));
		RegisterActor(ActorName, l_Shape, l_Body, Position, Orientation);
		return true;
	}
	CEngine::GetSingleton().GetLogManager()->Log("Error creating static actor " + ActorName);
	return false;
}

bool CPhysXManager::CreateDynamicActor(const std::string &ActorName, const std::string &ShapeName, const Vect3f &Position, const Quatf &Orientation, float Density, bool IsKinematic)
{
	physx::PxShape* l_Shape = GetShape(ShapeName);
	assert(l_Shape != nullptr);
	if (l_Shape != nullptr)
	{
		physx::PxRigidDynamic* l_Body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(Position), CastQuat(Orientation)));
		RegisterActor(ActorName, l_Shape, l_Body, Position, Orientation, Density, IsKinematic);
		return true;
	}
	CEngine::GetSingleton().GetLogManager()->Log("Error creating dynamic actor "+ActorName);
	return false;
}

bool CPhysXManager::CreateBoxTrigger(const std::string &ActorName, const std::string &ShapeName, const Vect3f &Size, const std::string &MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, const Vect3f &Position, const Quatf &Orientation, const std::string &ActorType)
{
	physx::PxShape* l_Shape = CreateBox(ShapeName, Size,MaterialName,MaterialStaticFriction,MaterialDynamicFriction,MaterialRestitution,Group,true);
	
	if (ActorType == "static")
		return CreateStaticActor(ActorName, ShapeName, Position, Orientation);
	else if (ActorType == "dynamic")
		return CreateDynamicActor(ActorName, ShapeName, Position, Orientation, 5.0, false);
	else if (ActorType == "kinematic")
		return CreateDynamicActor(ActorName, ShapeName, Position, Orientation, 5.0, true);
	else return CreateStaticActor(ActorName, ShapeName, Position, Orientation);
}
bool CPhysXManager::CreateBoxTrigger(const std::string &ActorName, const std::string &ShapeName, const Vect3f &Size, const std::string &MaterialName, const std::string &Group, const Vect3f &Position, const Quatf &Orientation, const std::string &ActorType)
{
	physx::PxShape* l_Shape = CreateBox(ShapeName, Size, MaterialName, 10.0, 20.0, 1.0, Group, true);

	if (ActorType == "static")
		return CreateStaticActor(ActorName, ShapeName, Position, Orientation);
	else if (ActorType == "dynamic")
		return CreateDynamicActor(ActorName, ShapeName, Position, Orientation, 5.0, false);
	else if (ActorType == "kinematic")
		return CreateDynamicActor(ActorName, ShapeName, Position, Orientation, 5.0, true);
	else return CreateStaticActor(ActorName, ShapeName, Position, Orientation);
}
bool CPhysXManager::CreateSphereTrigger(const std::string &ActorName, const std::string &ShapeName, float Radius, const std::string &MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, const Vect3f &Position, const Quatf &Orientation, const std::string &ActorType)
{
	physx::PxShape* l_Shape = CreateSphere(ShapeName, Radius, MaterialName, MaterialStaticFriction, MaterialDynamicFriction, MaterialRestitution, Group, true);

	if (ActorType == "static")
		return CreateStaticActor(ActorName, ShapeName, Position, Orientation);
	else if (ActorType == "dynamic")
		return CreateDynamicActor(ActorName, ShapeName, Position, Orientation, 5.0, false);
	else if (ActorType == "kinematic")
		return CreateDynamicActor(ActorName, ShapeName, Position, Orientation, 5.0, true);
	else return CreateStaticActor(ActorName, ShapeName, Position, Orientation);
}

bool CPhysXManager::CreateSphereTrigger(const std::string &ActorName, const std::string &ShapeName, float Radius, const std::string &MaterialName, const std::string &Group, const Vect3f &Position, const Quatf &Orientation, const std::string &ActorType)
{
	physx::PxShape* l_Shape = CreateSphere(ShapeName, Radius, MaterialName, 10.0, 20.0, 1.0, Group, true);

	if (ActorType == "static")
		return CreateStaticActor(ActorName, ShapeName, Position, Orientation);
	else if (ActorType == "dynamic")
		return CreateDynamicActor(ActorName, ShapeName, Position, Orientation, 5.0, false);
	else if (ActorType == "kinematic")
		return CreateDynamicActor(ActorName, ShapeName, Position, Orientation, 5.0, true);
	else return CreateStaticActor(ActorName, ShapeName, Position, Orientation);
}

/*Meshes that need to be cooked*/
physx::PxConvexMesh*  CPhysXManager::CookConvexMesh(const std::string &CoreName, std::vector<Vect3f> Vertices)
{
	#if USE_PHYSX_DEBUG
		CheckMapAndVectors();
	#endif

	std::string l_CookedMeshFilename;
	bool l_CookedMeshExists = CFileUtils::CookedMeshExists(false, CoreName, 1, l_CookedMeshFilename);
	physx::PxConvexMesh* l_ConvexMesh = nullptr;

	if ((l_CookedMeshExists && CFileUtils::MeshFileModified(false, CoreName, 1)) || (!l_CookedMeshExists))
	{
		physx::PxConvexMeshDesc l_ConvexDesc;
		l_ConvexDesc.points.count = Vertices.size();
		l_ConvexDesc.points.stride = sizeof(Vect3f);
		l_ConvexDesc.points.data = &Vertices[0];
		l_ConvexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

		physx::PxDefaultMemoryOutputStream l_Buffer;
		physx::PxConvexMeshCookingResult::Enum l_Result;
		bool l_Success = m_Cooking->cookConvexMesh(l_ConvexDesc, l_Buffer, &l_Result);
		if (!l_Success)
		{
			CEngine::GetSingleton().GetLogManager()->Log("Error cooking convex mesh " + CoreName);
		}
		if (!l_Success)
		{
			l_ConvexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX | physx::PxConvexFlag::eINFLATE_CONVEX;
			l_Success = m_Cooking->cookConvexMesh(l_ConvexDesc, l_Buffer, &l_Result);
			if (!l_Success)
			{
				CEngine::GetSingleton().GetLogManager()->Log("Error cooking inflated convex mesh " + CoreName);
				assert(false);
			}
		}

		WriteCookingDataToFile(l_CookedMeshFilename, (void *)l_Buffer.getData(), l_Buffer.getSize());

		physx::PxDefaultMemoryInputData l_Input(l_Buffer.getData(), l_Buffer.getSize());
		l_ConvexMesh = GetConvexMesh(CoreName, l_Input);
	}
	else
	{
		unsigned int l_DataSize;
		void *l_Data;
		ReadCookingDataFromFile(l_CookedMeshFilename, &l_Data, l_DataSize);

		physx::PxDefaultMemoryInputData l_Input((physx::PxU8*)l_Data, l_DataSize);
		l_ConvexMesh = GetConvexMesh(CoreName, l_Input);
		free(l_Data);
	}

	return l_ConvexMesh;
}
//physx::PxTriangleMesh*  CPhysXManager::CreateTriangleMesh(const std::string &FileName, std::vector<Vect3f> Vertices, std::vector<unsigned int> Indices)
//{
//	#if USE_PHYSX_DEBUG
//		CheckMapAndVectors();
//	#endif
//
//	/*Faster cooking*/
//	//physx::PxTolerancesScale l_Scale;
//	//physx::PxCookingParams l_Params(l_Scale);
//	//// disable mesh cleaning - perform mesh validation on development configurations
//	//l_Params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
//	//// disable edge precompute, edges are set for each triangle, slows contact generation
//	//l_Params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
//	//// lower hierarchy for internal mesh
//	//l_Params.meshCookingHint = physx::PxMeshCookingHint::eCOOKING_PERFORMANCE;
//	//m_Cooking->setParams(l_Params);
//
//
//	physx::PxTriangleMeshDesc l_TriangleDesc;
//	l_TriangleDesc.points.count = Vertices.size();
//	l_TriangleDesc.points.stride = sizeof(Vect3f);
//	l_TriangleDesc.points.data = &Vertices[0];
//
//	l_TriangleDesc.triangles.count = Indices.size() - 2;
//	l_TriangleDesc.triangles.stride = 3 * sizeof(physx::PxU32);
//	l_TriangleDesc.triangles.data = &Indices[0];
//
//	//#if USE_PHYSX_DEBUG
//	//	// mesh should be validated before cooked without the mesh cleaning
//	//	bool res = m_Cooking->validateTriangleMesh(l_TriangleDesc);
//	//	PX_ASSERT(res);
//	//#endif
//
//	physx::PxDefaultMemoryOutputStream l_Buffer;
//	bool success = m_Cooking->cookTriangleMesh(l_TriangleDesc, l_Buffer);
//	assert(success);
//	//WriteCookingDataToFile("cocinando", l_Buffer);
//
//	std::string l_Cooked = CFileUtils::GetCookedMeshFileName(FileName,1);
//	std::string l_Mesh = CFileUtils::GetMeshFileName(FileName, 1);
//
//	_int64 i = CFileUtils::Delta(CFileUtils::GetFileCreationDate(l_Mesh), CFileUtils::GetFileCreationDate(l_Cooked));
//
//	if (i < 0)
//	{
//
//	}
//
//	unsigned int l_DataORiginalSize = l_Buffer.getSize();
//	WriteCookingDataToFile("D:\\a_physics.mesh", (void *)l_Buffer.getData(), l_Buffer.getSize());
//
//	unsigned int l_DataSize;
//	void *l_Data;
//	ReadCookingDataFromFile("D:\\a_physics.mesh", &l_Data, l_DataSize);
//
//	//physx::PxDefaultMemoryInputData l_Input(l_Buffer.getData(), l_Buffer.getSize());
//	physx::PxDefaultMemoryInputData l_Input((physx::PxU8 *)l_Data, l_DataSize);
//
//	physx::PxTriangleMesh* l_TriangleMesh = m_PhysX->createTriangleMesh(l_Input);
//
//	free(l_Data);
//
//	return l_TriangleMesh;
//}

physx::PxTriangleMesh*  CPhysXManager::CookTriangleMesh(const std::string &CoreName, std::vector<Vect3f> &Vertices, std::vector<unsigned short> &Indices)
{
	#if USE_PHYSX_DEBUG
		CheckMapAndVectors();
	#endif

	/*Faster cooking*/
	//physx::PxTolerancesScale l_Scale;
	//physx::PxCookingParams l_Params(l_Scale);
	//// disable mesh cleaning - perform mesh validation on development configurations
	//l_Params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
	//// disable edge precompute, edges are set for each triangle, slows contact generation
	//l_Params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
	//// lower hierarchy for internal mesh
	//l_Params.meshCookingHint = physx::PxMeshCookingHint::eCOOKING_PERFORMANCE;
	//m_Cooking->setParams(l_Params);

	std::string l_CookedMeshFileName;
	bool l_CookedMeshExists = CFileUtils::CookedMeshExists(true, CoreName, 1, l_CookedMeshFileName);
	physx::PxTriangleMesh* l_TriangleMesh = nullptr;

	/*Mesh never has been cooked or has been cooked but not need to be updated*/
	if ((l_CookedMeshExists && CFileUtils::MeshFileModified(true, CoreName, 1)) || (!l_CookedMeshExists))
	{
		/*
		physx::PxTolerancesScale l_Scale;
		physx::PxCookingParams params(l_Scale);
		params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
		params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
		params.meshCookingHint = physx::PxMeshCookingHint::eCOOKING_PERFORMANCE;
		m_Cooking->setParams(params);
		*/

		physx::PxTriangleMeshDesc l_TriangleDesc;
		l_TriangleDesc.flags = physx::PxMeshFlag::e16_BIT_INDICES;
		l_TriangleDesc.points.count = Vertices.size();
		l_TriangleDesc.points.stride = sizeof(Vect3f);
		l_TriangleDesc.points.data = &Vertices[0].x;

		l_TriangleDesc.triangles.count = Indices.size() / 3;
		l_TriangleDesc.triangles.stride = 3 * sizeof(physx::PxU16);
		l_TriangleDesc.triangles.data = &Indices[0];

		//#if USE_PHYSX_DEBUG
		//	// mesh should be validated before cooked without the mesh cleaning
		//bool res = m_Cooking->validateTriangleMesh(l_TriangleDesc);
		//	PX_ASSERT(res);
		//#endif

		physx::PxDefaultMemoryOutputStream l_Buffer;
		bool success = m_Cooking->cookTriangleMesh(l_TriangleDesc, l_Buffer);
		assert(success);

		unsigned int l_DataORiginalSize = l_Buffer.getSize();
		WriteCookingDataToFile(l_CookedMeshFileName, (void *)l_Buffer.getData(), l_Buffer.getSize());

		physx::PxDefaultMemoryInputData l_Input(l_Buffer.getData(), l_Buffer.getSize());
		l_TriangleMesh = GetTriangleMesh(CoreName,l_Input);
	}
	else
	{
		unsigned int l_DataSize;
		void *l_Data;
		ReadCookingDataFromFile(l_CookedMeshFileName, &l_Data, l_DataSize);

		physx::PxDefaultMemoryInputData l_Input((physx::PxU8 *)l_Data, l_DataSize);
		l_TriangleMesh = GetTriangleMesh(CoreName, l_Input);
		free(l_Data);
	}

	return l_TriangleMesh;
}

//void CPhysXManager::CreatePlane(const std::string &Name, const Vect3f &Normal, float Distance, const Vect3f &Position, const Quatf &Orientation, const std::string &MaterialName)
//{
//	CheckMapAndVectors();
//	physx::PxMaterial* l_Material = GetMaterial(MaterialName);
//
//	physx::PxRigidStatic* groundPlane = PxCreatePlane(*m_PhysX, physx::PxPlane(CastVec(Normal), Distance), *l_Material);
//	size_t index = m_Actors.size();
//	groundPlane->userData = (void*)index;
//	m_Scene->addActor(*groundPlane);
//
//	physx::PxShape* shape;
//	size_t numShapes = groundPlane->getShapes(&shape, 1);
//	assert(numShapes == 1);
//
//	SaveActorData(index, Name, Position, Orientation, groundPlane);
//}


size_t CPhysXManager::GetActorIndex(const std::string& ActorName) const
{
	auto it = m_ActorIndexs.find(ActorName);
	
	if (it == m_ActorIndexs.end())
	{
		#ifdef _DEBUG
			CEngine::GetSingleton().GetLogManager()->Log("Can't find actor " + ActorName + " index");
		#endif
		assert(false);
	}
	return it->second;

	/*assert(it != m_ActorIndexs.end());
	return it->second;*/

	/*auto it = m_ActorIndexs.find(ActorName);
	if (it == m_ActorIndexs.end());
	{

	}
	*/
}

void CPhysXManager::SaveActorData(size_t Index, const std::string ActorName, const Vect3f &Position, const Quatf &Orientation, physx::PxActor *Actor)
{
	m_ActorIndexs[ActorName] = Index;
	m_ActorNames.push_back(ActorName);
	m_ActorPositions.push_back(Position);
	m_ActorOrientations.push_back(Orientation);
	m_Actors.push_back(Actor);
}

void CPhysXManager::CheckMapAndVectors()
{
	if (m_Actors.size() != m_ActorNames.size() || m_Actors.size() != m_ActorPositions.size() || m_Actors.size() != m_ActorOrientations.size() || m_Actors.size() != m_ActorIndexs.size())
	{
		#ifdef _DEBUG
			CEngine::GetSingleton().GetLogManager()->Log("Actor vectors size doesn't match");
		#endif
		assert(false);
	}
}

void CPhysXManager::Update(float ElapsedTime)
{
	m_LeftOverSeconds += ElapsedTime;

	if (m_LeftOverSeconds >= PHYSX_UPDATE_STEP)
	{
		m_Scene->simulate(PHYSX_UPDATE_STEP);
		m_Scene->fetchResults(true);

		m_LeftOverSeconds = fmod(m_LeftOverSeconds, PHYSX_UPDATE_STEP);

		physx::PxU32 l_NumActiveTransforms;
		const physx::PxActiveTransform* activeTransforms = m_Scene->getActiveTransforms(l_NumActiveTransforms);

		for (physx::PxU32 i = 0; i<l_NumActiveTransforms; ++i)
		{
			uintptr_t index = (uintptr_t)(activeTransforms[i].userData);
			m_ActorPositions[index] = CastVec(activeTransforms[i].actor2World.p);
			m_ActorOrientations[index] = CastQuat(activeTransforms[i].actor2World.q);
		}
	}

	UpdateComponents(ElapsedTime);
}

void CPhysXManager::UpdateComponents(float ElapsedTime)
{
	for (size_t i = 0; i < m_CharacterColliderComponents.size(); ++i)
	{
		m_CharacterColliderComponents[i]->Update(ElapsedTime);
	}
	for (size_t i = 0; i < m_ColliderComponents.size(); ++i)
	{
		m_ColliderComponents[i]->Update(ElapsedTime);
	}
}

bool CPhysXManager::RemoveActor(const std::string &ActorName)
{
	
	auto it_controller = m_CharacterControllers.find(ActorName);
	auto it_Actors = m_ActorIndexs.find(ActorName);

	/*if (it_Actors != m_ActorIndexs.end())
	{
		size_t a = it_Actors->second;
		m_Scene->removeActor(*m_Actors[a]);
	}
	*/

	if (it_controller != m_CharacterControllers.end()) //Is a C.Controller
	{
		it_controller->second->release();
		m_CharacterControllers.erase(it_controller); 
	}

	if (it_Actors != m_ActorIndexs.end())
	{
		size_t l_RemovedActorIndex = it_Actors->second;

		//m_Actors[l_RemovedActorIndex]->release();
		
		m_Scene->removeActor(*m_Actors[l_RemovedActorIndex]);

		size_t l_MovedActorIndex = m_Actors.size() - 1;

		//Coppy last actor info to removed actor index.
		m_Actors[l_RemovedActorIndex] = m_Actors[m_Actors.size() - 1];
		m_Actors.resize(m_Actors.size() - 1);

		m_ActorNames[l_RemovedActorIndex] = m_ActorNames[m_ActorNames.size() - 1];
		m_ActorNames.resize(m_ActorNames.size() - 1);

		m_ActorPositions[l_RemovedActorIndex] = m_ActorPositions[m_ActorPositions.size() - 1];
		m_ActorPositions.resize(m_ActorPositions.size() - 1);

		m_ActorOrientations[l_RemovedActorIndex] = m_ActorOrientations[m_ActorOrientations.size() - 1];
		m_ActorOrientations.resize(m_ActorOrientations.size() - 1);

		m_ActorIndexs.erase(it_Actors);

		if (l_RemovedActorIndex<m_Actors.size())
		{
			m_ActorIndexs[m_ActorNames[l_RemovedActorIndex]] = l_RemovedActorIndex; //indice del obj movido
			m_Actors[l_RemovedActorIndex]->userData = (void*)l_RemovedActorIndex;
		}
		return true;
	}

	return false;
}

void CPhysXManager::CreateCharacterController(const std::string &Name, const float &Height, const float &Radius, const float &Density, const Vect3f &Position, const std::string &MaterialName, float StaticFriction, float DynamicFriction, float Restitution)
{

}

class CFilter : public physx::PxControllerFilterCallback
{
	CPhysXManager *m_PhysXManager;
public:
	CFilter(CPhysXManager *PhysXManager)
	{
		m_PhysXManager = PhysXManager;
	}
	~CFilter()
	{
	}
	bool filter(const physx::PxController& a, const physx::PxController& b)
	{
		void *l_UserData = a.getUserData();
		void *l_UserDataB = b.getUserData();
		size_t l_IdA = (size_t)l_UserData;
		size_t l_IdB = (size_t)l_UserDataB;
		std::vector<std::string> l_Names = m_PhysXManager->GetActorNames();
		std::string l_NameA = "";
		std::string l_NameB = "";

		if (l_IdA < l_Names.size())
			l_NameA = l_Names[l_IdA];
		if (l_IdB < l_Names.size())
			l_NameB = l_Names[l_IdB];

		/*physx::PxRigidDynamic* l_a = a.getActor();

		if (l_a != NULL)
		{
		std::string l_actorA = a.getActor()->getName();
		l_actorA = a.getActor()->getName();
		}*/

		//std::string l_actorB = b.getActor()->getName();
		//physx::PxRigidDynamic* l_Actor = a.getActor();
		if (l_NameA == "Player" || l_NameB == "Player")
			l_NameA = "";
		return true;
	}
};

Vect3f CPhysXManager::MoveCharacterController(const std::string& CharacterControllerName, const Vect3f &Movement, float ElapsedTime)
{
	static CFilter l_Filter(this);
	Vect3f l_Move = v3fZERO;
	if ((!isnan(Movement.x) && !isnan(Movement.y) && !isnan(Movement.z)) && ((!isinf(Movement.x) && !isinf(Movement.y) && !isinf(Movement.z))))
		l_Move = Movement;

	physx::PxController* l_Controller = m_CharacterControllers[CharacterControllerName];
	const physx::PxControllerFilters l_Filters(nullptr, nullptr, &l_Filter);
	
	#ifdef _DEBUG
	if (l_Controller == nullptr)
		CEngine::GetSingleton().GetLogManager()->Log("Unable to move " + CharacterControllerName + ". Is nullptr.");
	#endif

	size_t index = (size_t)l_Controller->getUserData();
	l_Controller->move(CastVec(l_Move), l_Move.Length()*0.01f, ElapsedTime, l_Filters);

	physx::PxRigidDynamic* l_Actor = l_Controller->getActor();

	physx::PxExtendedVec3 l_Pos = l_Controller->getPosition();
	Vect3f l_Return = CastVec(l_Pos);
	//physx::PxVec3 l_V = l_Actor->getLinearVelocity();
	//SCharacterControllerData l_CCData(CastVec(l_FPos), CastVec(l_V));

	m_ActorPositions[GetActorIndex(CharacterControllerName)] = l_Return;
	//return GetActorPosition(CharacterControllerName);

	return l_Return;
}



Vect3f CPhysXManager::DisplacementCharacterController(const std::string& CharacterControllerName, const Vect3f &Displacement, float ElapsedTime)
{
	//Vect3f Acceleration = Vect3f(0.0, -10.0, 0.0);
	//Vect3f l_ResultingVelocity;
	Vect3f l_Position = GetCharacterControllerPosition(CharacterControllerName);
	//Vect3f InitialVelocity = Displacement / ElapsedTime;
	//Vect3f Velocity = l_ResultingVelocity + Displacement;
	//Vect3f result = (Velocity*ElapsedTime) + ((Acceleration*0.5)*(ElapsedTime*ElapsedTime));
	Vect3f l_NewPosition = MoveCharacterController(CharacterControllerName, Displacement, ElapsedTime);
	Vect3f Velocity = (l_NewPosition - l_Position) / ElapsedTime;
	return Velocity;
}

Vect3f CPhysXManager::GetCharacterControllerPosition(const std::string& CharacterControllerName)
{
	Vect3f l_Result = v3fZERO;
	physx::PxController* l_CController = m_CharacterControllers[CharacterControllerName];
	if (l_CController != nullptr)
	{
		l_Result = Vect3f(CastVec(l_CController->getPosition()));
	}
	return l_Result;
}

Vect3f CPhysXManager::GetCharacterControllerFootPosition(const std::string& CharacterControllerName)
{
	Vect3f l_Result = v3fZERO;
	physx::PxController* l_CController = m_CharacterControllers[CharacterControllerName];
	if (l_CController != nullptr)
	{
		l_Result = Vect3f(CastVec(l_CController->getFootPosition()));
	}
	return l_Result;
}

void CPhysXManager::SetShapeAsTrigger(const std::string &ShapeName)
{
	physx::PxShape* l_Shape = GetShape(ShapeName);
	assert(l_Shape != nullptr);

	if (l_Shape != nullptr)
	{
		l_Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		l_Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	}
	//auto it_Actors = m_ActorIndexs.find(ActorName);

	//if (it_Actors != m_ActorIndexs.end())
	//{
	//	size_t l_Index = it_Actors->second;
	//	physx::PxActor* l_PxActor = m_Actors[l_Index];

	//	physx::PxRigidDynamic* l_DynamicActor = l_PxActor->is<physx::PxRigidDynamic>();
	//	physx::PxShape* l_Shape;

	//	if (l_DynamicActor != nullptr)
	//	{
	//		l_DynamicActor->getShapes(&l_Shape, 1);
	//		l_Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
	//		l_Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	//	}
	//	else
	//	{
	//		physx::PxRigidStatic* l_StaticActor = l_PxActor->is<physx::PxRigidStatic>();

	//		if (l_StaticActor != nullptr)
	//		{
	//			l_StaticActor->getShapes(&l_Shape, 1);
	//			l_Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
	//			l_Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	//		}
	//	}
	//}
}

bool CPhysXManager::Raycast(const Vect3f& Origin, const Vect3f& End, int FilterMask, SRaycastData* Result_)
{
	physx::PxFilterData filterData;
	filterData.setToDefault();
	//filterData.word0 = GROUP1 | GROUP2;
	filterData.word0 = 1 | 2;

	Vect3f l_Direction = End - Origin;
	float l_Lenght = l_Direction.Length();
	l_Direction.Normalize(); //normalize

	physx::PxRaycastBuffer l_ReturnBuffer;
	bool status = m_Scene->raycast
	(
	CastVec(Origin), CastVec(l_Direction), l_Lenght,
	l_ReturnBuffer,
	physx::PxHitFlags(physx::PxHitFlag::eDEFAULT),
	physx::PxQueryFilterData(
	filterData,
	physx::PxQueryFlag::eDYNAMIC | physx::PxQueryFlag::eSTATIC)
	);

	Result_->m_Position = CastVec(l_ReturnBuffer.block.position);
	Result_->m_Normal = CastVec(l_ReturnBuffer.block.normal);
	Result_->m_Distance = l_ReturnBuffer.block.distance;
	Result_->m_ActorName = m_ActorNames[(size_t)l_ReturnBuffer.block.actor->userData];

	return true;
}

float CPhysXManager::CameraRaycast(const Vect3f& Origin, const Vect3f& End, const float& Length)
{
	float l_HitDistance = Length;
	Vect3f l_Direction = End - Origin;
	float l_Lenght = l_Direction.Length();
	l_Direction.Normalize(); 

	// [in] Define filter for static objects only
	physx::PxQueryFilterData filterData(physx::PxQueryFlag::eSTATIC);

	physx::PxRaycastBuffer l_ReturnBuffer;
	bool status = m_Scene->raycast
	(
		CastVec(Origin),
		CastVec(l_Direction),
		Length,
		l_ReturnBuffer,
		physx::PxHitFlags(physx::PxHitFlag::eDEFAULT),
		filterData
	);
	if (status)
	{
		Vect3f l_HitPosition = CastVec(l_ReturnBuffer.block.position);
		//std::string l_HitName = m_ActorNames[(size_t)l_ReturnBuffer.block.actor->userData];
		l_HitDistance = l_ReturnBuffer.block.distance;
	}
	return l_HitDistance;
}

void CPhysXManager::MoveKinematicActor(const std::string& ActorName, const Vect3f &Position)
{
	physx::PxActor* l_Actor = IsRigidDynamic(ActorName);
	assert(l_Actor != nullptr);
	((physx::PxRigidDynamic*)l_Actor)->setKinematicTarget(physx::PxTransform(CastVec(Position), CastQuat(GetActorOrientation(ActorName))));
}

void CPhysXManager::MoveKinematicActor(const std::string& ActorName, const Quatf &Rotation)
{
	physx::PxActor* l_Actor = IsRigidDynamic(ActorName);
	assert(l_Actor != nullptr);
	((physx::PxRigidDynamic*)l_Actor)->setKinematicTarget(physx::PxTransform(CastVec(GetActorPosition(ActorName)), CastQuat(Rotation)));
}


void CPhysXManager::MoveKinematicActor(const std::string& ActorName, const Vect3f &Position, const Quatf &Rotation)
{
	physx::PxActor* l_Actor = IsRigidDynamic(ActorName);
	assert(l_Actor != nullptr);
	((physx::PxRigidDynamic*)l_Actor)->setKinematicTarget(physx::PxTransform(CastVec(Position), CastQuat(Rotation)));
}


physx::PxActor* CPhysXManager::IsRigidDynamic(const std::string& ActorName)
{
	auto it_Actors = m_ActorIndexs.find(ActorName);
	size_t l_ActorIndex = it_Actors->second;
	physx::PxActor* l_Actor = m_Actors[l_ActorIndex];
	return l_Actor->isRigidDynamic();
}

void CPhysXManager::Reload()
{

}


void CPhysXManager::WriteCookingDataToFile(const std::string &FileName, void *Data, unsigned int DataSize)
{
	FILE *l_File;
	errno_t l_Error;
	l_Error = fopen_s(&l_File, FileName.c_str(), "wb");
	assert(l_Error == 0);
	if (l_Error == 0)
	{
		fwrite(&DataSize, sizeof(unsigned int), 1, l_File);
		fwrite(Data, sizeof(unsigned char), DataSize, l_File);
	}
	fclose(l_File);
}

void CPhysXManager::ReadCookingDataFromFile(const std::string &FileName, void **Data, unsigned int &DataSize)
{
	FILE *l_File;
	errno_t l_Error;
	l_Error = fopen_s(&l_File, FileName.c_str(), "rb");
	assert(l_Error == 0);
	if (l_Error == 0)
	{		
		fread(&DataSize, sizeof(unsigned int), 1, l_File);
		*Data = malloc(DataSize);
		fread(*Data, sizeof(unsigned char), DataSize, l_File);
	}
	fclose(l_File);
}


void CPhysXManager::ApplyForce(const std::string &ActorName, const Vect3f &Force)
{
	physx::PxVec3 l_Force = CastVec(Force);
	auto it_Actors = m_ActorIndexs.find(ActorName);

	if (it_Actors != m_ActorIndexs.end())
	{
		size_t l_Index = it_Actors->second;
		physx::PxActor* l_PxActor = m_Actors[l_Index];

		physx::PxRigidDynamic* l_DynamicActor = l_PxActor->is<physx::PxRigidDynamic>();

		if (l_DynamicActor != nullptr)
		{
			l_DynamicActor->addForce(l_Force);
		}

	}
}

void CPhysXManager::RemoveTriggerState(const std::string &ActorName)
{
	auto it_Actors = m_ActorIndexs.find(ActorName);

	if (it_Actors != m_ActorIndexs.end())
	{
		size_t l_Index = it_Actors->second;
		physx::PxActor* l_PxActor = m_Actors[l_Index];

		physx::PxRigidDynamic* l_DynamicActor = l_PxActor->is<physx::PxRigidDynamic>();
		physx::PxShape* l_Shape;

		if (l_DynamicActor != nullptr)
		{
			l_DynamicActor->getShapes(&l_Shape, 1);
			l_Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
			l_Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
		}
		else
		{
			physx::PxRigidStatic* l_StaticActor = l_PxActor->is<physx::PxRigidStatic>();

			if (l_StaticActor != nullptr)
			{
				l_StaticActor->getShapes(&l_Shape, 1);
				l_Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
				l_Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
			}
		}
	}
}

void CPhysXManager::ChangeGravityState(const std::string& ActorName, bool State)
{
	physx::PxActor* l_Actor = IsRigidDynamic(ActorName);
	assert(l_Actor != nullptr);
	l_Actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, State);
}

void CPhysXManager::ChangeKinematicState(const std::string& ActorName, bool State)
{

	physx::PxActor* l_Actor = IsRigidDynamic(ActorName);
	assert(l_Actor != nullptr);
	physx::PxRigidDynamic* l_DynamicActor = l_Actor->is<physx::PxRigidDynamic>();
	l_DynamicActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, State);

}

void CPhysXManager::SetMaterialStaticFriction(const std::string &MaterialName, float StaticFriction)
{
	auto it = m_Materials.find(MaterialName);
	if (it != m_Materials.end())
	{
		it->second->setStaticFriction(StaticFriction);
	}
}

void CPhysXManager::SetMaterialDynamicFriction(const std::string &MaterialName, float DynamicFriction)
{
	auto it = m_Materials.find(MaterialName);
	if (it != m_Materials.end())
	{
		it->second->setDynamicFriction(DynamicFriction);
	}
}

void CPhysXManager::SetMaterialRestitution(const std::string &MaterialName, float Restitution)
{
	auto it = m_Materials.find(MaterialName);
	if (it != m_Materials.end())
	{
		it->second->setRestitution(Restitution);
	}
}

physx::PxTriangleMesh* CPhysXManager::GetTriangleMesh(const std::string &TriangleMeshName, physx::PxDefaultMemoryInputData MemoryInputData)
{
	std::map<std::string, physx::PxTriangleMesh*>::iterator itMap;
	itMap = m_TriangleMeshes.find(TriangleMeshName);
	if (itMap != m_TriangleMeshes.end())
	{
		return itMap->second;
	}
	else
	{
		physx::PxTriangleMesh* l_TriangleMesh = m_PhysX->createTriangleMesh(MemoryInputData);
		m_TriangleMeshes.insert(std::pair<std::string, physx::PxTriangleMesh*>(TriangleMeshName, l_TriangleMesh));
		return l_TriangleMesh;
	}
}

physx::PxConvexMesh* CPhysXManager::GetConvexMesh(const std::string &ConvexMeshName, physx::PxDefaultMemoryInputData MemoryInputData)
{
	std::map<std::string, physx::PxConvexMesh*>::iterator itMap;
	itMap = m_ConvexMeshes.find(ConvexMeshName);
	if (itMap != m_ConvexMeshes.end())
	{
		return itMap->second;
	}
	else
	{
		physx::PxConvexMesh* l_ConvexMesh = m_PhysX->createConvexMesh(MemoryInputData);
		m_ConvexMeshes.insert(std::pair<std::string, physx::PxConvexMesh*>(ConvexMeshName, l_ConvexMesh));
		return l_ConvexMesh;
	}
}

/*SHAPES*/
physx::PxShape* CPhysXManager::GetShape(const std::string &ShapeName)
{
	std::map<std::string, physx::PxShape*>::iterator itMap;
	itMap = m_Shapes.find(ShapeName);
	if (itMap != m_Shapes.end())
	{
		return itMap->second;
	}
	else
	{
		#ifdef _DEBUG
			CEngine::GetSingleton().GetLogManager()->Log("Can't find the shape "+ShapeName+". It hasn't been loaded.");
		#endif	
		return nullptr;
	}
}
physx::PxShape* CPhysXManager::GenerateShape(const std::string &ShapeName, const physx::PxGeometry &Geometry, const std::string Material, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, bool IsExclusive)
{
	physx::PxMaterial* l_Material = RegisterMaterial(Material, MaterialStaticFriction,MaterialDynamicFriction,MaterialRestitution);
	physx::PxShape* l_Shape = m_PhysX->createShape(Geometry, *l_Material,IsExclusive);
	m_Shapes.insert(std::pair<std::string, physx::PxShape*>(ShapeName, l_Shape));
	physx::PxFilterData l_FilterData;
	l_FilterData.word0 = GetGroup(Group);
	l_Shape->setQueryFilterData(l_FilterData);
	return l_Shape;
}

physx::PxShape* CPhysXManager::CreateBox(const std::string &ShapeName, const Vect3f &Size, const std::string MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, bool IsExclusive)
{
	return GenerateShape(ShapeName, physx::PxBoxGeometry(Size.x / 2, Size.y / 2, Size.z / 2), MaterialName, MaterialStaticFriction, MaterialDynamicFriction, MaterialRestitution, Group, IsExclusive);
}
physx::PxShape* CPhysXManager::CreateSphere(const std::string &ShapeName, float Radius, const std::string MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, bool IsExclusive)
{
	return GenerateShape(ShapeName, physx::PxSphereGeometry(Radius), MaterialName, MaterialStaticFriction, MaterialDynamicFriction, MaterialRestitution, Group, IsExclusive);
}
physx::PxShape* CPhysXManager::CreateCapsule(const std::string &ShapeName, float Radius, float HalfHeight, const std::string MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, bool IsExclusive)
{
	return GenerateShape(ShapeName, physx::PxCapsuleGeometry(Radius, HalfHeight), MaterialName, MaterialStaticFriction, MaterialDynamicFriction, MaterialRestitution, Group, IsExclusive);
}
physx::PxShape* CPhysXManager::CreatePlane(const std::string &ShapeName, const std::string MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, bool IsExclusive)
{
	return GenerateShape(ShapeName, physx::PxPlaneGeometry(), MaterialName, MaterialStaticFriction, MaterialDynamicFriction, MaterialRestitution, Group, IsExclusive);
}
physx::PxShape* CPhysXManager::CreateConvexMesh(const std::string &ShapeName, std::vector<Vect3f> Vertices, const std::string MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, bool IsExclusive)
{
	physx::PxConvexMesh* l_ConvexMesh = CookConvexMesh(ShapeName, Vertices);
	return GenerateShape(ShapeName, physx::PxConvexMeshGeometry(l_ConvexMesh), MaterialName, MaterialStaticFriction, MaterialDynamicFriction, MaterialRestitution, Group, IsExclusive);
}
physx::PxShape* CPhysXManager::CreateTriangleMesh(const std::string &ShapeName, std::vector<Vect3f> Vertices, std::vector<unsigned short> Indices, const std::string MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, bool IsExclusive)
{
	physx::PxTriangleMesh* l_TriangleMesh = CookTriangleMesh(ShapeName, Vertices, Indices);
	return GenerateShape(ShapeName, physx::PxTriangleMeshGeometry(l_TriangleMesh), MaterialName, MaterialStaticFriction, MaterialDynamicFriction, MaterialRestitution, Group, IsExclusive);
}

physx::PxShape* CPhysXManager::CreateTriangleMeshFromFile(const std::string &ShapeName, std::string &Filename, const std::string MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group)
{
	physx::PxShape* l_Shape = nullptr;

	std::vector<Vect3f> l_ShapeVertex;
	std::vector<unsigned short> l_ShapeIndex;

	FILE *l_File = NULL;
	fopen_s(&l_File, (Filename.c_str()), "rb");

	void *l_VtxsData = NULL;
	void *l_IdxData = NULL;

	unsigned short l_DefaultHeader = 0xfe55;
	unsigned short l_DefaultFooter = 0x55fe;

	unsigned short l_Header;

	fread(&l_Header, sizeof(unsigned short), 1, l_File);

	if (l_Header == l_DefaultHeader)
	{
		unsigned int l_NumVertexs = 0;
		fread(&l_NumVertexs, sizeof(unsigned short), 1, l_File);

		if (l_NumVertexs > 0)
		{
			unsigned int l_NumBytes = sizeof(float)*3*l_NumVertexs;

			l_VtxsData = malloc(l_NumBytes);
			fread(l_VtxsData, l_NumBytes, 1, l_File);
			long offset = (l_NumBytes / l_NumVertexs);

			size_t l_LastNumberOfVertices = l_ShapeVertex.size();
			l_ShapeVertex.resize(l_LastNumberOfVertices + l_NumVertexs);
			void* l_ReadVtx = l_VtxsData;

			for (size_t i = 0; i < l_NumVertexs; ++i)
			{
				Vect3f* l_Vertex = static_cast<Vect3f*>(l_ReadVtx);
				l_ShapeVertex[i + l_LastNumberOfVertices] = *l_Vertex;

				l_ReadVtx = static_cast<char*>(l_ReadVtx)+offset;
			}

			free(l_VtxsData);

			unsigned short m_NumIndexs;
			unsigned short l_NumIndexsFile;
			fread(&l_NumIndexsFile, sizeof(unsigned short), 1, l_File);
			l_NumBytes = sizeof(unsigned short)*l_NumIndexsFile;
			m_NumIndexs = (unsigned short)l_NumIndexsFile;
			l_IdxData = malloc(l_NumBytes);
			fread(l_IdxData, 1, l_NumBytes, l_File);

			size_t l_LastNumberOfIndexs = l_ShapeIndex.size();
			l_ShapeIndex.resize(l_LastNumberOfIndexs + m_NumIndexs);
			void* l_ReadIndex = l_IdxData;

			for (size_t i = 0; i < m_NumIndexs; ++i)
			{
				unsigned short* l_Index = static_cast<unsigned short*>(l_ReadIndex);
				l_ShapeIndex[i + l_LastNumberOfIndexs] = *l_Index + l_LastNumberOfVertices;
				l_ReadIndex = static_cast<char*>(l_ReadIndex)+sizeof(unsigned short);
			}

			free(l_IdxData);
		}
		unsigned short l_Footer = 0;
		fread(&l_Footer, sizeof(unsigned short), 1, l_File);
		if (l_Footer == l_DefaultFooter)
		{
			l_Shape = CreateTriangleMesh(ShapeName, l_ShapeVertex, l_ShapeIndex, MaterialName, MaterialStaticFriction, MaterialDynamicFriction, MaterialRestitution, Group, true);

			//SUPER HARDCODEADO PARA LA SALA 3
			Quatf l_Rot = Quatf(0.0, 0.0, 0.0, 1.0);
			Vect3f l_PositionQ = Vect3f(10.8045f, -2.90423f, 16.0195f);
			CreateStaticActor(ShapeName, ShapeName, l_PositionQ, l_Rot);
		}
		else
		{
			#ifdef _DEBUG
				CEngine::GetSingleton().GetLogManager()->Log("Error loading shape file " + Filename);
			#endif
		}
	}

	fclose(l_File);

	return l_Shape;
}