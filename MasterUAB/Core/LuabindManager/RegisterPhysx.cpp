#include "LuabindManager\LuabindManager.h"
#include "LuabindManager\LuabindMacros.h"
#include <luabind/luabind.hpp>

#include <luabind/operator.hpp>
#include <luabind/iterator_policy.hpp>


#include "Engine\Engine.h"
#include "PhysXManager.h"

#include "Components\Collider.h"
#include "Components\CharacterCollider.h"

using namespace luabind;

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
		.def("GeometryQuery", &CPhysXManager::GeometryQuery)

		.def("MoveKinematicActor", (void(CPhysXManager::*)(const std::string&, const Vect3f&))&CPhysXManager::MoveKinematicActor)
		.def("MoveKinematicActor", (void(CPhysXManager::*)(const std::string&, const Quatf&))&CPhysXManager::MoveKinematicActor)
		.def("MoveKinematicActor", (void(CPhysXManager::*)(const std::string&, const Vect3f&, const Quatf&))&CPhysXManager::MoveKinematicActor)
		.def("Reload", &CPhysXManager::Reload)
		.def("DisplacementCharacterController", &CPhysXManager::DisplacementCharacterController)

		.def("SetMaterialStaticFriction", &CPhysXManager::SetMaterialStaticFriction)
		.def("SetMaterialDynamicFriction", &CPhysXManager::SetMaterialDynamicFriction)
		.def("SetMaterialRestitution", &CPhysXManager::SetMaterialRestitution)

		.def("CreateBoxLua", &CPhysXManager::CreateBoxLua)
		.def("CreateSphereLua", &CPhysXManager::CreateSphereLua)
		
		.def("CreateBoxTrigger", (bool(CPhysXManager::*)(const std::string&, const Vect3f&, const std::string&, const Vect3f&, const Quatf&, const std::string&))&CPhysXManager::CreateBoxTrigger)
		.def("CreateSphereTrigger", (bool(CPhysXManager::*)(const std::string&, float, const std::string&, const Vect3f&, const Quatf&, const std::string&))&CPhysXManager::CreateSphereTrigger)
		.def("DeleteTrigger", &CPhysXManager::DeleteTrigger)
		
		.def("AddColliderComponent", (CCollider*(CPhysXManager::*)(const std::string&, CLuaGameObjectHandle*))&CPhysXManager::AddColliderComponent)

		.def("AddCharacterColliderComponent", &CPhysXManager::AddCharacterColliderComponent)
		.def("RemoveComponents", &CPhysXManager::RemoveComponents)
		.def("ApplyForce", &CPhysXManager::ApplyForce)
		.def("ChangeKinematicState", &CPhysXManager::ChangeKinematicState)
		.def("ChangeGravityState", &CPhysXManager::ChangeGravityState)

		.def("ChangeShapeTriggerState", (void(CPhysXManager::*)(const std::string&, bool))&CPhysXManager::ChangeShapeTriggerState)
		.def("ChangeShapeTriggerState", (void(CPhysXManager::*)(const std::string&, bool, bool))&CPhysXManager::ChangeShapeTriggerState)

		.def("CreateDynamicActor", &CPhysXManager::CreateDynamicActor)

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
			.def(constructor<>())
			.def_readwrite("m_Position", &CPhysXManager::SRaycastData::m_Position)
			.def_readwrite("m_Normal", &CPhysXManager::SRaycastData::m_Normal)
			.def_readwrite("m_Distance", &CPhysXManager::SRaycastData::m_Distance)
			.def_readwrite("m_ActorName", &CPhysXManager::SRaycastData::m_ActorName)
		];
	module(LUA_STATE)
		[
			class_<CPhysXManager::SActorData>("SActorData")
		];
}