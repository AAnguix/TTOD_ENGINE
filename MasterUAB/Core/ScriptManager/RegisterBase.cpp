#include "ScriptManager\ScriptManager.h"

#include <luabind/luabind.hpp>

#include <luabind/operator.hpp>
#include <luabind/iterator_policy.hpp>


#include "Engine.h"

#include "Utils\3DElement.h"
#include "Utils\Named.h"
#include "Math\Vector2.h"
#include "Math\Vector3.h"
#include "Math\Vector4.h"
#include "Utils\EmptyPointerClass.h"
#include <cmath>

using namespace luabind;

#define LUA_STATE CEngine::GetSingleton().GetScriptManager()->GetLuaState()
#define REGISTER_LUA_FUNCTION(FunctionName,AddrFunction) {luabind::module(LUA_STATE) [ luabind::def(FunctionName,AddrFunction) ];}

void CScriptManager::RegisterBase()
{
	module(LUA_STATE) 
	[
		class_<C3DElement>("C3DElement")
		.def(constructor<>())
		.def(constructor<const Vect3f>())
		.def(constructor<const Vect3f,float,float,float>())
		.def("GetYaw", &C3DElement::GetYaw)
		.def("SetYaw", &C3DElement::SetYaw)
		.def("GetPitch", &C3DElement::GetPitch)
		.def("SetPitch", &C3DElement::SetPitch)
		.def("GetRoll", &C3DElement::GetRoll)
		.def("SetRoll", &C3DElement::SetRoll)
		.def("GetForward", &C3DElement::GetForward)
		.def("GetTranslation", &C3DElement::GetTranslation)
		.def("GetPosition", &C3DElement::GetPosition)
		.def("SetPosition", &C3DElement::SetPosition)
		.def("GetPrevPosition", &C3DElement::GetPrevPosition)
		.def("GetScale", &C3DElement::GetScale)
		.def("SetScale", &C3DElement::SetScale)
		.def("GetVisible", &C3DElement::GetVisible)
		.def("SetVisible", &C3DElement::SetVisible)
		.def("GetTransform", &C3DElement::GetTransform)
		.def("SetYawPitchRoll", &C3DElement::SetYawPitchRoll)
	];
	
	module(LUA_STATE)
	[
		class_<CXMLTreeNode>("CXMLTreeNode")
		.def(constructor<>())
		.def("LoadFile", &CXMLTreeNode::LoadFile)
		.def("GetBoolKeyword", &CXMLTreeNode::GetBoolKeyword)
		.def("GetBoolProperty", &CXMLTreeNode::GetBoolProperty)
		.def("GetFloatKeyword", &CXMLTreeNode::GetFloatKeyword)
		.def("GetFloatProperty", &CXMLTreeNode::GetFloatProperty)
		.def("GetIntKeyword", &CXMLTreeNode::GetIntKeyword)
		.def("GetIntProperty", &CXMLTreeNode::GetIntProperty)
		.def("GetName", &CXMLTreeNode::GetName)
		.def("GetNumChildren", &CXMLTreeNode::GetNumChildren)
		.def("GetPszISOProperty", &CXMLTreeNode::GetPszISOProperty)
		.def("GetPszKeyword", &CXMLTreeNode::GetPszKeyword)
		.def("GetPszProperty", &CXMLTreeNode::GetPszProperty)
		.def("GetVect2fProperty", &CXMLTreeNode::GetVect2fProperty)
		.def("GetVect3fProperty", &CXMLTreeNode::GetVect3fProperty)
		.def("GetVect4fProperty", &CXMLTreeNode::GetVect4fProperty)
		.def("Exists", &CXMLTreeNode::Exists)
		.def("[]", &CXMLTreeNode::operator[])
		.def("GetChild", &CXMLTreeNode::operator())
	];

	module(LUA_STATE) 
	[
		class_<CNamed>("CNamed")
		.def(constructor<CXMLTreeNode>())
		.def("GetName", &CNamed::GetName)
		.def("SetName", &CNamed::SetName)
	];

	module(LUA_STATE)
	[
		class_< Quatf >("Quatf")
		.def(constructor<>())
		.def(constructor<float, float,float,float>())
		.def(constructor<float, float, float>())
		.def("GetRadians", &Quatf::GetRadians)
		.def_readwrite("x", &Quatf::x)
		.def_readwrite("y", &Quatf::y)
		.def_readwrite("z", &Quatf::z)
		.def_readwrite("w", &Quatf::w)
	];

	module(LUA_STATE) 
	[  
		class_< Vect2f >("Vect2f")   
		.def(constructor<float, float>()) 
		.def(const_self + const_self)   
		.def(const_self - const_self)   
		.def(const_self * const_self)  
		.def(const_self == const_self)  
		.def_readwrite("x", &Vect2f::x) 
		.def_readwrite("y", &Vect2f::y)  
	]; 

	module(LUA_STATE) 
	[  
		class_< Vect3f >("Vect3f")   
		.def(constructor<float, float, float>()) 
		.def_readwrite("x", &Vect3f::x) 
		.def_readwrite("y", &Vect3f::y)  
		.def_readwrite("z", &Vect3f::z) 

		.def(const_self + const_self)   
		.def(const_self - const_self)   
		.def(const_self * other<const float>())
		.def(const_self / other<const float>())
		.def(const_self + other<const float>())
		.def(const_self - other<const float>())
		.def("cross", &Vect3f::operator^)
		.def("Set", &Vect3f::Set)
		.def("SetZero", &Vect3f::SetZero)

		.def("SetFromPolar", &Vect3f::SetFromPolar)
		.def("GetPolar", &Vect3f::GetPolar)

		.def(const_self * const_self) //P.Escalar  
		.def(const_self ^ const_self) //P.Vectorial
		.def(const_self == const_self)

		.def("IsEqualEpsilon", &Vect3f::IsEqualEpsilon)
		.def("IsNotEqualEpsilon", &Vect3f::IsNotEqualEpsilon)

		.def("Scale", &Vect3f::Scale)
		.def("GetScaled", &Vect3f::GetScaled)

		.def("SetIfMinComponents", &Vect3f::SetIfMinComponents)
		.def("SetIfMaxComponents", &Vect3f::SetIfMaxComponents)

		.def("GetProjXY", &Vect3f::GetProjXY)
		.def("GetProjYZ", &Vect3f::GetProjYZ)
		.def("GetProjZX", &Vect3f::GetProjZX)

		.def("Normalize", &Vect3f::Normalize)
		.def("GetNormalized", &Vect3f::GetNormalized)
		.def("Length", &Vect3f::Length)
		.def("SquaredLength", &Vect3f::SquaredLength)
		.def("Distance", &Vect3f::Distance)
		.def("SqDistance", &Vect3f::SqDistance)
		
		.def("RotateX", &Vect3f::RotateX)
		.def("GetRotatedX", &Vect3f::GetRotatedX)
		.def("RotateY", &Vect3f::RotateY)
		.def("GetRotatedY", &Vect3f::GetRotatedY)
		.def("RotateZ", &Vect3f::RotateZ)
		.def("GetRotatedZ", &Vect3f::GetRotatedZ)
		.def("GetAngleX", &Vect3f::GetAngleX)
		.def("GetAngleY", &Vect3f::GetAngleY)
		.def("GetAngleZ", &Vect3f::GetAngleZ)
		.def("GetAngles", &Vect3f::GetAngles)

		.def("Lerp", &Vect3f::Lerp)
		.def("GetLerp", &Vect3f::GetLerp)

	]; 

	module(LUA_STATE) [  
		class_< Vect4f >("Vect4f")   
		.def(constructor<float, float, float, float>()) 
		.def(const_self + const_self)   
		.def(const_self - const_self)   
		.def(const_self * const_self) 
		.def(const_self == const_self)   
		.def_readwrite("x", &Vect4f::x) 
		.def_readwrite("y", &Vect4f::y)  
		.def_readwrite("z", &Vect4f::z)  
		.def_readwrite("z", &Vect4f::w)  
	]; 

	/*module(LUA_STATE)
	[  
		class_< Mat33f >("Mat33f")   
		.def(constructor<>()) 
		.def(constructor<Mat33f>()) 
		.def(constructor<const float,const float,const float,const float,const float,const float,const float,const float,const float>()) 
		.def(constructor<const Vect3f,const Vect3f,const Vect3f>()) 
		.def(constructor<const float,const float,const float>()) 
		.def("reset_rotation", & Mat33f::ResetRotation)
		.def("reset_scale", & Mat33f::ResetScale)
		.def("set_identity", & Mat33f::SetIdentity)
		.def("set_zero", & Mat33f::SetZero)
		.def(const_self + const_self)   
		.def(const_self - const_self)   
		.def(const_self * const_self) 
		.def(const_self * other<const float>())
		.def(const_self / other<const float>()) 
		.def(const_self * other<const Vect3f>())
		.def(const_self ^ other<const Vect3f>())
		.def(const_self == const_self)
		.def("transform_vector", & Mat33f::TransformVector)
		.def("get_transform_vector", & Mat33f::GetTransformVector)
		.def("transform_array_vectors", & Mat33f::TransformArrayVectors)
		.def("get_inverted", & Mat33f::GetInverted)
		.def("get_inverted_rs", & Mat33f::GetInvertedRS)
		.def("get_transposed", & Mat33f::GetTransposed)
		.def("get_rotated_by_anglex", & Mat33f::GetRotedByAngleX)
		.def("get_rotated_by_angley", & Mat33f::GetRotedByAngleY)
		.def("get_rotated_by_anglez", & Mat33f::GetRotedByAngleZ)
		.def("get_rotated_by_anglexz", & Mat33f::GetRotedByAnglesXZ)
		.def("get_rotated_by_angleyxz", & Mat33f::GetRotedByAnglesYXZ)
		.def("get_scaled", & Mat33f::GetScaled)
		.def("align_yx", & Mat33f::AlignYX)
		.def("align_yz", & Mat33f::AlignYZ)
		.def("align_xz", & Mat33f::AlignXZ)
		.def("determinant", & Mat33f::Determinant)
		.def("is_orthogonal_epsilon", & Mat33f::IsOrthogonalEpsilon)
		.def("is_orthonormal_epsilon", & Mat33f::IsOrthonormalEpsilon)
	];*/

	/*module(LUA_STATE)
	[  
		class_< Mat34f >("Mat34f")   
		.def(constructor<>()) 
	];*/

	/*
	module(LUA_STATE) [  
		class_< Mat44f >("Mat44f")   
		.def(constructor<>()) 
		.def(constructor<Mat44f>())
		.def(constructor<Mat33f>()) 
		.def(constructor<Mat34f>()) 
		.def(constructor<const float,const float,const float,const float,const float,const float,const float,const float,const float,const float,const float,const float>()) 
		.def(constructor<const Vect3f,const Vect3f,const Vect3f,const Vect3f>()) 
		.def(constructor<const float,const float,const float>()) 
	
	];*/

	module(LUA_STATE) 
	[
		class_<CEmptyPointerClass>("CEmptyPointerClass")
	];

}