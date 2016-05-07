#ifndef _CPHYSXMANAGERIMPLEMENTATION_H
#define _CPHYSXMANAGERIMPLEMENTATION_H

#include <PxPhysicsAPI.h>
#include "PhysXManager.h"

#include <map>
#include <vector>

class CPhysXManagerImplementation:
	public CPhysXManager,
	public physx::PxSimulationEventCallback,
	public physx::PxUserControllerHitReport
{

public:
	CPhysXManagerImplementation();
	virtual ~CPhysXManagerImplementation();
	void Reload();

	//PxSimulationEventCallback
	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count);
	void onWake(physx::PxActor** actors, physx::PxU32 count);
	void onSleep(physx::PxActor** actors, physx::PxU32 count);
	void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);

	// PxUserContrrolerHitReport
	void onShapeHit(const physx::PxControllerShapeHit& hit){};
	void onControllerHit(const physx::PxControllersHit& hit){};
	void onObstacleHit(const physx::PxControllerObstacleHit& hit){};
	
	void CreateCharacterController(const std::string &Name, const float &Height, const float &Radius, const float &Density, const Vect3f &Position, const std::string &MaterialName);
};

#endif