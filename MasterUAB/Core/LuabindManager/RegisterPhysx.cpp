#include "LuabindManager\LuabindManager.h"

#include <luabind/luabind.hpp>

#include <luabind/operator.hpp>
#include <luabind/iterator_policy.hpp>


#include "Engine.h"
#include "PhysXManager.h"

#include "Components\Collider.h"
#include "Components\CharacterCollider.h"

using namespace luabind;

#define LUA_STATE CEngine::GetSingleton().GetLuabindManager()->GetLuaState()
#define REGISTER_LUA_FUNCTION(FunctionName,AddrFunction) {luabind::module(LUA_STATE) [ luabind::def(FunctionName,AddrFunction) ];}


void CLuabindManager::RegisterPhysics()
{
	module(LUA_STATE) 
	[
		class_<CPhysXManager>("CPhysXManager")
		.def("CreateCharacterController", &CPhysXManager::CreateCharacterController)
		.def("MoveCharacterController", &CPhysXManager::MoveCharacterController)
		.def("GetCharacterControllerPosition", &CPhysXManager::GetCharacterControllerPosition)
		.def("GetCharacterControllerFootPosition", &CPhysXManager::GetCharacterControllerFootPosition)
		.def("GetActorPosition", &CPhysXManager::GetActorPosition)
		.def("GetActorOrientation", &CPhysXManager::GetActorOrientation)
		.def("GetActorPositionAndOrientation", &CPhysXManager::GetActorPositionAndOrientation)
	
		.def("Raycast", &CPhysXManager::Raycast)
		.def("MoveKinematicActor", (void(CPhysXManager::*)(const std::string&, const Vect3f&))&CPhysXManager::MoveKinematicActor)
		.def("MoveKinematicActor", (void(CPhysXManager::*)(const std::string&, const Quatf&))&CPhysXManager::MoveKinematicActor)
		.def("MoveKinematicActor", (void(CPhysXManager::*)(const std::string&, const Vect3f&, const Quatf&))&CPhysXManager::MoveKinematicActor)
		.def("Reload", &CPhysXManager::Reload)
		.def("DisplacementCharacterController", &CPhysXManager::DisplacementCharacterController)
		
		.def("SetMaterialStaticFriction", &CPhysXManager::SetMaterialStaticFriction)
		.def("SetMaterialDynamicFriction", &CPhysXManager::SetMaterialDynamicFriction)
		.def("SetMaterialRestitution", &CPhysXManager::SetMaterialRestitution)

		/*.def("CreateBox", &CPhysXManager::CreateBox)
		.def("CreateSphere", &CPhysXManager::CreateSphere)
		.def("CreateCapsule", &CPhysXManager::CreateCapsule)
		.def("CreatePlane", &CPhysXManager::CreatePlane)
		.def("CreateConvexMesh", &CPhysXManager::CreateConvexMesh)
		.def("CreateTriangleMesh", &CPhysXManager::CreateTriangleMesh)*/
		.def("SetShapeAsTrigger", &CPhysXManager::SetShapeAsTrigger)
		/*.def("CreateBoxTrigger", &CPhysXManager::CreateBoxTrigger)
		.def("CreateSphereTrigger", &CPhysXManager::CreateSphereTrigger)*/

		.def("AddColliderComponent", &CPhysXManager::AddColliderComponent)
		.def("AddCharacterColliderComponent", &CPhysXManager::AddCharacterColliderComponent)
		.def("RemoveComponents", &CPhysXManager::RemoveComponents)
		//.def("AddComponent", (CCollider*(CPhysXManager::*)(const std::string&, CMeshInstance*))&CPhysXManager::AddComponent)
		//.def("AddComponent", (CCharacterCollider*(CPhysXManager::*)(const std::string&, CAnimatedInstanceModel*))&CPhysXManager::AddComponent)
		

		//.def("DisplacementCharacterController2", &CPhysXManager::DisplacementCharacterController2)
	];
	module(LUA_STATE) 
	[
		class_<CPhysXManager::SCharacterControllerData>("SCharacterControllerData")
	];
	module(LUA_STATE)
	[
		class_<CPhysXManager::SRaycastData>("SRaycastData")
	];
	module(LUA_STATE)
	[
		class_<CPhysXManager::SActorData>("SActorData")
	];
}