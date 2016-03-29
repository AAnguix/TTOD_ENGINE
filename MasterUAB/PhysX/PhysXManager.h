#ifndef _PHYSXMANAGER_H
#define _PHYSXMANAGER_H

#include <string>
#include <map>
#include <vector>
#include "Math\Vector3.h"
#include "Math\Quaternion.h"
#include <assert.h>

#define USE_PHYSX_DEBUG 1

namespace physx
{
	class PxFoundation;
	class PxPhysics;
	class PxDefaultCpuDispatcher;
	class PxScene;
	class PxCooking;
	class PxMaterial;
	class PxActor;
	class PxControllerManager;
	class PxController;

	class PxShape;
	class PxRigidActor;

	class PxGeometry;

	namespace debugger
	{
		namespace comm
		{
			class PvdConnection;
		}
	}

	typedef debugger::comm::PvdConnection PxVisualDebuggerConnection;
}

struct RaycastData;

class CPhysXManager
{

private:
	float m_LeftOverSeconds;

protected:
	CPhysXManager();
	
	physx::PxFoundation *m_Foundation;
	physx::PxPhysics *m_PhysX;

	# if USE_PHYSX_DEBUG
		physx::PxVisualDebuggerConnection *m_DebugConnection;
	# endif

	physx::PxDefaultCpuDispatcher *m_Dispatcher;
	physx::PxScene *m_Scene;
	physx::PxCooking *m_Cooking;
	physx::PxControllerManager *m_ControllerManager;

	std::map<std::string,physx::PxMaterial*> m_Materials;
	std::map<std::string,size_t> m_ActorIndexs;
	std::vector<std::string> m_ActorNames;
	std::vector<Vect3f> m_ActorPositions;
	std::vector<Quatf> m_ActorOrientations;
	std::vector<physx::PxActor*> m_Actors;
	std::map<std::string,physx::PxController*> m_CharacterControllers;

	void SaveActorData(size_t Index, const std::string ActorName,const Vect3f &Position, const Quatf &Orientation, physx::PxActor *Actor);
	size_t GetActorIndex(const std::string& ActorName) const;
	void CheckMapAndVectors();
	physx::PxActor* IsRigidDynamic(const std::string& ActorName);

public:
	struct SRaycastData
	{
		Vect3f m_Position;
		Vect3f m_Normal;
		float m_Distance;
		std::string m_ActorName;
	};

	struct SActorData
	{
		Vect3f m_Position;
		Quatf m_Orientation;
	};

	struct SCharacterControllerData
	{
		Vect3f m_FootPosition;
		Vect3f m_LinearVelocity;
		SCharacterControllerData(Vect3f FootPosition, Vect3f LinearVelocity):m_FootPosition(FootPosition),m_LinearVelocity(LinearVelocity){}
	};

	static CPhysXManager* CreatePhysxManager();
	virtual ~CPhysXManager();
	void Update(float ElapsedTime);
	virtual void Reload();

	/*Testing*/
	void CreateSTSphere(const std::string &Name, const float &Radius, const std::string Material, const Vect3f &Position, const Quatf &Orientation, int Group);
	void CreateSTBOX(const std::string &Name, const Vect3f &Size, const std::string Material, const Vect3f &Position, const Quatf &Orientation, int Group);
	physx::PxShape* CreateStaticShape(const std::string &Name, physx::PxGeometry &Geometry, const std::string Material, const Vect3f &Position, const Quatf &Orientation, int Group);
	physx::PxShape* CreateDinamicShape(const std::string &Name, physx::PxGeometry Geometry, const std::string &Material, const Vect3f &Position, const Quatf &Orientation, float Density, int Group, bool IsKinematic = false);
	size_t AddActor(const std::string &ActorName, const Vect3f &Position, const Quatf &Orientation, physx::PxActor* Actor);
	void RegisterActor(const std::string &ActorName, physx::PxShape* Shape, physx::PxRigidActor* Body, Vect3f Position, Quatf Orientation, int Group);
	/**/

	void RegisterMaterial(const std::string &Name, float StaticFriction, float DynamicFriction,float Restitution);
	physx::PxMaterial* GetMaterial(const std::string &MaterialName);
	
	void CreatePlane(const std::string &Name, const Vect3f &Normal, float Distance,  const Vect3f &Position,  const Quatf &Orientation, const std::string &MaterialName);
	void CreateRigidStatic(const std::string &Name, const Vect3f Size, const Vect3f &Position, const Quatf &Orientation, const std::string &MaterialName);
	void CreateRigidDynamic(const std::string &Name, const Vect3f Size, const Vect3f &Position, const Quatf &Orientation, const std::string &MaterialName, float Density, bool Trigger);
	void CreateConvexMesh(std::vector<Vect3f> Vertices,const std::string &MeshName, const Vect3f &Position, const Quatf &Orientation, const std::string &MaterialName);
	
	bool RemoveActor(const std::string &ActorName);
	
	virtual void CreateCharacterController(const std::string &Name, float Height, float Radius, float Density, Vect3f &Position, const std::string &MaterialName);
	Vect3f MoveCharacterController(const std::string& CharacterControllerName, const Vect3f &Movement, float ElapsedTime);
	Vect3f GetCharacterControllerPosition(const std::string& CharacterControllerName);
	Vect3f GetCharacterControllerFootPosition(const std::string& CharacterControllerName);

	void MoveKinematicActor(const std::string& ActorName, const Vect3f &Position);
	void MoveKinematicActor(const std::string& ActorName, const Quatf &Rotation);
	void MoveKinematicActor(const std::string& ActorName, const Vect3f &Position, const Quatf &Rotation);

	bool Raycast(const Vect3f& Origin, const Vect3f& End, int FilterMask, SRaycastData* result_ = nullptr);
	void SetShapeAsTrigger(const std::string &ActorName);

	Vect3f GetActorPosition(const std::string& ActorName) const
	{
		return m_ActorPositions[GetActorIndex(ActorName)];
	}

	Quatf GetActorOrientation(const std::string& ActorName) const
	{
		return m_ActorOrientations[GetActorIndex(ActorName)];
	}

	SActorData GetActorPositionAndOrientation(const std::string& ActorName) const
	{
		SActorData l_ActorData;
		l_ActorData.m_Position = m_ActorPositions[GetActorIndex(ActorName)];
		l_ActorData.m_Orientation = m_ActorOrientations[GetActorIndex(ActorName)];

		return l_ActorData;
	}
};

#endif