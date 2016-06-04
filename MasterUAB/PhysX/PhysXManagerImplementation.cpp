#include "PhysXManagerImplementation.h"
#include <Windows.h>
#include <sstream>
#include "Engine.h"
#include "LuabindManager\LuabindManager.h"

static physx::PxDefaultErrorCallback gDefaultErrorCallback;
static physx::PxDefaultAllocator gDefaultAllocatorCallback;

#if USE_PHYSX_DEBUG
#define PVD_HOST "127.0.0.1"
#endif

CPhysXManager* CPhysXManager::CreatePhysxManager()
{
	return new CPhysXManagerImplementation();
}

CPhysXManagerImplementation::CPhysXManagerImplementation()
{
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	assert(m_Foundation);
	physx::PxProfileZoneManager* profileZoneManager = nullptr;

#if USE_PHYSX_DEBUG
	profileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager(m_Foundation);
#endif

	m_PhysX = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale(), true, profileZoneManager);
	assert(m_PhysX);

	m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, physx::PxCookingParams(physx::PxTolerancesScale()));
	assert(m_Cooking);

#if USE_PHYSX_DEBUG
	if (m_PhysX->getPvdConnectionManager())
	{
		m_PhysX->getVisualDebugger()->setVisualizeConstraints(true);
		m_PhysX->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_CONTACTS, true);
		m_PhysX->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
		m_DebugConnection = physx::PxVisualDebuggerExt::createConnection(m_PhysX->getPvdConnectionManager(), PVD_HOST, 5425, 10);
	}
	else
	{
		m_DebugConnection = nullptr;
	}
#endif

	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	DWORD numCPU = sysinfo.dwNumberOfProcessors;
	m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(numCPU);

	physx::PxSceneDesc sceneDesc(m_PhysX->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.cpuDispatcher = m_Dispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	sceneDesc.flags = physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
	m_Scene = m_PhysX->createScene(sceneDesc);
	assert(m_Scene);

	m_Scene->setSimulationEventCallback(this);

	m_ControllerManager = PxCreateControllerManager(*m_Scene);
	m_ControllerManager->setOverlapRecoveryModule(true);
}

CPhysXManagerImplementation::~CPhysXManagerImplementation()
{
	for (auto it = m_CharacterControllers.begin(); it != m_CharacterControllers.end(); ++it)
	{
		physx::PxController* l_Controller = it->second;
		l_Controller->release();
	}
	m_CharacterControllers.clear();

	/*for (size_t i = 0; i<m_Actors.size();++i)
	{
		physx::PxRigidDynamic* l_DynamicActor = m_Actors[i]->is<physx::PxRigidDynamic>();
		{
			if (l_DynamicActor != nullptr)
			{
				l_DynamicActor->detachShape();
			}
		}
	}*/
	m_CharacterControllers.clear();

	if (m_ControllerManager != nullptr)
	{
		m_ControllerManager->release();	m_ControllerManager = nullptr;
	}

	if (m_Scene != nullptr)
	{
		m_Scene->release();	m_Scene = nullptr;
	}

	if (m_Dispatcher != nullptr)
	{
		m_Dispatcher->release();	m_Dispatcher = nullptr;
	}

	physx::PxProfileZoneManager* profileZoneManager = m_PhysX->getProfileZoneManager();
#if USE_PHYSX_DEBUG
	{
		if (m_DebugConnection != nullptr)
		{
			m_DebugConnection->release();	m_DebugConnection = nullptr;
		}
	}
#endif

	if (m_Cooking != nullptr)
	{
		m_Cooking->release();	m_Cooking = nullptr;
	}

	if (m_PhysX != nullptr)
	{
		m_PhysX->release();	m_PhysX = nullptr;
	}

	if (profileZoneManager != nullptr)
	{
		profileZoneManager->release();	profileZoneManager = nullptr;
	}

	if (m_Foundation != nullptr)
	{
		m_Foundation->release();	m_Foundation = nullptr;
	}
}

void CPhysXManagerImplementation::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
{
	/*for(physx::PxU32 i=0;i<count;++i)
	{

	}*/
}

void CPhysXManagerImplementation::onWake(physx::PxActor** actors, physx::PxU32 count)
{
	for (physx::PxU32 i = 0; i<count; ++i)
	{
		size_t index = (size_t)(*actors)->userData;
		std::string objectName = m_ActorNames[index];

		std::stringstream l_Call;
		l_Call << "OnWake" << "('" << objectName << "')";
		std::string l_Code = l_Call.str();
		CEngine::GetSingleton().GetLuabindManager()->RunCode(l_Code);
	}
}

void CPhysXManagerImplementation::onSleep(physx::PxActor** actors, physx::PxU32 count)
{
	for (physx::PxU32 i = 0; i<count; ++i)
	{
		size_t index = (size_t)(*actors)->userData;
		std::string objectName = m_ActorNames[index];

		std::stringstream l_Call;
		l_Call << "OnSleep" << "('" << objectName << "')";
		std::string l_Code = l_Call.str();
		CEngine::GetSingleton().GetLuabindManager()->RunCode(l_Code);
	}
}

void CPhysXManagerImplementation::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
	for (physx::PxU32 i = 0; i<nbPairs; ++i)
	{
		size_t indexOne = (size_t)pairHeader.actors[0]->userData;
		size_t indexTwo = (size_t)pairHeader.actors[1]->userData;

		std::string objectOneName = m_ActorNames[indexOne];
		std::string objectTwoName = m_ActorNames[indexTwo];

		std::stringstream l_Call;
		l_Call << "OnContact" << "('" << objectOneName << "','" << objectTwoName << "')";
		std::string l_Code = l_Call.str();
		CEngine::GetSingleton().GetLuabindManager()->RunCode(l_Code);

		//printf("\"%s\" in contact with \"%s\"",objectOneName.c_str(),objectTwoName.c_str());
	}
}

void CPhysXManagerImplementation::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
	for (physx::PxU32 i = 0; i<count; ++i)
	{
		//ignore pairs when shapes have been deleted
		if ((pairs[i].flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER)))
			continue;

		size_t indexTrigger = (size_t)pairs[i].triggerActor->userData;
		size_t indexActor = (size_t)pairs[i].otherActor->userData;

		std::string triggerName = m_ActorNames[indexTrigger];
		std::string actorName = m_ActorNames[indexActor];

		std::stringstream l_Ss;

		l_Ss << "OnCollide" << triggerName << "('" << actorName << "')";

		std::string l_Code = l_Ss.str();
		CEngine::GetSingleton().GetLuabindManager()->RunCode(l_Code);
	}

}

void CPhysXManagerImplementation::onControllerHit(const physx::PxControllersHit& hit)
{
	hit.other;
	hit.controller;
}

void CPhysXManagerImplementation::onObstacleHit(const physx::PxControllerObstacleHit& hit)
{
	hit.controller;
}

void CPhysXManagerImplementation::onShapeHit(const physx::PxControllerShapeHit& hit)
{
	hit.actor;
	hit.controller;
}
void CPhysXManagerImplementation::CreateCharacterController(const std::string &Name, const float &Height, const float &Radius, const float &Density, const Vect3f &Position, const std::string &MaterialName, float StaticFriction, float DynamicFriction, float Restitution)
{
	CheckMapAndVectors();
	physx::PxCapsuleControllerDesc l_Desc;
	l_Desc.height = Height;
	l_Desc.radius = Radius;
	l_Desc.climbingMode = physx::PxCapsuleClimbingMode::eCONSTRAINED;
	l_Desc.slopeLimit = cosf(3.1415f / 6); //The maximum slope which the character can walk up. //cosf(3.1415f / 6);
	l_Desc.stepOffset = 0.001f; //maximum height of an obstacle which the character can climb.
	l_Desc.contactOffset = 0.001f;
	l_Desc.density = Density;
	l_Desc.reportCallback = this;
	l_Desc.position = physx::PxExtendedVec3(Position.x, Position.y + Radius + Height * 0.5f, Position.z);
	physx::PxMaterial* l_Material = RegisterMaterial(MaterialName,StaticFriction,DynamicFriction,Restitution);
	l_Desc.material = l_Material;
	size_t l_Index = m_CharacterControllers.size();
	l_Desc.userData = (void*)l_Index;

	physx::PxController* l_Controller = m_ControllerManager->createController(l_Desc);
	m_CharacterControllers.insert(std::pair<std::string, physx::PxController*>(Name, l_Controller));

	size_t l_ActorIndex = m_Actors.size();
	l_Controller->getActor()->userData = (void*)l_ActorIndex;
	SaveActorData(l_ActorIndex, Name, Position, Quatf(0.0f, 0.0f, 0.0f, 1.0f), l_Controller->getActor());
}

void CPhysXManagerImplementation::Reload()
{
	for (auto it = m_CharacterControllers.begin(); it != m_CharacterControllers.end(); ++it)
	{
		if (it->second != nullptr)
		{
			std::string l_CControllerName = it->first;

			it->second->release();
			it->second = nullptr;

			for (auto it = m_ActorIndexs.begin(); it != m_ActorIndexs.end(); ++it)
			{
				if (it->first == l_CControllerName)
				{
					size_t index = it->second;
					if (m_Actors[index] != nullptr)
					{
						m_Actors[index] = nullptr;
					}
				}
			}

		}
	}
	m_CharacterControllers.clear();

	for (auto it = m_Materials.begin(); it != m_Materials.end(); ++it)
	{
		if (it->second != nullptr)
		{
			it->second->release();
			it->second = nullptr;
		}
	}
	m_Materials.clear();

	for (size_t i = 0; i<m_Actors.size(); ++i)
	{
		if (m_Actors[i] != nullptr)
		{
			m_Actors[i]->release();
			m_Actors[i] = nullptr;
		}
	}
	m_Actors.clear();

	m_ActorIndexs.clear();
	m_ActorNames.clear();
	m_ActorPositions.clear();
	m_ActorOrientations.clear();
}

bool CPhysXManagerImplementation::filter(const physx::PxController& a, const physx::PxController& b)
{

	std::string l_actorA = a.getActor()->getName();
	return true;
}
