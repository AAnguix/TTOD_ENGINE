#include "PhysXManager.h"

#include <PxPhysicsAPI.h>

class CPhysxManagerImplementation;

#define PHYSX_UPDATE_STEP (1.0f/30.0f)

inline physx::PxVec3 CastVec(const Vect3f& v)
{return physx::PxVec3(v.x,v.y,v.z);}

inline Vect3f CastVec(const physx::PxVec3& v)
{return Vect3f(v.x,v.y,v.z);}

inline Vect3f CastVec(const physx::PxExtendedVec3& v)
{return Vect3f((float)v.x,(float)v.y,(float)v.z);}

inline physx::PxQuat CastQuat(const Quatf& q)
{return physx::PxQuat(q.x,q.y,q.z,q.w);}

inline Quatf CastQuat(const physx::PxQuat& q)
{return Quatf(q.x,q.y,q.z,q.w);}

struct RaycastData
{
	Vect3f m_Position;
	Vect3f m_Normal;
	float m_Distance;
	std::string m_ActorName;
};

CPhysXManager::CPhysXManager()
{
	m_LeftOverSeconds=0.0f;
}

CPhysXManager::~CPhysXManager()
{
	
}

void CPhysXManager::RegisterMaterial(const std::string &Name, float StaticFriction, float DynamicFriction,float Restitution)
{
	auto it = m_Materials.find(Name);
	
	if(it != m_Materials.end())
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
	
	if(it != m_Materials.end())
	{
		return it->second;
	}

	return nullptr;
}


void CPhysXManager::CreateRigidStatic(const std::string &Name, const Vect3f Size, const Vect3f &Position, const Quatf &Orientation, const std::string &MaterialName)
{
	CheckMapAndVectors();
	const physx::PxMaterial* l_Material = GetMaterial(MaterialName);

	physx::PxVec3 v = CastVec(Size);
	physx::PxShape* l_Shape = m_PhysX->createShape(physx::PxBoxGeometry(v.x/2,v.y/2,v.z/2),*l_Material);
	physx::PxRigidStatic* l_Body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(Position),CastQuat(Orientation)));
	
	l_Body->attachShape(*l_Shape);
	size_t index=m_Actors.size();
	l_Body->userData = (void*)index;
	m_Scene->addActor(*l_Body);
	l_Shape->release();

	SaveActorData(index,Name,Position,Orientation,l_Body);
}

void CPhysXManager::CreatePlane(const std::string &Name, const Vect3f &Normal, float Distance,  const Vect3f &Position,  const Quatf &Orientation, const std::string &MaterialName)
{
	CheckMapAndVectors();
	physx::PxMaterial* l_Material = GetMaterial(MaterialName);

	physx::PxRigidStatic* groundPlane = PxCreatePlane(*m_PhysX,physx::PxPlane(CastVec(Normal),Distance),*l_Material);
	size_t index=m_Actors.size();
	groundPlane->userData = (void*)index;
	m_Scene->addActor(*groundPlane);

	physx::PxShape* shape;
	size_t numShapes = groundPlane->getShapes(&shape,1);
	assert(numShapes == 1);

	SaveActorData(index,Name,Position,Orientation,groundPlane);
}

void CPhysXManager::CreateRigidDynamic(const std::string &Name, const Vect3f Size, const Vect3f &Position, const Quatf &Orientation, const std::string &MaterialName, physx::PxReal Density, bool Trigger)
{
	CheckMapAndVectors();

	physx::PxVec3 v = CastVec(Size);
	physx::PxMaterial* l_Material = GetMaterial(MaterialName);
	physx::PxShape* l_Shape = m_PhysX->createShape(physx::PxBoxGeometry(v.x/2,v.y/2,v.z/2),*l_Material);
	physx::PxRigidDynamic* l_Body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(Position),CastQuat(Orientation)));
	
	l_Body->attachShape(*l_Shape);
	physx::PxRigidBodyExt::updateMassAndInertia(*l_Body,Density);
	l_Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE,!Trigger);
	l_Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE,Trigger);
	
	size_t index=m_Actors.size();
	l_Body->userData = (void*)index;
	m_Scene->addActor(*l_Body);
	l_Shape->release();

	SaveActorData(index,Name,Position,Orientation,l_Body);
}

void CPhysXManager::CreateConvexMesh(std::vector<Vect3f> Vertices, const std::string &MeshName, const Vect3f &Position, const Quatf &Orientation, const std::string &MaterialName)
{
	CheckMapAndVectors();

	physx::PxConvexMeshDesc l_ConvexDesc;
	l_ConvexDesc.points.count= Vertices.size();
	l_ConvexDesc.points.stride = sizeof(Vect3f);
	l_ConvexDesc.points.data=&Vertices[0];
	l_ConvexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

	physx::PxDefaultMemoryOutputStream l_Buffer;
	physx::PxConvexMeshCookingResult::Enum l_Result;
	bool success = m_Cooking->cookConvexMesh(l_ConvexDesc,l_Buffer,&l_Result);
	assert(success);
	physx::PxDefaultMemoryInputData l_Input(l_Buffer.getData(),l_Buffer.getSize());
	physx::PxConvexMesh* l_ConvexMesh = m_PhysX->createConvexMesh(l_Input);

	physx::PxMaterial* l_Material = GetMaterial(MaterialName);

	physx::PxRigidDynamic* l_Body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(Position),CastQuat(Orientation)));
	physx::PxShape* l_Shape = l_Body->createShape(physx::PxConvexMeshGeometry(l_ConvexMesh),*l_Material);

	l_Body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);

	size_t index=m_Actors.size();
	l_Body->userData = (void*)index;
	m_Scene->addActor(*l_Body);
	//l_Shape->release();

	SaveActorData(index,MeshName,Position,Orientation,l_Body);
}



void CPhysXManager::SaveActorData(size_t Index, const std::string ActorName,const Vect3f &Position, const Quatf &Orientation, physx::PxActor *Actor)
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

	if(m_LeftOverSeconds >= PHYSX_UPDATE_STEP)
	{
		m_Scene->simulate(PHYSX_UPDATE_STEP);
		m_Scene->fetchResults(true);

		m_LeftOverSeconds = fmod(m_LeftOverSeconds,PHYSX_UPDATE_STEP);
	
		physx::PxU32 l_NumActiveTransforms;
		const physx::PxActiveTransform* activeTransforms = m_Scene->getActiveTransforms(l_NumActiveTransforms);
		
		for(physx::PxU32 i=0;i<l_NumActiveTransforms;++i)
		{
			uintptr_t index = (uintptr_t) (activeTransforms[i].userData);
			m_ActorPositions[index] = CastVec(activeTransforms[i].actor2World.p);
			m_ActorOrientations[index] = CastQuat(activeTransforms[i].actor2World.q);
		}
	}
}

bool CPhysXManager::RemoveActor(const std::string &ActorName)
{
	auto it_controller = m_CharacterControllers.find(ActorName);
	auto it_Actors = m_ActorIndexs.find(ActorName);

	if(it_controller!=m_CharacterControllers.end()) //Is a C.Controller
	{
		it_controller->second->release();
		m_CharacterControllers.erase(it_controller);

		return true;
	}
	else
	{
		if(it_Actors!=m_ActorIndexs.end())
		{
			size_t l_RemovedActorIndex = it_Actors->second;

			m_Actors[l_RemovedActorIndex]->release();

			size_t l_MovedActorIndex=m_Actors.size()-1;
		
			//Coppy last actor info to removed actor index.
			m_Actors[l_RemovedActorIndex] = m_Actors[m_Actors.size()-1]; 
			m_Actors.resize(m_Actors.size() - 1);

			m_ActorNames[l_RemovedActorIndex] = m_ActorNames[m_ActorNames.size()-1];
			m_ActorNames.resize(m_ActorNames.size()-1);

			m_ActorPositions[l_RemovedActorIndex] = m_ActorPositions[m_ActorPositions.size()-1];
			m_ActorPositions.resize(m_ActorPositions.size()-1);

			m_ActorOrientations[l_RemovedActorIndex] = m_ActorOrientations[m_ActorOrientations.size()-1];
			m_ActorOrientations.resize(m_ActorOrientations.size()-1);

			m_ActorIndexs.erase(it_Actors);

			if(l_RemovedActorIndex<m_Actors.size())
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
	physx::PxController* l_Controller = m_CharacterControllers[CharacterControllerName];
	const physx::PxControllerFilters l_Filters(nullptr,nullptr,nullptr); 
	
	size_t index = (size_t)l_Controller->getUserData();

	l_Controller->move(CastVec(Movement),Movement.Length()*0.01f,ElapsedTime,l_Filters);

	physx::PxRigidDynamic* l_Actor = l_Controller->getActor();

	physx::PxExtendedVec3 l_FPos = l_Controller->getFootPosition();
	physx::PxVec3 l_V = l_Actor->getLinearVelocity();

	CharacterControllerData l_CCData(CastVec(l_FPos),CastVec(l_V));

	return GetActorPosition(CharacterControllerName);
	//return l_CCData;
}

bool CPhysXManager::Raycast(const Vect3f& Origin, const Vect3f& End, int FilterMask, RaycastData* Result_)
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
	
	Result_->m_Position=CastVec(l_ReturnBuffer.block.position);
	Result_->m_Normal=CastVec(l_ReturnBuffer.block.normal);
	Result_->m_Distance=l_ReturnBuffer.block.distance;
	Result_->m_ActorName=m_ActorNames[(size_t)l_ReturnBuffer.block.actor->userData];

	return true;
}

void CPhysXManager::Reload()
{

}






