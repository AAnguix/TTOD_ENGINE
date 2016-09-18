#ifndef _PHYSXMANAGER_H
#define _PHYSXMANAGER_H

#include <string>
#include <map>
#include <vector>
#include "Math\Vector3.h"
#include "Math\Quaternion.h"
#include <assert.h>
#include <vector>

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
	class PxRigidBody;
	class PxGeometry;

	class PxConvexMesh;
	class PxTriangleMesh;
	class PxDefaultMemoryInputData;

	class PxDefaultMemoryOutputStream;

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
class CCollider;
class CCharacterCollider;
class CLuaGameObjectHandle;
class CGameObject;

class CPhysXManager
{

private:
	float m_LeftOverSeconds;
	std::vector<const std::string> m_ActorsToBeDeleted;
	void RemoveActors();
	bool RemoveActorToBeDeleted(const std::string &ActorName);

	void UpdateComponents(float ElapsedTime);

	physx::PxShape* GetShape(const std::string &ShapeName);
	void ChangeShapeTriggerState(physx::PxShape* Shape, bool State);

	enum Groups
	{
		GROUP1 = (1 << 0),
		GROUP2 = (1 << 1),
		GROUP3 = (1 << 2),
		GROUP4 = (1 << 3),
		GROUP5 = (1 << 4),
		GROUP6 = (1 << 5),
		GROUP7 = (1 << 6),
		GROUP8 = (1 << 7),
	};
	Groups GetGroup(const std::string &Group);

protected:
	std::vector<CCharacterCollider*> m_CharacterColliderComponents;
	std::vector<CCollider*> m_ColliderComponents;

	CPhysXManager();

	physx::PxFoundation *m_Foundation;
	physx::PxPhysics *m_PhysX;

	# if USE_PHYSX_DEBUG
		physx::PxVisualDebuggerConnection *m_DebugConnection;
	# endif

	physx::PxMaterial* GetMaterial(const std::string &MaterialName);

	/*Physx API*/
	physx::PxDefaultCpuDispatcher *m_Dispatcher;
	physx::PxScene *m_Scene;
	physx::PxCooking *m_Cooking;
	physx::PxControllerManager *m_ControllerManager;

	/*Actors*/
	std::map<std::string, physx::PxMaterial*> m_Materials;
	std::map<std::string, size_t> m_ActorIndexs;
	std::vector<std::string> m_ActorNames;
	std::vector<Vect3f> m_ActorPositions;
	std::vector<Quatf> m_ActorOrientations;
	std::vector<physx::PxActor*> m_Actors;
	std::map<std::string, physx::PxController*> m_CharacterControllers;

	/*Shared Data*/
	std::map<std::string, physx::PxShape*> m_Shapes;
	std::map<std::string, physx::PxConvexMesh*> m_ConvexMeshes;
	std::map<std::string, physx::PxTriangleMesh*> m_TriangleMeshes;
	physx::PxConvexMesh* GetConvexMesh(const std::string &ConvexMeshName, physx::PxDefaultMemoryInputData MemoryInputData);
	physx::PxTriangleMesh* GetTriangleMesh(const std::string &TriangleMeshName, physx::PxDefaultMemoryInputData MemoryInputData);
	physx::PxShape* GenerateShape(const std::string &ShapeName, const physx::PxGeometry &Geometry, const std::string MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, bool IsExclusive);
	bool ReleaseShape(const std::string &ShapeName);

	physx::PxMaterial* RegisterMaterial(const std::string &Name, float StaticFriction, float DynamicFriction, float Restitution);

	/*Cooks meshes*/
	physx::PxConvexMesh* CookConvexMesh(const std::string &CoreName, std::vector<Vect3f> Vertices);
	physx::PxTriangleMesh* CookTriangleMesh(const std::string &CoreName, std::vector<Vect3f> &Vertices, std::vector<unsigned short> &Indices);
	void WriteCookingDataToFile(const std::string &FileName, void *Data, unsigned int DataSize);
	void ReadCookingDataFromFile(const std::string &FileName, void **Data, unsigned int &DataSize);
	/*physx::PxDefaultMemoryOutputStream ReadCookingDataFromFile(const std::string &FileName);*/

	void SaveActorData(size_t Index, const std::string ActorName, const Vect3f &Position, const Quatf &Orientation, physx::PxActor *Actor);
	
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
	float CameraRaycast(const Vect3f& Origin, const Vect3f& End, const float& Length);

	struct SActorData
	{
		Vect3f m_Position;
		Quatf m_Orientation;
	};

	struct SCharacterControllerData
	{
		Vect3f m_FootPosition;
		Vect3f m_LinearVelocity;
		SCharacterControllerData(Vect3f FootPosition, Vect3f LinearVelocity) :m_FootPosition(FootPosition), m_LinearVelocity(LinearVelocity){}
	};

	static CPhysXManager* CreatePhysxManager();
	virtual ~CPhysXManager();
	virtual void Destroy(){};
	void Update(float ElapsedTime);
	virtual void Reload();

	/*Components*/
	CCollider* AddColliderComponent(const std::string &Name, CGameObject* Owner);
	CCollider* AddColliderComponent(const std::string &Name, CLuaGameObjectHandle* Owner);
	void RemoveColliderComponent(CGameObject *Owner);
	CCharacterCollider* AddCharacterColliderComponent(const std::string &Name, CLuaGameObjectHandle* Owner, float Height, float Radius, float Density);
	void RemoveCharacterColliderComponent(CGameObject *Owner);
	virtual void RemoveComponents(){};

	/*Actors*/
	bool CreateStaticActor(const std::string &ActorName, const std::string &ShapeName, const Vect3f &Position, const Quatf &Orientation);
	bool CreateDynamicActor(const std::string &ActorName, const std::string &ShapeName, const Vect3f &Position, const Quatf &Orientation, float Density, bool IsKinematic);
	size_t AddActor(const std::string &ActorName, const Vect3f &Position, const Quatf &Orientation, physx::PxActor* Actor);
	void RegisterActor(const std::string &ActorName, physx::PxShape* Shape, physx::PxRigidActor* Body, Vect3f Position, Quatf Orientation);
	void RegisterActor(const std::string &ActorName, physx::PxShape* Shape, physx::PxRigidBody* Body, Vect3f Position, Quatf Orientation, float Density, bool IsKinematic);
	void RemoveActor(const std::string &ActorName);

	/*Triggers*/
	bool CreateBoxTrigger(const std::string &ActorName, const std::string &ShapeName, const Vect3f &Size, const std::string &MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, const Vect3f &Position, const Quatf &Orientation, const std::string &ActorType);
	bool CreateBoxTrigger(const std::string &TriggerName, const Vect3f &Size, const std::string &Group, const Vect3f &Position, const Quatf &Orientation, const std::string &ActorType);
	bool CreateSphereTrigger(const std::string &ActorName, const std::string &ShapeName, float Radius, const std::string &MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, const Vect3f &Position, const Quatf &Orientation, const std::string &ActorType);
	bool CreateSphereTrigger(const std::string &TriggerName, float Radius, const std::string &Group, const Vect3f &Position, const Quatf &Orientation, const std::string &ActorType);
	bool DeleteTrigger(const std::string &TriggerName);
	
	void CreateBoxLua(const std::string &ShapeName, const Vect3f &Size, const std::string MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, bool IsExclusive);
	void CreateSphereLua(const std::string &ShapeName, float Radius, const std::string MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, bool IsExclusive);

	/*Shapes*/
	physx::PxShape* CreateBox(const std::string &ShapeName, const Vect3f &Size, const std::string MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, bool IsExclusive);
	physx::PxShape* CreateSphere(const std::string &ShapeName, float Radius, const std::string MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, bool IsExclusive);
	physx::PxShape* CreateCapsule(const std::string &ShapeName, float Radius, float HalfHeight, const std::string MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, bool IsExclusive);
	physx::PxShape* CreatePlane(const std::string &ShapeName, const std::string MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, bool IsExclusive);
	physx::PxShape* CreateConvexMesh(const std::string &ShapeName, std::vector<Vect3f> Vertices, const std::string MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, bool IsExclusive);
	physx::PxShape* CreateTriangleMesh(const std::string &ShapeName, std::vector<Vect3f> Vertices, std::vector<unsigned short> Indices, const std::string MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, bool IsExclusive);
	physx::PxShape* CreateTriangleMeshFromFile(const std::string &ShapeName, std::string &Filename, const Vect3f Pos, const float Yaw, const float Pitch, const float Roll, const std::string MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group);
	/*Triangle meshes can't be dynamic*/
	/*Triangle,heightField cant be trigger*/
	/*Plane,triangle,height must be kinematic*/

	/*Materials*/
	void SetMaterialStaticFriction(const std::string &MaterialName, float StaticFriction);
	void SetMaterialDynamicFriction(const std::string &MaterialName, float DynamicFriction);
	void SetMaterialRestitution(const std::string &MaterialName, float Restitution);

	virtual void CreateCharacterController(const std::string &Name, const float &Height, const float &Radius, const float &Density, const Vect3f &Position, const std::string &MaterialName, float StaticFriction, float DynamicFriction, float Restitution);
	Vect3f MoveCharacterController(const std::string& CharacterControllerName, const Vect3f &Movement, float ElapsedTime);
	Vect3f DisplacementCharacterController(const std::string& CharacterControllerName, const Vect3f &Displacement, float ElapsedTime);
	Vect3f GetCharacterControllerPosition(const std::string& CharacterControllerName);
	Vect3f GetCharacterControllerFootPosition(const std::string& CharacterControllerName);

	void MoveKinematicActor(const std::string& ActorName, const Vect3f &Position);
	void MoveKinematicActor(const std::string& ActorName, const Quatf &Rotation);
	void MoveKinematicActor(const std::string& ActorName, const Vect3f &Position, const Quatf &Rotation);

	/*Gameplay*/
	//bool Raycast(const Vect3f& Origin, const Vect3f& End, int FilterMask, SRaycastData* result_ = nullptr);
	bool Raycast(const Vect3f& Origin, const Vect3f& Direction, const float& Length, SRaycastData* Result_);
	void ChangeShapeTriggerState(const std::string &ShapeName, bool State);
	void ChangeShapeTriggerState(const std::string &ShapeName, bool EnableTrigger, bool EnableSimulation);
	bool GeometryQuery(const std::string ActorName, const Vect3f& Position, const Quatf& Orientation, const Vect3f& Direction, const float& Length, SRaycastData* Result_);


	void ApplyForce(const std::string &ActorName, const Vect3f &Force);
	void ChangeGravityState(const std::string& ActorName, bool State);
	void ChangeKinematicState(const std::string& ActorName, bool State);
	
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
	const std::vector<std::string> & GetActorNames() const { return m_ActorNames; }

	physx::PxActor* GetActor(const std::string& ActorName) const;


};

#endif