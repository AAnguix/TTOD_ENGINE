#include "PhysXManager.h"

#include <PxPhysicsAPI.h>

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

void CPhysXManager::RegisterMaterial(const std::string &Name, float StaticFriction, float DynamicFriction, float Restitution)
{
	auto it = m_Materials.find(Name);

	if (it != m_Materials.end())
	{
		it->second->setStaticFriction(StaticFriction);
		it->second->setDynamicFriction(DynamicFriction);
		it->second->setRestitution(Restitution);
	}

	m_Materials[Name] = m_PhysX->createMaterial(StaticFriction, DynamicFriction, Restitution);
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

void CPhysXManager::RegisterActor(const std::string &ActorName, physx::PxShape* Shape, physx::PxRigidActor* Body, Vect3f Position, Quatf Orientation, int Group)
{
	//L_PutGroupToShape(Shape, Group);

	Body->attachShape(*Shape);
	m_Scene->addActor(*Body);
	Body->userData = (void*)AddActor(ActorName, Position, Orientation, Body);

}

void CPhysXManager::RegisterActor(const std::string &ActorName, physx::PxShape* Shape, physx::PxRigidBody* Body, Vect3f Position, Quatf Orientation, int Group, float Density, bool IsKinematic)
{
	Body->attachShape(*Shape);
	physx::PxRigidBodyExt::updateMassAndInertia(*Body, Density);
	
	//Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !Trigger);
	//Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, Trigger);
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

physx::PxShape* CPhysXManager::CreateStaticShape(const std::string &Name, physx::PxGeometry &Geometry, const std::string &Material, const Vect3f &Position, const Quatf &Orientation, int Group)
{
	physx::PxMaterial* l_Material = GetMaterial(Material);
	physx::PxShape* shape = m_PhysX->createShape(Geometry, *l_Material, false);

	assert(shape != nullptr);
	physx::PxRigidStatic* body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(Position), CastQuat(Orientation)));

	RegisterActor(Name, shape, body, Position, Orientation, Group);

	return shape;
}
physx::PxShape* CPhysXManager::CreateDinamicShape(const std::string &Name, physx::PxGeometry &Geometry, const std::string &Material, const Vect3f &Position, const Quatf &Orientation, int Group, physx::PxReal Density, bool IsKinematic)
{
	physx::PxMaterial* l_Material = GetMaterial(Material);
	physx::PxShape* shape = m_PhysX->createShape(Geometry, *l_Material, false);

	assert(shape != nullptr);
	physx::PxRigidDynamic* body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(Position), CastQuat(Orientation)));

	RegisterActor(Name, shape, body, Position, Orientation, Group, Density, IsKinematic);

	return shape;
}

/*OBSOLETE*/
void CPhysXManager::CreateRigidDynamic(const std::string &Name, const Vect3f Size, const Vect3f &Position, const Quatf &Orientation, const std::string &MaterialName, physx::PxReal Density, bool Trigger)
{
	CheckMapAndVectors();

	physx::PxVec3 v = CastVec(Size);
	physx::PxMaterial* l_Material = GetMaterial(MaterialName);
	physx::PxShape* l_Shape = m_PhysX->createShape(physx::PxBoxGeometry(v.x / 2, v.y / 2, v.z / 2), *l_Material);
	physx::PxRigidDynamic* l_Body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(Position), CastQuat(Orientation)));

	l_Body->attachShape(*l_Shape);
	physx::PxRigidBodyExt::updateMassAndInertia(*l_Body, Density);
	l_Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !Trigger);
	l_Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, Trigger);

	size_t index = m_Actors.size();
	l_Body->userData = (void*)index;
	m_Scene->addActor(*l_Body);
	l_Shape->release();

	SaveActorData(index, Name, Position, Orientation, l_Body);
}

/*Rigid static*/
void CPhysXManager::CreateRigidStaticBox(const std::string &Name, const Vect3f &Size, const std::string Material, const Vect3f &Position, const Quatf &Orientation, int Group)
{
	physx::PxVec3 l_Size = CastVec(Size);
	physx::PxShape* l_Shape = CreateStaticShape(Name, physx::PxBoxGeometry(l_Size.x / 2, l_Size.y / 2, l_Size.z / 2), Material, Position, Orientation, Group);

	if (l_Shape != nullptr)
		l_Shape->release();
}
void CPhysXManager::CreateRigidStaticSphere(const std::string &Name, const float &Radius, const std::string Material, const Vect3f &Position, const Quatf &Orientation, int Group)
{
	physx::PxShape* l_Shape = CreateStaticShape(Name, physx::PxSphereGeometry(Radius), Material, Position, Orientation, Group);

	if (l_Shape != nullptr)
		l_Shape->release();
}
void CPhysXManager::CreateRigidStaticCapsule(const std::string &Name, const float &Radius, const float &HalfHeight, const std::string Material, const Vect3f &Position, const Quatf &Orientation, int Group)
{
	physx::PxShape* l_Shape = CreateStaticShape(Name, physx::PxCapsuleGeometry(Radius,HalfHeight), Material, Position, Orientation, Group);

	if (l_Shape != nullptr)
		l_Shape->release();
}
void CPhysXManager::CreateRigidStaticPlane(const std::string &Name, const Vect3f &Normal, float Distance, const std::string Material, const Vect3f &Position, const Quatf &Orientation, int Group)
{
	physx::PxMaterial* l_Material = GetMaterial(Material);

	physx::PxRigidStatic* l_GroundPlane = PxCreatePlane(*m_PhysX, physx::PxPlane(CastVec(Normal), Distance), *l_Material);
	
	physx::PxShape* l_Shape;
	size_t numShapes = l_GroundPlane->getShapes(&l_Shape, 1);
	assert(numShapes == 1);

	l_GroundPlane->userData = (void*)AddActor(Name, Position, Orientation, l_GroundPlane);
	m_Scene->addActor(*l_GroundPlane);
}
void CPhysXManager::CreateRigidStaticConvexMesh(const std::string &Name, std::vector<Vect3f> Vertices, const std::string Material, const Vect3f &Position, const Quatf &Orientation, int Group)
{
	/*physx::PxConvexMesh* l_ConvexMesh = CreateConvexMesh(Vertices);

	physx::PxMaterial* l_Material = GetMaterial(Material);

	physx::PxRigidStatic* l_Body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(Position), CastQuat(Orientation)));
	physx::PxShape* l_Shape = l_Body->createShape(physx::PxConvexMeshGeometry(l_ConvexMesh), *l_Material);

	RegisterActor(Name, l_Shape, l_Body, Position, Orientation, Group);
	*/
	//if (l_Shape != nullptr)
		//l_Shape->release();

	physx::PxConvexMesh* l_ConvexMesh = CreateConvexMesh(Vertices);
	physx::PxShape* l_Shape = CreateStaticShape(Name, physx::PxConvexMeshGeometry(l_ConvexMesh), Material, Position, Orientation, Group);

	if (l_Shape != nullptr)
		l_Shape->release();
	
}
void CPhysXManager::CreateRigidStaticTriangleMesh(const std::string &Name, std::vector<Vect3f> Vertices, std::vector<unsigned int> Indices, const std::string Material, const Vect3f &Position, const Quatf &Orientation, int Group)
{
	//physx::PxTriangleMesh* l_TriangleMesh= CreateTriangleMesh(Vertices);

	//physx::PxMaterial* l_Material = GetMaterial(Material);

	//physx::PxRigidStatic* l_Body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(Position), CastQuat(Orientation)));
	//physx::PxShape* l_Shape = l_Body->createShape(physx::PxTriangleMeshGeometry(l_TriangleMesh), *l_Material);

	////l_Body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);

	//RegisterActor(Name, l_Shape, l_Body, Position, Orientation, Group);

	//if (l_Shape != nullptr)
	//	l_Shape->release();

	physx::PxTriangleMesh* l_TriangleMesh = CreateTriangleMesh(Vertices, Indices);
	physx::PxShape* l_Shape = CreateStaticShape(Name, physx::PxTriangleMeshGeometry(l_TriangleMesh), Material, Position, Orientation, Group);

	if (l_Shape != nullptr)
		l_Shape->release();
}

/*Rigid dynamic*/
void CPhysXManager::CreateRigidDynamicBox(const std::string &Name, const Vect3f &Size, const std::string Material, const Vect3f &Position, const Quatf &Orientation, int Group, float Density, bool IsKinematic)
{
	physx::PxVec3 l_Size = CastVec(Size);
	physx::PxShape* l_Shape = CreateDinamicShape(Name, physx::PxBoxGeometry(l_Size.x / 2, l_Size.y / 2, l_Size.z / 2), Material, Position, Orientation, Group, Density, IsKinematic);

	if (l_Shape != nullptr)
		l_Shape->release();
}
void CPhysXManager::CreateRigidDynamicSphere(const std::string &Name, const float &Radius, const std::string Material, const Vect3f &Position, const Quatf &Orientation, int Group, float Density, bool IsKinematic)
{
	physx::PxShape* l_Shape = CreateDinamicShape(Name, physx::PxSphereGeometry(Radius), Material, Position, Orientation, Group, Density, IsKinematic);

	if (l_Shape != nullptr)
		l_Shape->release();
}
void CPhysXManager::CreateRigidDynamicCapsule(const std::string &Name, const float &Radius, const float &HalfHeight, const std::string Material, const Vect3f &Position, const Quatf &Orientation, int Group, physx::PxReal Density, bool IsKinematic)
{
	physx::PxShape* l_Shape = CreateDinamicShape(Name, physx::PxCapsuleGeometry(Radius, HalfHeight), Material, Position, Orientation, Group, Density, IsKinematic);

	//physx::PxTransform l_RelativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
	//l_Shape->setLocalPose(l_RelativePose);

	if (l_Shape != nullptr)
		l_Shape->release();
}

void CPhysXManager::CreateRigidDynamicConvexMesh(const std::string &Name, std::vector<Vect3f> Vertices, const std::string Material, const Vect3f &Position, const Quatf &Orientation, int Group, float Density, bool IsKinematic)
{
	physx::PxConvexMesh* l_ConvexMesh = CreateConvexMesh(Vertices);

	physx::PxMaterial* l_Material = GetMaterial(Material);

	physx::PxRigidDynamic* l_Body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(Position), CastQuat(Orientation)));
	physx::PxShape* l_Shape = l_Body->createShape(physx::PxConvexMeshGeometry(l_ConvexMesh), *l_Material);

	l_Body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, IsKinematic);
	
	l_Body->userData = (void*)AddActor(Name, Position, Orientation, l_Body);
	m_Scene->addActor(*l_Body);

	//if (l_Shape != nullptr)
		//l_Shape->release();
}

void CPhysXManager::CreateRigidKinematicTriangleMesh(const std::string &Name, std::vector<Vect3f> Vertices, std::vector<unsigned int> Indices, const std::string Material, const Vect3f &Position, const Quatf &Orientation, int Group, float Density)
{
	physx::PxTriangleMesh* l_ConvexMesh = CreateTriangleMesh(Vertices, Indices);

	physx::PxMaterial* l_Material = GetMaterial(Material);

	physx::PxRigidDynamic* l_Body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(Position), CastQuat(Orientation)));
	l_Body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
	physx::PxShape* l_Shape = l_Body->createShape(physx::PxTriangleMeshGeometry(l_ConvexMesh), *l_Material);

	l_Body->userData = (void*)AddActor(Name, Position, Orientation, l_Body);
	m_Scene->addActor(*l_Body);
}



/*Meshes that need to be cooked*/
physx::PxConvexMesh*  CPhysXManager::CreateConvexMesh(std::vector<Vect3f> Vertices)
{
	#if USE_PHYSX_DEBUG
		CheckMapAndVectors();
	#endif

	physx::PxConvexMeshDesc l_ConvexDesc;
	l_ConvexDesc.points.count = Vertices.size();
	l_ConvexDesc.points.stride = sizeof(Vect3f);
	l_ConvexDesc.points.data = &Vertices[0];
	l_ConvexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

	physx::PxDefaultMemoryOutputStream l_Buffer;
	physx::PxConvexMeshCookingResult::Enum l_Result;
	bool success = m_Cooking->cookConvexMesh(l_ConvexDesc, l_Buffer, &l_Result);
	
	if (!success)
	{
		l_ConvexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX | physx::PxConvexFlag::eINFLATE_CONVEX;
		success = m_Cooking->cookConvexMesh(l_ConvexDesc, l_Buffer, &l_Result);
		assert(success);
	}
	
	physx::PxDefaultMemoryInputData l_Input(l_Buffer.getData(), l_Buffer.getSize());
	physx::PxConvexMesh* l_ConvexMesh = m_PhysX->createConvexMesh(l_Input);

	return l_ConvexMesh;
}
physx::PxTriangleMesh*  CPhysXManager::CreateTriangleMesh(std::vector<Vect3f> Vertices, std::vector<unsigned int> Indices)
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


	physx::PxTriangleMeshDesc l_TriangleDesc;
	l_TriangleDesc.points.count = Vertices.size();
	l_TriangleDesc.points.stride = sizeof(Vect3f);
	l_TriangleDesc.points.data = &Vertices[0];

	l_TriangleDesc.triangles.count = Indices.size() - 2;
	l_TriangleDesc.triangles.stride = 3 * sizeof(physx::PxU32);
	l_TriangleDesc.triangles.data = &Indices[0];

	//#if USE_PHYSX_DEBUG
	//	// mesh should be validated before cooked without the mesh cleaning
	//	bool res = m_Cooking->validateTriangleMesh(l_TriangleDesc);
	//	PX_ASSERT(res);
	//#endif

	physx::PxDefaultMemoryOutputStream l_Buffer;
	bool success = m_Cooking->cookTriangleMesh(l_TriangleDesc, l_Buffer);
	assert(success);
	physx::PxDefaultMemoryInputData l_Input(l_Buffer.getData(), l_Buffer.getSize());
	physx::PxTriangleMesh* l_TriangleMesh = m_PhysX->createTriangleMesh(l_Input);

	return l_TriangleMesh;
}

void CPhysXManager::CreateRigidStatic(const std::string &Name, const Vect3f Size, const Vect3f &Position, const Quatf &Orientation, const std::string &MaterialName)
{
	CheckMapAndVectors();
	const physx::PxMaterial* l_Material = GetMaterial(MaterialName);

	physx::PxVec3 v = CastVec(Size);
	physx::PxShape* l_Shape = m_PhysX->createShape(physx::PxBoxGeometry(v.x / 2, v.y / 2, v.z / 2), *l_Material, false);
	physx::PxRigidStatic* l_Body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(Position), CastQuat(Orientation)));

	l_Body->attachShape(*l_Shape);
	size_t index = m_Actors.size();
	l_Body->userData = (void*)index;
	m_Scene->addActor(*l_Body);
	l_Shape->release();

	SaveActorData(index, Name, Position, Orientation, l_Body);
}

void CPhysXManager::CreatePlane(const std::string &Name, const Vect3f &Normal, float Distance, const Vect3f &Position, const Quatf &Orientation, const std::string &MaterialName)
{
	CheckMapAndVectors();
	physx::PxMaterial* l_Material = GetMaterial(MaterialName);

	physx::PxRigidStatic* groundPlane = PxCreatePlane(*m_PhysX, physx::PxPlane(CastVec(Normal), Distance), *l_Material);
	size_t index = m_Actors.size();
	groundPlane->userData = (void*)index;
	m_Scene->addActor(*groundPlane);

	physx::PxShape* shape;
	size_t numShapes = groundPlane->getShapes(&shape, 1);
	assert(numShapes == 1);

	SaveActorData(index, Name, Position, Orientation, groundPlane);
}

/*va aqui*/

void CPhysXManager::CreateConvexMeshOLD(std::vector<Vect3f> Vertices, const std::string &MeshName, const Vect3f &Position, const Quatf &Orientation, const std::string &MaterialName)
{
	CheckMapAndVectors();

	physx::PxConvexMeshDesc l_ConvexDesc;
	l_ConvexDesc.points.count = Vertices.size();
	l_ConvexDesc.points.stride = sizeof(Vect3f);
	l_ConvexDesc.points.data = &Vertices[0];
	l_ConvexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

	physx::PxDefaultMemoryOutputStream l_Buffer;
	physx::PxConvexMeshCookingResult::Enum l_Result;
	bool success = m_Cooking->cookConvexMesh(l_ConvexDesc, l_Buffer, &l_Result);
	assert(success);
	physx::PxDefaultMemoryInputData l_Input(l_Buffer.getData(), l_Buffer.getSize());
	physx::PxConvexMesh* l_ConvexMesh = m_PhysX->createConvexMesh(l_Input);

	physx::PxMaterial* l_Material = GetMaterial(MaterialName);

	physx::PxRigidDynamic* l_Body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(Position), CastQuat(Orientation)));
	physx::PxShape* l_Shape = l_Body->createShape(physx::PxConvexMeshGeometry(l_ConvexMesh), *l_Material);

	l_Body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);

	size_t index = m_Actors.size();
	l_Body->userData = (void*)index;
	m_Scene->addActor(*l_Body);
	//l_Shape->release();

	SaveActorData(index, MeshName, Position, Orientation, l_Body);
}

size_t CPhysXManager::GetActorIndex(const std::string& ActorName) const
{
	auto it = m_ActorIndexs.find(ActorName);
	assert(it != m_ActorIndexs.end());
	return it->second;
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
	assert(m_Actors.size() == m_ActorNames.size());
	assert(m_Actors.size() == m_ActorPositions.size());
	assert(m_Actors.size() == m_ActorOrientations.size());
	assert(m_Actors.size() == m_ActorIndexs.size());
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
}

bool CPhysXManager::RemoveActor(const std::string &ActorName)
{
	auto it_controller = m_CharacterControllers.find(ActorName);
	auto it_Actors = m_ActorIndexs.find(ActorName);

	if (it_controller != m_CharacterControllers.end()) //Is a C.Controller
	{
		it_controller->second->release();
		m_CharacterControllers.erase(it_controller);

		return true;
	}
	else
	{
		if (it_Actors != m_ActorIndexs.end())
		{
			size_t l_RemovedActorIndex = it_Actors->second;

			m_Actors[l_RemovedActorIndex]->release();

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
	}

	return false;
}

/*bool CPhysXManager::RemoveActor(const std::string &ActorName)
{
auto it_controller = m_CharacterControllers.find(ActorName);
auto it_Actors = m_ActorIndexs.find(ActorName);

if(it_Actors != m_ActorIndexs.end())
{
size_t index = it_Actors->second;

if(it_controller!=m_CharacterControllers.end()) //Is a C.Controller
{
it_controller->second->release();
m_CharacterControllers.erase(it_controller);
}
else
{
m_Actors[index]->release();
}

//Coppy last object to removed actor pos.
m_Actors[index] = m_Actors[m_Actors.size()-1];

m_Actors.resize(m_Actors.size() - 1);

m_ActorIndexs[m_ActorNames[index]] = index;
m_Actors[index]->userData = (void*)index;

return true;
}

return false;
}
*/

void CPhysXManager::CreateCharacterController(const std::string &Name, float Height, float Radius, float Density, Vect3f &Position, const std::string &MaterialName)
{

}

Vect3f CPhysXManager::MoveCharacterController(const std::string& CharacterControllerName, const Vect3f &Movement, float ElapsedTime)
{
	Vect3f l_Move = v3fZERO;
	if (!isnan(Movement.x) && !isnan(Movement.y) && !isnan(Movement.z))
		l_Move = Movement;
	else 
	{
		int j = 0;
	}
	physx::PxController* l_Controller = m_CharacterControllers[CharacterControllerName];
	const physx::PxControllerFilters l_Filters(nullptr, nullptr, nullptr);

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
	physx::PxController* l_CController = m_CharacterControllers[CharacterControllerName];
	return Vect3f(CastVec(l_CController->getPosition()));
}

Vect3f CPhysXManager::GetCharacterControllerFootPosition(const std::string& CharacterControllerName)
{
	physx::PxController* l_CController = m_CharacterControllers[CharacterControllerName];
	return Vect3f(CastVec(l_CController->getFootPosition()));
}

void CPhysXManager::SetShapeAsTrigger(const std::string &ActorName)
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
			l_Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
			l_Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
		}
		else
		{
			physx::PxRigidStatic* l_StaticActor = l_PxActor->is<physx::PxRigidStatic>();

			if (l_StaticActor != nullptr)
			{
				l_StaticActor->getShapes(&l_Shape, 1);
				l_Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
				l_Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
			}
		}
	}
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
		CastVec(Origin),
		CastVec(l_Direction),
		l_Lenght,
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






