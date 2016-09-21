#include "PhysXManagerImplementation.h"
#include <Windows.h>
#include <sstream>

#include "Engine\Engine.h"
#include "Log\Log.h"
#include "Engine\EngineSettings.h"

#include "LuabindManager\LuabindManager.h"
#include "Components\Collider.h"
#include "Components\CharacterCollider.h"
#include "Components\Script\ScriptManager.h"
#include "Components\Script\Script.h"
#include "Components\LuaComponent.h"

static physx::PxDefaultErrorCallback gDefaultErrorCallback;
static physx::PxDefaultAllocator gDefaultAllocatorCallback;

#if USE_PHYSX_DEBUG
#define PVD_HOST "127.0.0.1"
#endif

physx::PxFilterFlags contactReportFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	/*
	if ((physx::PxGetFilterObjectType(attributes0) == physx::PxFilterObjectType::eRIGID_STATIC || physx::PxGetFilterObjectType(attributes1) == physx::PxFilterObjectType::eRIGID_STATIC)
	&& (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1)))
	{
	return physx::PxFilterFlag::eSUPPRESS;
	}
	else if ((physx::PxFilterObjectIsKinematic(attributes0)) && (physx::PxFilterObjectIsKinematic(attributes1)))
	{
	return physx::PxFilterFlag::eDEFAULT;
	}

	else
	{
	pairFlags =
	physx::PxPairFlag::eCONTACT_DEFAULT
	| physx::PxPairFlag::eTRIGGER_DEFAULT
	| physx::PxPairFlag::eNOTIFY_CONTACT_POINTS
	| physx::PxPairFlag::eSOLVE_CONTACT;
	}

	return physx::PxFilterFlag::eDEFAULT;
	*/
	//_________________________________

	// let triggers through
	if ((physx::PxGetFilterObjectType(attributes0) == physx::PxFilterObjectType::eRIGID_STATIC || physx::PxGetFilterObjectType(attributes1) == physx::PxFilterObjectType::eRIGID_STATIC)
		&& (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1)))
	{
		return physx::PxFilterFlag::eSUPPRESS;
	}
	else if (physx::PxFilterObjectIsTrigger(attributes0) && physx::PxFilterObjectIsTrigger(attributes1))
	{
		return physx::PxFilterFlag::eSUPPRESS;
	}
	else if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
		return physx::PxFilterFlag::eDEFAULT;
	}
	else if ((physx::PxFilterObjectIsKinematic(attributes0)) && (physx::PxFilterObjectIsKinematic(attributes1)))
	{
		return physx::PxFilterFlag::eSUPPRESS;
	}
	//else if (physx::PxGetFilterObjectType(attributes0) == physx::PxFilterObjectType::eUNDEFINED)
	// generate contacts for all that were not filtered above
	pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

	// trigger the contact callback for pairs (A,B) where
	// the filtermask of A contains the ID of B and vice versa.
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return physx::PxFilterFlag::eDEFAULT;
};
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
	//sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	sceneDesc.flags = physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
	m_Scene = m_PhysX->createScene(sceneDesc);
	assert(m_Scene);

	m_Scene->setSimulationEventCallback(this);

	m_ControllerManager = PxCreateControllerManager(*m_Scene);
	m_ControllerManager->setOverlapRecoveryModule(true);
}

void CPhysXManagerImplementation::RemoveComponents()
{
	for (size_t i = 0; i < m_CharacterColliderComponents.size(); ++i)
	{
		delete m_CharacterColliderComponents[i];
		m_CharacterColliderComponents[i] = NULL;
	}
	m_CharacterColliderComponents.clear();

	for (size_t i = 0; i < m_ColliderComponents.size(); ++i)
	{
		delete m_ColliderComponents[i];
		m_ColliderComponents[i] = NULL;
	}
	m_ColliderComponents.clear();
}

void CPhysXManagerImplementation::CreateScene()
{
	SYSTEM_INFO l_SystemInfo;
	GetSystemInfo(&l_SystemInfo);
	DWORD l_CPUs = l_SystemInfo.dwNumberOfProcessors;
	m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(l_CPUs);

	physx::PxSceneDesc sceneDesc(m_PhysX->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.cpuDispatcher = m_Dispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	sceneDesc.flags = physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS | physx::PxSceneFlag::eENABLE_KINEMATIC_PAIRS;
	m_Scene = m_PhysX->createScene(sceneDesc);
	assert(m_Scene);

	m_Scene->setSimulationEventCallback(this);

	m_ControllerManager = PxCreateControllerManager(*m_Scene);
	m_ControllerManager->setOverlapRecoveryModule(true);
}


void CPhysXManagerImplementation::DestroyScene()
{
	if (m_Scene && !Simulating())
	{
		Destroy();
	}
}

void CPhysXManagerImplementation::Destroy()
{
	RemoveComponents();

	for (auto it = m_CharacterControllers.begin(); it != m_CharacterControllers.end(); ++it)
	{
		physx::PxController* l_Controller = it->second;
		l_Controller->release();
		l_Controller = nullptr;
		int l_CControllerIndex = GetActorIndex(it->first);
		m_Actors[l_CControllerIndex] = nullptr;
	}
	m_CharacterControllers.clear();

	for (size_t i = 0; i < m_Actors.size(); ++i)
	{
		if (m_Actors[i])
		{
			m_Actors[i]->release();
			m_Actors[i] = nullptr;
		}
	}
	m_Actors.clear();

	std::map<std::string, physx::PxMaterial*>::iterator it;
	for (it = m_Materials.begin(); it != m_Materials.end(); ++it)
	{
		it->second->release();
		it->second = nullptr;
	}
	m_Materials.clear();

	m_ActorIndexs.clear();
	m_ActorNames.clear();
	m_ActorPositions.clear();
	m_ActorOrientations.clear();

	m_Shapes.clear();
}

CPhysXManagerImplementation::~CPhysXManagerImplementation()
{
	RemoveComponents();

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

		if (pairs[i].status == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			CScript* l_Script = CEngine::GetSingleton().GetScriptManager()->GetScript(triggerName);
			if (l_Script != nullptr)
			{
				if (CEngine::GetSingleton().GetEngineSettings()->GetDebugOptions().m_DebugPhysXTriggers)
					LOG("TriggerEnter: "+actorName+" -> "+triggerName);
				l_Script->GetLuaComponent()->OnTriggerEnter(actorName);
			}
		}
		if (pairs[i].status == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			CScript* l_Script = CEngine::GetSingleton().GetScriptManager()->GetScript(triggerName);
			if (l_Script != nullptr)
			{
				if (CEngine::GetSingleton().GetEngineSettings()->GetDebugOptions().m_DebugPhysXTriggers)
					LOG("TriggerExit: " + actorName + " -> " + triggerName);
				l_Script->GetLuaComponent()->OnTriggerExit(actorName);
			}
		}
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
	physx::PxRigidActor* l_Actor = hit.actor;
	size_t index = (size_t)l_Actor->userData;
	std::string l_ActorName = m_ActorNames[index];

	physx::PxController* l_CharacterController = hit.controller;
	size_t l_CharacterControllerIndex = (size_t)l_CharacterController->getActor()->userData;
	std::string l_CharacterControllerName = m_ActorNames[l_CharacterControllerIndex];

	CScript* l_Script = CEngine::GetSingleton().GetScriptManager()->GetScript(l_CharacterControllerName);
	if (l_Script != nullptr)
		l_Script->GetLuaComponent()->OnShapeHit(l_ActorName);

	if (l_ActorName == "CATRigTail4")
	{
		physx::PxVec3 l_PxDir = hit.dir;
		physx::PxF32 l_PxLength = hit.length;
		physx::PxExtendedVec3 l_PosW = hit.worldPos;
	}
}
void CPhysXManagerImplementation::CreateCharacterController(const std::string &Name, const float &Height, const float &Radius, const float &Density, const Vect3f &Position, const std::string &MaterialName, float StaticFriction, float DynamicFriction, float Restitution)
{
	CheckMapAndVectors();
	physx::PxCapsuleControllerDesc l_Desc;
	l_Desc.height = Height;
	l_Desc.radius = Radius;
	l_Desc.climbingMode = physx::PxCapsuleClimbingMode::eCONSTRAINED;
	l_Desc.slopeLimit = cosf(3.1415f / 6); //The maximum slope which the character can walk up. //cosf(3.1415f / 6);
	l_Desc.stepOffset = 0.3f; //maximum height of an obstacle which the character can climb.
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
	RemoveComponents();

	for (auto it = m_CharacterControllers.begin(); it != m_CharacterControllers.end(); ++it)
	{
		if (it->second != nullptr)
		{
			std::string l_CControllerName = it->first;

			it->second->release();
			it->second = nullptr;

			/*for (auto it = m_ActorIndexs.begin(); it != m_ActorIndexs.end(); ++it)
			{
				if (it->first == l_CControllerName)
				{
					size_t index = it->second;
					if (m_Actors[index] != nullptr)
					{
						m_Actors[index] = nullptr;
					}
				}
			}*/

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

	#ifdef _DEBUG
		CEngine::GetSingleton().GetLogManager()->Log("PhysxManager reloaded");
	#endif	
}

bool CPhysXManagerImplementation::filter(const physx::PxController& a, const physx::PxController& b)
{

	std::string l_actorA = a.getActor()->getName();
	return true;
}
