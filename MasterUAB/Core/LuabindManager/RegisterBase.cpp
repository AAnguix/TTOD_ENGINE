#include "LuabindManager\LuabindManager.h"

#include <luabind/luabind.hpp>

#include <luabind/operator.hpp>
#include <luabind/iterator_policy.hpp>


#include "Engine.h"

#include "XML\XMLTreeNode.h"
#include "Utils\3DElement.h"
#include "Utils\Named.h"
#include "Math\Vector2.h"
#include "Math\Vector3.h"
#include "Math\Vector4.h"
#include "Math\Color.h"
#include "Math\Matrix33.h"
#include "Math\Matrix34.h"
#include "Math\Matrix44.h"
#include "Utils\EmptyPointerClass.h"
#include "Utils\TTODMathUtils.h"
#include "Utils\TTODFileUtils.h"
#include "Utils\TTODXMLWriter.h"

#include <cmath>

using namespace luabind;

#define LUA_STATE CEngine::GetSingleton().GetLuabindManager()->GetLuaState()
#define REGISTER_LUA_FUNCTION(FunctionName,AddrFunction) {luabind::module(LUA_STATE) [ luabind::def(FunctionName,AddrFunction) ];}

void CLuabindManager::RegisterBase()
{
	module(LUA_STATE)
	[
		class_<std::string>("string")
		.def(constructor<>())
		.def(constructor<const char *>())
	];

	module(LUA_STATE)
	[
		class_<CTTODMathUtils>("CTTODMathUtils")
		.scope
		[
			def("GetAngleToFacePoint", &CTTODMathUtils::GetAngleToFacePoint)
			,def("PointInsideCircle", &CTTODMathUtils::PointInsideCircle)
			, def("CalculateNewAngle", &CTTODMathUtils::CalculateNewAngle)
		]
	];

	module(LUA_STATE)
	[
		class_<CTTODFileUtils>("CTTODFileUtils")
		.scope
		[
			def("SelectTextureFile", &CTTODFileUtils::SelectTextureFile)
		]
	];

	module(LUA_STATE)
	[
		class_<CTTODXMLWriter>("CTTODXMLWriter")
		.def(constructor<>())
		.def("StartFile", &CTTODXMLWriter::StartFile)
		.def("EndFile", &CTTODXMLWriter::EndFile)
		.def("StartElement", &CTTODXMLWriter::StartElement)
		.def("EndElement", &CTTODXMLWriter::EndElement)
		.def("WriteStringProperty", &CTTODXMLWriter::WriteStringProperty)
		.def("WriteBoolProperty", &CTTODXMLWriter::WriteBoolProperty)
		.def("WriteIntProperty", &CTTODXMLWriter::WriteIntProperty)
		.def("WriteFloatProperty", &CTTODXMLWriter::WriteFloatProperty)
		.def("WriteVect2fProperty", &CTTODXMLWriter::WriteVect2fProperty)
		.def("WriteVect3fProperty", &CTTODXMLWriter::WriteVect3fProperty)
		.def("WriteVect4fProperty", &CTTODXMLWriter::WriteVect4fProperty)
		/*.def("WriteProperty", (bool(CTTODXMLWriter::*)(const std::string&, const std::string&))&CTTODXMLWriter::WriteProperty)
		.def("WriteProperty", (bool(CTTODXMLWriter::*)(const std::string&, const int&))&CTTODXMLWriter::WriteProperty)
		.def("WriteProperty", (bool(CTTODXMLWriter::*)(const std::string&, const float&))&CTTODXMLWriter::WriteProperty)*/

		/*.def("Log", (void(CLog::*)(int))&CLog::Log)
		.def("Log", (void(CLog::*)(float))&CLog::Log)
		.def("Log", (void(CLog::*)(const std::string&))&CLog::Log)
		.def("Log", (void(CLog::*)(const Vect3f&))&CLog::Log)*/
	];

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
		.def("GetRight", &C3DElement::GetRight)
		.def("GetUp", &C3DElement::GetUp)
		.def("SetYawPitchRoll", &C3DElement::SetYawPitchRoll)
		.def("GetYawLuaAddress", &C3DElement::GetYawLuaAddress)
		.def("GetPitchLuaAddress", &C3DElement::GetPitchLuaAddress)
		.def("GetRollLuaAddress", &C3DElement::GetRollLuaAddress)
		.def("GetPositionLuaAddress", &C3DElement::GetPositionLuaAddress)
		.def("GetScaleLuaAddress", &C3DElement::GetScaleLuaAddress)
		
		.def("ChildGetTransform", &C3DElement::ChildGetTransform)
		
	];
	
	module(LUA_STATE)
	[
		class_<CXMLTreeNode>("CXMLTreeNode")
		.def(constructor<>())
		.def("StartNewFile", &CXMLTreeNode::StartNewFile)
		.def("StartElement", &CXMLTreeNode::StartElement)
		.def("EndElement", &CXMLTreeNode::EndElement)
		.def("EndNewFile", &CXMLTreeNode::EndNewFile)
		.def("LoadFile", &CXMLTreeNode::LoadFile)
		.def("GetBoolKeyword", &CXMLTreeNode::GetBoolKeyword)
		.def("WriteBoolKeyword", &CXMLTreeNode::WriteBoolKeyword)
		.def("GetBoolProperty", &CXMLTreeNode::GetBoolProperty)
		.def("WriteBoolProperty", &CXMLTreeNode::WriteBoolProperty)
		.def("GetFloatKeyword", &CXMLTreeNode::GetFloatKeyword)
		.def("GetFloatProperty", &CXMLTreeNode::GetFloatProperty)
		.def("WriteFloatKeyword", &CXMLTreeNode::WriteFloatKeyword)
		.def("WriteFloatProperty", &CXMLTreeNode::WriteFloatProperty)
		.def("GetIntKeyword", &CXMLTreeNode::GetIntKeyword)
		.def("GetIntProperty", &CXMLTreeNode::GetIntProperty)
		.def("WriteIntKeyword", &CXMLTreeNode::WriteIntKeyword)
		.def("WriteIntProperty", &CXMLTreeNode::WriteIntProperty)
		.def("GetName", &CXMLTreeNode::GetName)
		.def("GetNumChildren", &CXMLTreeNode::GetNumChildren)
		.def("GetPszISOProperty", &CXMLTreeNode::GetPszISOProperty)
		.def("GetPszKeyword", &CXMLTreeNode::GetPszKeyword)
		.def("GetPszProperty", &CXMLTreeNode::GetPszProperty)
		.def("WritePszKeyword", &CXMLTreeNode::WritePszKeyword)
		.def("WritePszProperty", &CXMLTreeNode::WritePszProperty)
		.def("GetVect2fProperty", &CXMLTreeNode::GetVect2fProperty)
		.def("WriteVect2fProperty", &CXMLTreeNode::WriteVect2fProperty)
		.def("GetVect3fProperty", &CXMLTreeNode::GetVect3fProperty)
		.def("WriteVect3fProperty", &CXMLTreeNode::WriteVect3fProperty)
		.def("GetVect4fProperty", &CXMLTreeNode::GetVect4fProperty)
		.def("WriteVect4fProperty", &CXMLTreeNode::WriteVect4fProperty)
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
		.def("GetNameLuaAddress", &CNamed::GetNameLuaAddress)
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
		.def(constructor<>())
		.def(constructor<const Vect3f>())
		.def(constructor<const float, const float, const float>()) 
		.def(constructor<const float>())

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
		.def(constructor<>())
		.def(constructor<const Vect4f>())
		.def(constructor<const Vect3f>())
		.def(constructor<const Vect3f, const float>())
		.def(constructor<const float, const float, const float, const float>()) 
		.def(constructor<const float>())

		.def(const_self + const_self)   
		.def(const_self - const_self)   
		.def(const_self * const_self) 
		.def(const_self == const_self)   
		.def_readwrite("x", &Vect4f::x) 
		.def_readwrite("y", &Vect4f::y)  
		.def_readwrite("z", &Vect4f::z)  
		.def_readwrite("z", &Vect4f::w)  
	]; 

	module(LUA_STATE)
	[
		class_<CColor, Vect4f>("CColor")
		.def(constructor<>())
		.def(constructor<Vect4f>())
		.def(constructor<float,float,float,float>())
		.def(const_self + const_self)
		.def(const_self - const_self)
		.def(const_self * const_self)
		.def(const_self * other<const float>())
		.def_readwrite("x", &CColor::x)
		.def_readwrite("y", &CColor::y)
		.def_readwrite("z", &CColor::z)
		.def_readwrite("z", &CColor::w)
		.def("GetRed", &CColor::GetRed)
		.def("SetRed", &CColor::SetRed)
		.def("GetBlue", &CColor::GetBlue)
		.def("SetBlue", &CColor::SetBlue)
		.def("GetGreen", &CColor::GetGreen)
		.def("SetGreen", &CColor::SetGreen)
		.def("GetAlpha", &CColor::GetAlpha)
		.def("SetAlpha", &CColor::SetAlpha)
		.def("GetArgb", &CColor::GetArgb)
		.def("SetArgb", &CColor::SetArgb)
		.def("Clamp", &CColor::Clamp)
		.def("Lerp", &CColor::Lerp)
	];

	module(LUA_STATE)
	[  
		class_< Mat33f >("Mat33f")   
		.def(constructor<>()) 
		.def(constructor<Mat33f>()) 
		.def(constructor<const float,const float,const float,const float,const float,const float,const float,const float,const float>()) 
		.def(constructor<const Vect3f,const Vect3f,const Vect3f>()) 
		.def(constructor<const float,const float,const float>()) 
		.def("reset_rotation", &Mat33f::ResetRotation)
		.def("reset_scale", &Mat33f::ResetScale)
		.def("set_identity", &Mat33f::SetIdentity)
		.def("set_zero", &Mat33f::SetZero)
		.def(const_self + const_self)   
		.def(const_self - const_self)   
		.def(const_self * const_self) 
		.def(const_self * other<const float>())
		.def(const_self / other<const float>()) 
		.def(const_self * other<const Vect3f>())
		.def(const_self == const_self)
		.def("transform_vector", &Mat33f::TransformVector)
		.def("get_transform_vector", &Mat33f::GetTransformVector)
		.def("transform_array_vectors", &Mat33f::TransformArrayVectors)
		.def("get_inverted", &Mat33f::GetInverted)
		.def("get_inverted_rs", &Mat33f::GetInvertedRS)
		.def("get_transposed", &Mat33f::GetTransposed)
		.def("get_rotated_by_anglex", &Mat33f::GetRotedByAngleX)
		.def("get_rotated_by_angley", &Mat33f::GetRotedByAngleY)
		.def("get_rotated_by_anglez", &Mat33f::GetRotedByAngleZ)
		.def("get_rotated_by_anglexz", &Mat33f::GetRotedByAnglesXZ)
		.def("get_rotated_by_angleyxz", &Mat33f::GetRotedByAnglesYXZ)
		.def("get_scaled", &Mat33f::GetScaled)
		.def("align_yx", &Mat33f::AlignYX)
		.def("align_yz", &Mat33f::AlignYZ)
		.def("align_xz", &Mat33f::AlignXZ)
		.def("determinant", & Mat33f::Determinant)
		.def("is_orthogonal_epsilon", &Mat33f::IsOrthogonalEpsilon)
		.def("is_orthonormal_epsilon", &Mat33f::IsOrthonormalEpsilon)
	];

	module(LUA_STATE)
	[  
		class_< Mat34f >("Mat34f")   
		.def(constructor<>()) 
		.def(constructor<const Mat34f>())
		.def(constructor<const Mat33f>())
		.def(constructor<const Mat33f, const Vect3f>())
		.def(constructor<const Vect3f, const Vect3f, const Vect3f, const Vect3f>())
		.def(constructor<const float, const float, const float>())
		.def("ResetTranslation", &Mat34f::ResetTranslation)
		.def("ResetRotation", &Mat34f::ResetRotation)
		.def("ResetScale", &Mat34f::ResetScale)
		.def("ResetRotationScale", &Mat34f::ResetRotationScale)
		.def("SetIdentity", &Mat34f::SetIdentity)
		.def("SetZero", &Mat34f::SetZero)

		.def("SetFromAngleX", &Mat34f::SetFromAngleX)
		.def("SetFromAngleY", &Mat34f::SetFromAngleY)
		.def("SetFromAngleZ", &Mat34f::SetFromAngleZ)
		.def("SetFromAnglesXZ", &Mat34f::SetFromAnglesXZ)
		.def("SetFromAnglesYXZ", &Mat34f::SetFromAnglesYXZ)
		/*.def("SetFromPos", &Mat34f::SetFromPos)
		.def("SetFromPos", &Mat34f::SetFromPos)*/
		.def("SetFromScale", &Mat34f::SetFromScale)
		
		.def("SetFromLookAt", (Mat34f&(Mat34f::*)(const Vect3f&, const Vect3f&, const Vect3f&))&Mat34f::SetFromLookAt)
	
		.def(const_self + const_self)
		.def(const_self - const_self)
		.def(const_self * const_self)
		.def(const_self * other<const float>())
		.def(const_self / other<const float>())
		.def(const_self * other<const Vect3f>())
		.def(const_self ^ other<const Vect3f>())
		.def(const_self == const_self)
		.def(const_self == const_self)
	];

	module(LUA_STATE) 
	[  
		class_< Mat44f >("Mat44f")   
		.def(constructor<>()) 
		.def(constructor<Mat44f>())
		.def(constructor<Mat33f>()) 
		.def(constructor<Mat34f>()) 
		.def(constructor<const Vect3f,const Vect3f,const Vect3f,const Vect3f>()) 
		.def(constructor<const float,const float,const float>()) 
		.def("ResetTranslation", &Mat44f::ResetTranslation)
		.def("ResetRotation", &Mat44f::ResetRotation)
		.def("ResetScale", &Mat44f::ResetScale)
		.def("ResetRotationScale", &Mat44f::ResetRotationScale)
		.def("SetIdentity", &Mat44f::SetIdentity)
		.def("SetZero", &Mat44f::SetZero)
	
		.def("SetFromBasis", (Mat44f&(Mat44f::*)(const Vect3f&, const Vect3f&, const Vect3f&, const Vect3f&))&Mat44f::SetFromBasis)
		.def("SetFromBasis", (Mat44f&(Mat44f::*)(const Vect3f&, const Vect3f&, const Vect3f&))&Mat44f::SetFromBasis)
		
		.def("SetFromAngleX", &Mat44f::SetFromAngleX)
		.def("SetFromAngleY", &Mat44f::SetFromAngleY)
		.def("SetFromAngleZ", &Mat44f::SetFromAngleZ)
		.def("SetFromAnglesXZ", &Mat44f::SetFromAnglesXZ)
		.def("SetFromAnglesYXZ", &Mat44f::SetFromAnglesYXZ)
	
		.def("SetFromPos", (Mat44f&(Mat44f::*)(const float, const float, const float))&Mat44f::SetFromPos)
		.def("SetFromPos", (Mat44f&(Mat44f::*)(const Vect3f&))&Mat44f::SetFromPos)

		.def("SetFromScale", &Mat44f::SetFromScale)
		.def("SetFromLookAt", (Mat44f&(Mat44f::*)(const Vect3f&, const Vect3f&, const Vect3f&))&Mat44f::SetFromLookAt)
		.def("SetFromLookAt", (Mat44f&(Mat44f::*)(const Vect3f&, const Vect3f&))&Mat44f::SetFromLookAt)
		.def("SetFromPerspective", &Mat44f::SetFromPerspective)
		
		.def("SetFromPosAndAnglesYXZ", &Mat44f::SetFromPosAndAnglesYXZ)
		.def("SetFromQuatPos", &Mat44f::SetFromQuatPos)
		.def("SetFromOrtho", &Mat44f::SetFromOrtho)

		.def("SetRotByAngleX", &Mat44f::SetRotByAngleX)
		.def("SetRotByAngleY", &Mat44f::SetRotByAngleY)
		.def("SetRotByAngleZ", &Mat44f::SetRotByAngleZ)
		.def("SetRotByAnglesXZ", &Mat44f::SetRotByAnglesXZ)
		.def("SetRotByAnglesYXZ", &Mat44f::SetRotByAnglesYXZ)
		.def("SetRotByQuat", &Mat44f::SetRotByQuat)

		.def("SetPos", (Mat44f&(Mat44f::*)(const Vect3f&))&Mat44f::SetPos)
		.def("SetPos", (Mat44f&(Mat44f::*)(const float,const float,const float))&Mat44f::SetPos)

		//.def("SetScale", (Mat44f&(Mat44f::*)(const Mat33f&))&Mat44f::SetScale)
		//.def("SetScale", (Mat44f&(Mat44f::*)(const Mat44f&))&Mat44f::SetScale)
		.def("SetScale", (Mat44f&(Mat44f::*)(const Vect3f&))&Mat44f::SetScale)
		.def("SetScale", (Mat44f&(Mat44f::*)(const float,const float,const float))&Mat44f::SetScale)
		
		.def("GetRow", &Mat44f::GetRow)
		.def("GetColum", &Mat44f::GetColum)

		.def("GetVectorBasis", &Mat44f::GetVectorBasis)
		.def("GetVectorBasisLength", &Mat44f::GetVectorBasisLength)
		.def("GetPosBasis", &Mat44f::GetPosBasis)
		.def("GetBasis", &Mat44f::GetBasis)
		/*.def("GetSubMatrix33", (Mat33f(Mat44f::*))&Mat44f::GetSubMatrix33)*/

		.def("Get33RotationNormalized", &Mat44f::Get33RotationNormalized)
		.def("Get33RotationScaled", &Mat44f::Get33RotationScaled)
		.def("Get33Scale", &Mat44f::Get33Scale)
		.def("GetTranslationVector", &Mat44f::GetTranslationVector)

		.def("Get44RotationNormalized", &Mat44f::Get44RotationNormalized)
		.def("Get44RotationScaled", &Mat44f::Get44RotationScaled)
		.def("Get44Scale", &Mat44f::Get44Scale)
		.def("Get44Translation", &Mat44f::Get44Translation)

		.def("GetPos", &Mat44f::GetPos)

		.def("GetScale", &Mat44f::GetScale)
		.def("GetScaleX", &Mat44f::GetScaleX)
		.def("GetScaleY", &Mat44f::GetScaleY)
		.def("GetScaleZ", &Mat44f::GetScaleZ)

		// Angulos de Euler
		.def("GetAnglesYXZ", &Mat44f::GetAnglesYXZ)
		.def("GetAngleX", &Mat44f::GetAngleX)
		.def("GetAngleY", &Mat44f::GetAngleY)
		.def("GetAngleZ", &Mat44f::GetAngleZ)

		// Angulos de Euler con criterio de Pitch-Roll-Yaw
		// (Orden XZY, teniendo los ángulos Y,Z el sentido contrario de rotación)
		.def("GetRoll", &Mat44f::GetRoll)
		.def("GetPitch", &Mat44f::GetPitch)
		.def("GetYaw", &Mat44f::GetYaw)
		.def("GetPitchRollYaw", &Mat44f::GetPitchRollYaw)
		
		.def("SetFromPitchRollYaw", &Mat44f::SetFromPitchRollYaw)
		.def("SetPitchRollYaw", &Mat44f::SetPitchRollYaw)

		// Operadores de aritmética de matrices
		.def(const_self + const_self)
		.def(const_self - const_self)
		.def(const_self * const_self)
		.def(const_self * other<const float>())
		.def(const_self / other<const float>())
		.def(const_self * other<const Vect3f>())
		.def(const_self ^ other<const Vect3f>())
		.def(const_self * other<const Vect4f>())
		.def(const_self == const_self)

		.def("IsEqualEpsilon", &Mat44f::IsEqualEpsilon)
		.def("IsNotEqualEpsilon", &Mat44f::IsNotEqualEpsilon)

		.def("TransformVector", &Mat44f::TransformVector)
		.def("TransformPoint", &Mat44f::TransformPoint)
		.def("GetTransformVector", &Mat44f::GetTransformVector)
		.def("GetTransformPoint", &Mat44f::GetTransformPoint)
		.def("TransformArrayVectors", &Mat44f::TransformArrayVectors)
		.def("TransformArrayPoints", &Mat44f::TransformArrayPoints)

		.def("GetInverted", &Mat44f::GetInverted)
		.def("GetInvertedTRS", &Mat44f::GetInvertedTRS)
		.def("GetInvertedTR", &Mat44f::GetInvertedTR)
		.def("GetTransposed", &Mat44f::GetTransposed)
		.def("GetRotedByAngleX", &Mat44f::GetRotedByAngleX)
		.def("GetRotedByAngleY", &Mat44f::GetRotedByAngleY)
		.def("GetRotedByAngleZ", &Mat44f::GetRotedByAngleZ)
		.def("GetRotedByAnglesXZ", &Mat44f::GetRotedByAnglesXZ)
		.def("GetRotedByAnglesYXZ", &Mat44f::GetRotedByAnglesYXZ)
		.def("GetTranslated", &Mat44f::GetTranslated)
		.def("GetScaled", &Mat44f::GetScaled)
	
		.def("Invert", &Mat44f::Invert)
		.def("InvertTRS", &Mat44f::InvertTRS)
		.def("InvertTR", &Mat44f::InvertTR)
		.def("Transpose", &Mat44f::Transpose)
		.def("RotByAngleX", &Mat44f::RotByAngleX)
		.def("RotByAngleY", &Mat44f::RotByAngleY)
		.def("RotByAngleZ", &Mat44f::RotByAngleZ)
		.def("RotByAnglesXZ", &Mat44f::RotByAnglesXZ)
		.def("RotByAnglesYXZ", &Mat44f::RotByAnglesYXZ)
		.def("Translate", &Mat44f::Translate)
		.def("Scale", &Mat44f::Scale)

		.def("AlignYX", &Mat44f::AlignYX)
		.def("AlignYZ", &Mat44f::AlignYZ)
		.def("AlignXZ", &Mat44f::AlignXZ)

		.def("Determinant", &Mat44f::Determinant)
		.def("IsOrthogonalEpsilon", &Mat44f::IsOrthogonalEpsilon)
		.def("IsOrthonormalEpsilon", &Mat44f::IsOrthonormalEpsilon)
	];

	module(LUA_STATE) 
	[
		class_<CEmptyPointerClass>("CEmptyPointerClass")
	];

}