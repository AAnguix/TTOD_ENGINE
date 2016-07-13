#include "LuabindManager\LuabindManager.h"
#include "LuabindManager\LuabindMacros.h"
#include <luabind/luabind.hpp>

#include <luabind/operator.hpp>
#include <luabind/iterator_policy.hpp>

/*Base*/
#include "XML\XMLTreeNode.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Utils\TemplatedMapManager.h"
#include "Utils\EmptyPointerClass.h"

/*Core*/
#include "Engine\Engine.h"
#include "Log\Log.h"
#include "Components\AnimatorController\AnimatorController.h"

#include "DebugHelper\DebugHelperImplementation.h"
#include "Input\InputManagerImplementation.h"

/*Graphics*/
#include "Render\RenderManager.h"
#include "Render\DebugRender.h"
#include "Render\GraphicsStats.h"
#include "SceneRendererCommands\SceneRendererCommandManager.h"
#include "Camera\Camera.h"
#include "Camera\CameraControllerManager.h"
#include "Camera\FixedCameraController.h"
#include "Camera\ThirdPersonCameraController.h"
#include "Camera\FPSCameraController.h"
#include "Camera\SphericalCameraController.h"
#include "RenderableObjects\LayerManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "RenderableObjects\MeshInstance.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "Lights\LightManager.h"
#include "Lights\OmniLight.h"
#include "Lights\SpotLight.h"
#include "Effects\EffectTechnique.h"
#include "Cinematics\CameraKey.h"
#include "Cinematics\CinematicPlayer.h"
#include "Cinematics\CameraKeyController.h"
#include "Cinematics\Cinematic.h"
#include "Animation\AnimatedCoreModel.h"
#include "Animation\AnimatedInstanceModel.h"
#include "Animation\AnimatedModelManager.h"
#include "Materials\MaterialManager.h"
#include "Materials\TemplatedMaterialParameter.h"
#include "Effects\EffectManager.h"
#include "Textures\TextureManager.h"
#include "Animation\AnimatorControllerManager.h"
#include "Particles\ParticleManager.h"
#include "Particles\ParticleSystemInstance.h"
#include "cal3d\model.h"
#include "cal3d\coremodel.h"

using namespace luabind;

void CLuabindManager::RegisterGraphics()
{
	RegisterRender();
	RegisterCamera();
	RegisterEffects();
	RegisterLights();
	RegisterMaterials();
	RegisterRenderableObjects();
	RegisterParticles();
	RegisterCinematics();
	RegisterAnimations();
	RegisterSceneRendererCommands();
}

void CLuabindManager::RegisterRender()
{
	module(LUA_STATE)
	[
		class_<CRenderManager>("CRenderManager")
		.def(constructor<CContextManager*>())
		.def("SetCurrentCamera", &CRenderManager::SetCurrentCamera)
		.def("SetDebugCamera", &CRenderManager::SetDebugCamera)
		.def("AddRenderableObjectToRenderList", &CRenderManager::AddRenderableObjectToRenderList)
		.def("Render", &CRenderManager::Render)
		.def("GetContextManager", &CRenderManager::GetContextManager)
		.def("GetDebugRender", &CRenderManager::GetDebugRender)
	];

	module(LUA_STATE)
	[
		class_<CDebugRender>("CDebugRender")
		.def(constructor<>())
		.def("GetAxis", &CDebugRender::GetAxis)
	];

	module(LUA_STATE)
	[
		class_<CContextManager>("CContextManager")
		.def(constructor<>())

		/*.def("ResizeBuffers", &CContextManager::ResizeBuffers)*/
		.def("GetAspectRatio", &CContextManager::GetAspectRatio)
		
		.def("Draw", &CContextManager::Draw)
		.def("DrawIndexed", &CContextManager::DrawIndexed)
		
		.def("GetDevice", &CContextManager::GetDevice)
		.def("GetDeviceContext", &CContextManager::GetDeviceContext)

		.def("SetBaseColor", &CContextManager::SetBaseColor)
		.def("SetWorldMatrix", &CContextManager::SetWorldMatrix)
		.def("GetViewPort", &CContextManager::GetViewPort)
	
		.def("GetFrameBufferWidth", &CContextManager::GetFrameBufferWidth)
		.def("GetFrameBufferHeight", &CContextManager::GetFrameBufferHeight)
		/*.def("CaptureStencilBuffer", &CContextManager::CaptureStencilBuffer)*/
	];

	module(LUA_STATE)
	[
		class_<CGraphicsStats>("CGraphicsStats")
		.def(constructor<>())
		.def("GetFps", &CGraphicsStats::GetFps)
		.def("GetMilliSecondsPerFrame", &CGraphicsStats::GetMilliSecondsPerFrame)
		.def("GetMinMillisecondsPerFrame", &CGraphicsStats::GetMinMillisecondsPerFrame)
		.def("GetMaxMillisecondsPerFrame", &CGraphicsStats::GetMaxMillisecondsPerFrame)
		.def("GetAvgMillisecondsPerFrame", &CGraphicsStats::GetAvgMillisecondsPerFrame)
	];
}

void CLuabindManager::RegisterAnimations()
{
	module(LUA_STATE)
	[
		class_<CAnimatorControllerManager>("CAnimatorControllerManager")
		.def(constructor<>())
		.def("AddComponent", &CAnimatorControllerManager::AddComponent)
		.def("RemoveComponents", &CAnimatorControllerManager::RemoveComponents)
	];

	module(LUA_STATE)
	[
		class_<CAnimatedCoreModel, CNamed>("CAnimatedCoreModel")
		.def(constructor<>())
		.def("GetMaterials", &CAnimatedCoreModel::GetMaterials)
		.def("GetCoreModel", &CAnimatedCoreModel::GetCoreModel)
		.def("GetBoneId", &CAnimatedCoreModel::GetBoneId)
		.def("Load", &CAnimatedCoreModel::Load)
	];

	module(LUA_STATE)
	[
		class_<CAnimatedInstanceModel, CRenderableObject>("CAnimatedInstanceModel")
		.def(constructor<CXMLTreeNode>())
		.def(constructor<CGameObject*, const std::string, const std::string, const Vect3f, float, float, float>())
		.def("GetAnimatedCoreModel", &CAnimatedInstanceModel::GetAnimatedCoreModel)
		.def("Initialize", &CAnimatedInstanceModel::Initialize)
		.def("Render", &CAnimatedInstanceModel::Render)
		.def("Update", &CAnimatedInstanceModel::Update)
		.def("Destroy", &CAnimatedInstanceModel::Destroy)
		.def("ExecuteAction", &CAnimatedInstanceModel::ExecuteAction)
		.def("BlendCycle", &CAnimatedInstanceModel::BlendCycle)
		.def("ClearCycle", &CAnimatedInstanceModel::ClearCycle)
		.def("IsCycleAnimationActive", &CAnimatedInstanceModel::IsCycleAnimationActive)
		.def("IsActionAnimationActive", &CAnimatedInstanceModel::IsActionAnimationActive)
		.def("GetBoneTransformationMatrix", &CAnimatedInstanceModel::GetBoneTransformationMatrix)
	];

	module(LUA_STATE)
	[
		class_<CTemplatedMapManager<CAnimatedCoreModel>>("CTemplatedMapManager")
		.def("GetResource", &CTemplatedMapManager<CAnimatedCoreModel>::GetResource)
		.def("AddResource", &CTemplatedMapManager<CAnimatedCoreModel>::AddResource)
	];

	module(LUA_STATE)
	[
		class_< CAnimatedModelManager, CTemplatedMapManager<CAnimatedCoreModel>>("CAnimatedModelManager")
		.def("Load", &CAnimatedModelManager::Load)
		.def("Reload", &CAnimatedModelManager::Reload)
		.def("GetLUAAnimatedModels", &CAnimatedModelManager::GetLUAAnimatedModels, return_stl_iterator)
	];
}

void CLuabindManager::RegisterCamera()
{
	module(LUA_STATE)
	[
		class_<CCamera, CNamed>("CCamera")
		.def(constructor<>())
		.def(constructor<const std::string>())
		.def(constructor<CXMLTreeNode>())
		.def("GetFOV", &CCamera::GetFOV)
		.def("SetFOV", &CCamera::SetFOV)
		.def("GetAspectRatio", &CCamera::GetAspectRatio)
		.def("SetAspectRatio", &CCamera::SetAspectRatio)
		.def("GetZNear", &CCamera::GetZNear)
		.def("SetZNear", &CCamera::SetZNear)
		.def("GetZFar", &CCamera::GetZFar)
		.def("SetZFar", &CCamera::SetZFar)
		.def("GetPosition", &CCamera::GetPosition)
		.def("SetPosition", &CCamera::SetPosition)
		.def("GetLookAt", &CCamera::GetLookAt)
		.def("SetLookAt", &CCamera::SetLookAt)
		.def("GetUp", &CCamera::GetUp)
		.def("SetUp", &CCamera::SetUp)
		.def("GetView", &CCamera::GetView)
		.def("GetProjection", &CCamera::GetProjection)
		.def("SetMatrixs", &CCamera::SetMatrixs)
	];

	module(LUA_STATE)
	[
		class_<CCameraController, CNamed>("CCameraController")
		//.def(constructor<>())
		.def("SetCamera", &CCameraController::SetCamera)
		.def("AddYaw", &CCameraController::AddYaw)
		.def("AddPitch", &CCameraController::AddPitch)
		.def("SetYaw", &CCameraController::SetYaw)
		.def("GetYaw", &CCameraController::GetYaw)
		.def("SetPitch", &CCameraController::SetPitch)
		.def("GetPitch", &CCameraController::GetPitch)
		.def("GetUp", &CCameraController::GetUp)
		.def("GetRight", &CCameraController::GetRight)
		.def("GetForward", &CCameraController::GetForward)
		.def("GetPosition", &CCameraController::GetPosition)
		.def("SetPosition", &CCameraController::SetPosition)
		.def("GetType", &CCameraController::GetType)
		.def("Update", &CCameraController::Update)
		.def("GetThisLuaAddress", &CCameraController::GetThisLuaAddress)
		.enum_("t_camera_controller_type")
		[
			value("FIXED", 0),
			value("THIRD_PERSON", 1),
			value("KEY", 2),
			value("DEBUG", 3)
		]
	];

	module(LUA_STATE)
	[
		class_<CFixedCameraController, CCameraController>("CFixedCameraController")
		.def(constructor<>())
		.def(constructor<CXMLTreeNode>())
		//.def("SetCamera", &CFixedCameraController::SetCamera)
		.def("GetDirection", &CFixedCameraController::GetDirection)
	];

	module(LUA_STATE)
	[
		class_<CFPSCameraController, CCameraController>("CFPSCameraController")
		.def(constructor<>())
		.def("Move", &CFPSCameraController::Move)
		.def("MoveUpDown", &CFPSCameraController::MoveUpDown)
		//.def("SetCamera", &CFPSCameraController::SetCamera)
		.def("GetDirection", &CFPSCameraController::GetDirection)
		.def("GetType", &CFPSCameraController::GetType)
	];

	module(LUA_STATE)
	[
		class_<CSphericalCameraController, CCameraController>("CSphericalCameraController")
		.def(constructor<>())
		.def("AddZoom", &CSphericalCameraController::AddZoom)
		.def("SetZoom", &CSphericalCameraController::SetZoom)
		//.def("SetCamera", &CSphericalCameraController::SetCamera)
		.def("GetDirection", &CSphericalCameraController::GetDirection)
		.def("Update", &CSphericalCameraController::Update)
		.def("GetType", &CSphericalCameraController::GetType)
	];

	module(LUA_STATE)
	[
		class_<CThirdPersonCameraController, CCameraController>("CThirdPersonCameraController")
		.def(constructor<>())
		.def(constructor<CXMLTreeNode>())
		.def("AddZoom", &CThirdPersonCameraController::AddZoom)
		.def("SetZoom", &CThirdPersonCameraController::SetZoom)
		//.def("SetCamera", &CThirdPersonCameraController::SetCamera)
		.def("MoveUpDown", &CThirdPersonCameraController::MoveUpDown)
		.def("Move", &CThirdPersonCameraController::Move)
		//.def("AddYaw", &CThirdPersonCameraController::AddYaw)
		//.def("AddPitch", &CThirdPersonCameraController::AddPitch)
		.def("GetDirection", &CThirdPersonCameraController::GetDirection)
		.def("GetType", &CThirdPersonCameraController::GetType)
		.def("GetPlayerCameraAngleDif", &CThirdPersonCameraController::GetPlayerCameraAngleDif)
		.def("AddLookAtPitch", &CThirdPersonCameraController::AddLookAtPitch)
		.def("GetZoomLuaAddress", &CThirdPersonCameraController::GetZoomLuaAddress)
		.def("GetZoomSpeedLuaAddress", &CThirdPersonCameraController::GetZoomSpeedLuaAddress)
		.def("GetMinZoomLuaAddress", &CThirdPersonCameraController::GetMinZoomLuaAddress)
		.def("GetMaxZoomLuaAddress", &CThirdPersonCameraController::GetMaxZoomLuaAddress)
		.def("GetYawSpeedLuaAddress", &CThirdPersonCameraController::GetYawSpeedLuaAddress)
		.def("GetPitchSpeedLuaAddress", &CThirdPersonCameraController::GetPitchSpeedLuaAddress)
		.def("GetRollSpeedLuaAddress", &CThirdPersonCameraController::GetRollSpeedLuaAddress)
		.def("GetSpeedLuaAddress", &CThirdPersonCameraController::GetSpeedLuaAddress)
		.def("GetFastSpeedLuaAddress", &CThirdPersonCameraController::GetFastSpeedLuaAddress)
		.def("GetMinLookAtPitchLuaAddress", &CThirdPersonCameraController::GetMinLookAtPitchLuaAddress)
		.def("GetMaxLookAtPitchLuaAddress", &CThirdPersonCameraController::GetMaxLookAtPitchLuaAddress)

	];

	module(LUA_STATE)
	[
		class_<CTemplatedMapManager<CCameraController>>("CTemplatedMapManager")
		.def("GetResource", &CTemplatedMapManager<CCameraController>::GetResource)
		.def("AddResource", &CTemplatedMapManager<CCameraController>::AddResource)
	];

	module(LUA_STATE)
	[
		class_< CCameraControllerManager, CTemplatedMapManager<CCameraController>>("CCameraControllerManager")
		.def("Load", &CCameraControllerManager::Load)
		.def("Reload", &CCameraControllerManager::Reload)
		.def("Update", &CCameraControllerManager::Update)
		.def("SetCurrentCameraController", &CCameraControllerManager::SetCurrentCameraController)
		.def("GetCurrentCameraController", &CCameraControllerManager::GetCurrentCameraController)
		.def("NextCameraController", &CCameraControllerManager::NextCameraController)

		.def("GetLocked", &CCameraControllerManager::GetLocked)
		.def("SetLocked", &CCameraControllerManager::SetLocked)
		.def("ChangeLockState", &CCameraControllerManager::ChangeLockState)
		.def("GetLUACameraControllers", &CCameraControllerManager::GetLUACameraControllers, return_stl_iterator)

	];
}

void CLuabindManager::RegisterCinematics()
{
	//Cinematics
	module(LUA_STATE)
	[
		class_<CCameraKey>("CCameraKey")
		.def(constructor<CCameraInfo, float>())
	];

	module(LUA_STATE)
	[
		class_<CCameraInfo>("CCameraInfo")
		.def(constructor<>())
		.def(constructor<Vect3f, Vect3f, Vect3f, Vect3f, float, float, float>())
		.def(constructor<CXMLTreeNode>())
	];

	module(LUA_STATE)
	[
		class_<CCameraKeyController, CCameraController>("CCameraKeyController")
		.def(constructor<CXMLTreeNode>())
		.def("set_camera", &CCameraKeyController::SetCamera)
		.def("set_current_time", &CCameraKeyController::SetCurrentTime)
		.def("reset_time", &CCameraKeyController::ResetTime)
		.def("get_total_time", &CCameraKeyController::GetTotalTime)
		.def("is_cycle", &CCameraKeyController::IsCycle)
		.def("set_cycle", &CCameraKeyController::SetCycle)
		.def("is_reverse", &CCameraKeyController::IsReverse)
		.def("set_reverse", &CCameraKeyController::SetReverse)
	];

	module(LUA_STATE)
	[
		class_<CCinematicPlayer>("CCinematicPlayer")
		.def(constructor<>())
		.def("Init", &CCinematicPlayer::Init)
		.def("Update", &CCinematicPlayer::Update)
		.def("Stop", &CCinematicPlayer::Stop)
		.def("Play", &CCinematicPlayer::Play)
		.def("Pause", &CCinematicPlayer::Pause)
		.def("IsFinished", &CCinematicPlayer::IsFinished)
		.def("GetDuration", &CCinematicPlayer::GetDuration)
		.def("GetCurrentTime", &CCinematicPlayer::GetActualTime)
		.def("OnRestartCycle", &CCinematicPlayer::OnRestartCycle)
	];

	module(LUA_STATE)
	[
		class_<CCinematic, bases<CRenderableObject, CCinematicPlayer>>("CCinematic")
		.def(constructor<CXMLTreeNode>())
		.def("load_xml", &CCinematic::LoadXML)
		.def("add_cinematic_object", &CCinematic::AddCinematicObject)
	];

	module(LUA_STATE)
	[
		class_<CCinematicObject, CCinematicPlayer>("CCinematicObject")
		.def(constructor<CXMLTreeNode>())
		.def("is_ok", &CCinematicObject::IsOk)
		.def("add_cinematic_object_keyframe", &CCinematicObject::AddCinematicObjectKeyFrame)
		.def("update", &CCinematicObject::Update)
		.def("stop", &CCinematicObject::Stop)
		.def("on_restart_cycle", &CCinematicObject::OnRestartCycle)
	];

	module(LUA_STATE)
	[
		class_<CCinematicObjectKeyFrame, C3DElement>("CCinematicObjectKeyFrame")
		.def(constructor<CXMLTreeNode>())
		.def("get_keyframe_time", &CCinematicObjectKeyFrame::GetKeyFrameTime)
		.def("set_keyframe_time", &CCinematicObjectKeyFrame::SetKeyFrameTime)
	];
}

void CLuabindManager::RegisterEffects()
{
	module(LUA_STATE)
	[
		class_<CEffectTechnique, CNamed>("CEffectTechnique")
		.def(constructor<CXMLTreeNode>())
		.def("GetVertexShader", &CEffectTechnique::GetVertexShader)
		.def("GetPixelShader", &CEffectTechnique::GetPixelShader)
		.def("Refresh", &CEffectTechnique::Refresh)
		.def("SetConstantBuffer", &CEffectTechnique::SetConstantBuffer)
	];

	module(LUA_STATE)
	[
		class_<CTemplatedMapManager<CEffectTechnique>>("CTemplatedMapManager")
		.def("GetResource", &CTemplatedMapManager<CEffectTechnique>::GetResource)
		.def("AddResource", &CTemplatedMapManager<CEffectTechnique>::AddResource)
	];

	module(LUA_STATE)
	[
		class_< CEffectManager, CTemplatedMapManager<CEffectTechnique>>("CEffectManager")
		.def("Load", &CEffectManager::Load)
		.def("Reload", &CEffectManager::Reload)
		.def("GetLUAEffects", &CEffectManager::GetLUAEffects, return_stl_iterator)
	];

	module(LUA_STATE)
	[
		class_<CEffectShader>("CEffectShader")
		.def("Load", &CEffectShader::Load)
		.def("Reload", &CEffectShader::Reload)
		.def("GetConstantBuffer", &CEffectShader::GetConstantBuffer)
	];

	module(LUA_STATE)
	[
		class_<CEffectPixelShader, CEffectShader>("CEffectPixelShader")
		.def(constructor<CXMLTreeNode>())
		.def("Load", &CEffectPixelShader::Load)
		.def("SetConstantBuffer", &CEffectPixelShader::SetConstantBuffer)
		.def("GetPixelShader", &CEffectPixelShader::GetPixelShader)
	];

	module(LUA_STATE)
	[
		class_<CEffectVertexShader, CEffectShader>("CEffectVertexShader")
		.def(constructor<CXMLTreeNode>())
		.def("Load", &CEffectVertexShader::Load)
		.def("SetConstantBuffer", &CEffectVertexShader::SetConstantBuffer)
		.def("GetVertexShader", &CEffectVertexShader::GetVertexShader)
		.def("GetVertexLayout", &CEffectVertexShader::GetVertexLayout)
	];
}

void CLuabindManager::RegisterLights()
{
	module(LUA_STATE)
	[
		class_<CLight, bases<C3DElement, CNamed>>("CLight")
		.enum_("t_light_type")
		[
			value("OMNI", 0),
			value("DIRECTIONAL", 1),
			value("SPOT", 2)
		]
		.def("GetIntensity", &CLight::GetIntensity)
		.def("SetIntensity", &CLight::SetIntensity)
		.def("SetActive", &CLight::SetActive)
		.def("GetActive", &CLight::GetActive)
		.def("GetColor", &CLight::GetColor)
		.def("SetColor", &CLight::SetColor)
		.def("GetStartRangeAttenuation", &CLight::GetStartRangeAttenuation)
		.def("SetStartRangeAttenuation", &CLight::SetStartRangeAttenuation)
		.def("GetEndRangeAttenuation", &CLight::GetEndRangeAttenuation)
		.def("SetEndRangeAttenuation", &CLight::SetEndRangeAttenuation)
		.def("GetType", &CLight::GetType)
		.def("SetType", &CLight::SetType)
		.def("Render", &CLight::Render)
		.def("GetActiveLuaAddress", &CLight::GetActiveLuaAddress)
		.def("GetIntensityLuaAddress", &CLight::GetIntensityLuaAddress)
		.def("GetColorLuaAddress", &CLight::GetColorLuaAddress)
		.def("GetStartRangeAttenuationLuaAddress", &CLight::GetStartRangeAttenuationLuaAddress)
		.def("GetEndRangeAttenuationLuaAddress", &CLight::GetEndRangeAttenuationLuaAddress)
		.scope
		[
			def("GetLightTypeByName", &CLight::GetLightTypeByName)
		]
	];

	module(LUA_STATE)
	[
		class_<CDirectionalLight, CLight>("CDirectionalLight")
		.def(constructor<const std::string&>())
		.def(constructor<CXMLTreeNode>())
		.def("GetDirection", &CDirectionalLight::GetDirection)
		.def("SetDirection", &CDirectionalLight::SetDirection)
		.def("Render", &CDirectionalLight::Render)
		.def("GetDirectionAddress", &CDirectionalLight::GetDirectionAddress)
	];

	module(LUA_STATE)
	[
		class_<COmniLight, CLight>("COmniLight")
		.def(constructor<const std::string&>())
		.def(constructor<CXMLTreeNode>())
		.def("render", &COmniLight::Render)
	];

	module(LUA_STATE)
	[
		class_<CSpotLight, CDirectionalLight>("CSpotLight")
		.def(constructor<const std::string&>())
		.def(constructor<CXMLTreeNode>())
		.def("GetAngle", &CSpotLight::GetAngle)
		.def("GetFallOff", &CSpotLight::GetFallOff)
		.def("Render", &CSpotLight::Render)
		.def("GetAngleLuaAddress", &CSpotLight::GetAngleLuaAddress)
		.def("GetFallOffLuaAddress", &CSpotLight::GetFallOffLuaAddress)
	];

	module(LUA_STATE)
	[
		class_<CTemplatedVectorMapManager<CLight>::TVectorResources>("TVectorResources")
		.def("size", &CTemplatedVectorMapManager<CLight>::TVectorResources::size)
	];

	module(LUA_STATE)
	[
		class_<CTemplatedVectorMapManager<CLight>>("CTemplatedVectorMapManager")
		.def("GetResource", &CTemplatedVectorMapManager<CLight>::GetResource)
		.def("GetResourceById", &CTemplatedVectorMapManager<CLight>::GetResourceById)
		.def("AddResource", &CTemplatedVectorMapManager<CLight>::AddResource)
		.def("RemoveResource", &CTemplatedVectorMapManager<CLight>::RemoveResource)
		.def("GetResourcesMap", &CTemplatedVectorMapManager<CLight>::GetResourcesMap)
		.def("GetResourcesVector", &CTemplatedVectorMapManager<CLight>::GetResourcesVector)
	];

	module(LUA_STATE)
	[
		class_< CLightManager, CTemplatedVectorMapManager<CLight>>("CLightManager")
		.def("Render", &CLightManager::Render)
		.def("Load", &CLightManager::Load)
		.def("Reload", &CLightManager::Reload)
		.def("GetFogColorAddress", &CLightManager::GetFogColorAddress)
		.def("GetAmbientLightAddress", &CLightManager::GetAmbientLightAddress)
		.def("GetFogStartAddress", &CLightManager::GetFogStartAddress)
		.def("GetFogEndAddress", &CLightManager::GetFogEndAddress)
		.def("GetFogDensityAddress", &CLightManager::GetFogDensityAddress)
		.def("AddLight", &CLightManager::AddLight)
		.def("GetAmbientLight", &CLightManager::GetAmbientLight)
		.def("GetFogParameters", &CLightManager::GetFogParameters)
		.def("GetFogColor", &CLightManager::GetFogColor)
	];
}

//std::vector<CMaterial *> l_Materials;
//const std::vector<CMaterial *> & GetHardCodeMaterials()
//{
//	l_Materials.clear();
//	CTemplatedMapManager<CMaterial>::TMapResource &l_MaterialsMap=CEngine::GetSingleton().GetMaterialManager()->GetResourcesMap();
//	for(CTemplatedMapManager<CMaterial>::TMapResource::iterator it=l_MaterialsMap.begin();it!=l_MaterialsMap.end();++it)
//		l_Materials.push_back(it->second);
//
//	return l_Materials;
//}

void CLuabindManager::RegisterMaterials()
{
	module(LUA_STATE)
	[
		class_<CMaterial, CNamed>("CMaterial")
		.def(constructor<CXMLTreeNode>())
		.def("Apply", &CMaterial::Apply)
		.def("GetRenderableObjectTechnique", &CMaterial::GetRenderableObjectTechnique)
		.def("GetParameters", &CMaterial::GetParameters, return_stl_iterator)
		.def("GetTextures", &CMaterial::GetTextures, return_stl_iterator)
		.def("ChangeTexture", &CMaterial::ChangeTexture)
		.def("ApplyLuaPhysxProperties", &CMaterial::ApplyLuaPhysxProperties)
		.def("GetThisLuaAddress", &CMaterial::GetThisLuaAddress)
		.def("GetStaticFrictionLuaAddress", &CMaterial::GetStaticFrictionLuaAddress)
		.def("GetDynamicFrictionLuaAddress", &CMaterial::GetDynamicFrictionLuaAddress)
		.def("GetRestitutionLuaAddress", &CMaterial::GetRestitutionLuaAddress)
		.def("GetStaticFriction", &CMaterial::GetStaticFriction)
		.def("GetDynamicFriction", &CMaterial::GetDynamicFriction)
		.def("GetRestitution", &CMaterial::GetRestitution)
	];

	module(LUA_STATE)
	[
		class_<CTemplatedMapManager<CMaterial>::TMapResource::iterator>("iterator")
	];

	module(LUA_STATE)
	[
		class_<CTemplatedMapManager<CMaterial>::TMapResource>("TMapResources")
		.def("size", &CTemplatedMapManager<CMaterial>::TMapResource::size)
	];

	module(LUA_STATE)
	[
		class_<CTemplatedMapManager<CMaterial>>("CTemplatedMapManager")
		.def("GetResource", &CTemplatedMapManager<CMaterial>::GetResource)
		.def("AddResource", &CTemplatedMapManager<CMaterial>::AddResource)
		.def("GetResourcesMap", &CTemplatedMapManager<CMaterial>::GetResourcesMap)
	];

	module(LUA_STATE)
	[
		class_< CMaterialManager, CTemplatedMapManager<CMaterial>>("CMaterialManager")
		.def("Load", &CMaterialManager::Load)
		.def("Reload", &CMaterialManager::Reload)
		.def("GetLUAMaterials", &CMaterialManager::GetLUAMaterials, return_stl_iterator)
		.def("GetLUAFileNameMaterials", &CMaterialManager::GetLUAFileNameMaterials, return_stl_iterator)
	];

	module(LUA_STATE)
	[
		class_<CMaterialParameter, CNamed>("CMaterialParameter")
		//.def(constructor<CMaterial*,CXMLTreeNode,CMaterialParameter::TMaterialType>())
		.enum_("t_material_type")
		[
			value("float", 0),
			value("vect2f", 1),
			value("vect3f", 2),
			value("vect4f", 3)
		]
		.def("Apply", &CMaterialParameter::Apply)
			.def("GetValueAddress", &CMaterialParameter::GetValueAddress)
			.def("GetMaterialType", &CMaterialParameter::GetMaterialType)
			.def("GetValueLuaAddress", &CMaterialParameter::GetValueLuaAddress)
			.def("GetDescription", &CMaterialParameter::GetDescription)
	];

	module(LUA_STATE)
	[
		class_<CTemplatedMaterialParameter<float>, CMaterialParameter>("CTemplatedMaterialParameter")

		.def("Apply", &CTemplatedMaterialParameter<float>::Apply)
		.def("GetValueAddress", &CTemplatedMaterialParameter<float>::GetValueAddress)
		.def(constructor<CMaterial*, CXMLTreeNode, const float, CMaterialParameter::TMaterialType>())
		.def("GetDescription", &CTemplatedMaterialParameter<float>::GetDescription)
		.def("GetValue", &CTemplatedMaterialParameter<float>::GetValue)
		//.def(constructor<CMaterial*,CXMLTreeNode,const float,CMaterialParameter::TMaterialType>())

		//.def("get_resource", &CTemplatedMaterialParameter<CMaterialParameter>::GetResource) 
		//	.def("add_resource", &CTemplatedMaterialParameter<CMaterialParameter>::AddResource) 
		//.def("get_resources_map", &CTemplatedMaterialParameter<CMaterialParameter>::GetResourcesMap) 
	];

	module(LUA_STATE)
	[
		class_<CTemplatedMaterialParameter<Vect2f>, CMaterialParameter>("CTemplatedMaterialParameter")

		.def("Apply", &CTemplatedMaterialParameter<Vect2f>::Apply)
		.def("GetValueAddress", &CTemplatedMaterialParameter<Vect2f>::GetValueAddress)
		.def(constructor<CMaterial*, CXMLTreeNode, const Vect2f, CMaterialParameter::TMaterialType>())
		.def("GetDescription", &CTemplatedMaterialParameter<Vect2f>::GetDescription)
		.def("GetValue", &CTemplatedMaterialParameter<Vect2f>::GetValue)
	];

	module(LUA_STATE)
	[
		class_<CTemplatedMaterialParameter<Vect3f>, CMaterialParameter>("CTemplatedMaterialParameter")

		.def("Apply", &CTemplatedMaterialParameter<Vect3f>::Apply)
		.def("GetValueAddress", &CTemplatedMaterialParameter<Vect3f>::GetValueAddress)
		.def(constructor<CMaterial*, CXMLTreeNode, const Vect3f, CMaterialParameter::TMaterialType>())
		.def("GetDescription", &CTemplatedMaterialParameter<Vect3f>::GetDescription)
		.def("GetValue", &CTemplatedMaterialParameter<Vect3f>::GetValue)
	];

	module(LUA_STATE)
	[
		class_<CTemplatedMaterialParameter<Vect4f>, CMaterialParameter>("CTemplatedMaterialParameter")

		.def("Apply", &CTemplatedMaterialParameter<Vect4f>::Apply)
		.def("GetValueAddress", &CTemplatedMaterialParameter<Vect4f>::GetValueAddress)
		.def(constructor<CMaterial*, CXMLTreeNode, const Vect4f, CMaterialParameter::TMaterialType>())
		.def("GetDescription", &CTemplatedMaterialParameter<Vect4f>::GetDescription)
		.def("GetValue", &CTemplatedMaterialParameter<Vect4f>::GetValue)
	];

	/*module(LUA_STATE)
	[
	class_< CTemplatedMaterialParameter<float>, CTemplatedMaterialParameter<CMaterialParameter>>("CTemplatedFloatMaterialParameter")
	];*/

	module(LUA_STATE)
	[
		class_<CTexture, CNamed>("CTexture")
		.def(constructor<>())
		.def("Load", &CTexture::Load)
		.def("Activate", &CTexture::Activate)
		.def("Reload", &CTexture::Reload)
	];

	module(LUA_STATE)
	[
		class_<CTemplatedMapManager<CTexture>>("CTemplatedMapManager")
		.def("GetResource", &CTemplatedMapManager<CTexture>::GetResource)
		.def("AddResource", &CTemplatedMapManager<CTexture>::AddResource)
	];

	module(LUA_STATE)
	[
		class_< CTextureManager, CTemplatedMapManager<CTexture>>("CTextureManager")
		.def("GetTexture", &CTextureManager::GetTexture)
		.def("Reload", &CTextureManager::Reload)
	];
	/*luabind::module(LUA_STATE) [ luabind::def("GetHardCodeMaterials",&GetHardCodeMaterials, return_stl_iterator) ];*/
	//REGISTER_LUA_FUNCTION("GetHardCodeMaterials", &GetHardCodeMaterials);
}

void CLuabindManager::RegisterParticles()
{
	module(LUA_STATE)
	[
		class_<CParticleSystemType, CNamed>("CParticleSystemType")
		.def(constructor<CXMLTreeNode>())

		.def("GetLUAControlPointsSize", &CParticleSystemType::GetLUAControlPointsSize, return_stl_iterator)
		.def("GetLUAControlPointsColor", &CParticleSystemType::GetLUAControlPointsColor, return_stl_iterator)

		.def("GetLUAControlPointsSizeSize", &CParticleSystemType::GetLUAControlPointsSizeSize)
		.def("GetLUAControlPointsColorSize", &CParticleSystemType::GetLUAControlPointsColorSize)

		.def("GetMaterial", &CParticleSystemType::GetMaterial)

		//.def("size", &CTemplatedMapManager<CParticleSystemType>::TMapResource::size)
		.def("AddControlPointSize", &CParticleSystemType::AddControlPointSize)
		.def("AddControlPointColor", &CParticleSystemType::AddControlPointColor)

		.def("RemoveControlPointSize", &CParticleSystemType::RemoveControlPointSize)
		.def("RemoveControlPointColor", &CParticleSystemType::RemoveControlPointColor)

		.def("GetNumFramesLuaAddress", &CParticleSystemType::GetNumFramesLuaAddress)
		.def("GetTimePerFrameLuaAddress", &CParticleSystemType::GetTimePerFrameLuaAddress)
		.def("GetLoopLuaAddress", &CParticleSystemType::GetLoopLuaAddress)
		.def("GetEmitAbsoluteLuaAddress", &CParticleSystemType::GetEmitAbsoluteLuaAddress)

		.def("GetStartingDirectionAngleLuaAddress", &CParticleSystemType::GetStartingDirectionAngleLuaAddress)
		.def("GetStartingAccelerationAngleLuaAddress", &CParticleSystemType::GetStartingAccelerationAngleLuaAddress)
		.def("GetSizeLuaAddress", &CParticleSystemType::GetSizeLuaAddress)

		.def("GetEmitRateLuaAddress", &CParticleSystemType::GetEmitRateLuaAddress)
		.def("GetAwakeTimeLuaAddress", &CParticleSystemType::GetAwakeTimeLuaAddress)
		.def("GetSleepTimeLuaAddress", &CParticleSystemType::GetSleepTimeLuaAddress)
		.def("GetLifeLuaAddress", &CParticleSystemType::GetLifeLuaAddress)

		.def("GetStartingAngleLuaAddress", &CParticleSystemType::GetStartingAngleLuaAddress)
		.def("GetStartingAngularSpeedLuaAddress", &CParticleSystemType::GetStartingAngularSpeedLuaAddress)
		.def("GetAngularAccelerationLuaAddress", &CParticleSystemType::GetAngularAccelerationLuaAddress)

		.def("GetStartingSpeed1LuaAddress", &CParticleSystemType::GetStartingSpeed1LuaAddress)
		.def("GetStartingSpeed2LuaAddress", &CParticleSystemType::GetStartingSpeed2LuaAddress)
		.def("GetStartingAcceleration1LuaAddress", &CParticleSystemType::GetStartingAcceleration1LuaAddress)
		.def("GetStartingAcceleration2LuaAddress", &CParticleSystemType::GetStartingAcceleration2LuaAddress)
		.def("GetColor1LuaAddress", &CParticleSystemType::GetColor1LuaAddress)
		.def("GetColor2LuaAddress", &CParticleSystemType::GetColor2LuaAddress)

		.def("GetThisLuaAddress", &CParticleSystemType::GetThisLuaAddress)

		.def_readonly("m_NumFrames", &CParticleSystemType::m_NumFrames)
		.def_readonly("m_TimerPerFrame", &CParticleSystemType::m_TimerPerFrame)
		.def_readonly("m_LoopFrames", &CParticleSystemType::m_LoopFrames)
		.def_readonly("m_EmitAbsolute", &CParticleSystemType::m_EmitAbsolute)

		.def_readonly("m_StartingDirectionAngle", &CParticleSystemType::m_StartingDirectionAngle)
		.def_readonly("m_StartingAccelerationAngle", &CParticleSystemType::m_StartingAccelerationAngle)
		.def_readonly("m_Size", &CParticleSystemType::m_Size)

		.def_readonly("m_EmitRate", &CParticleSystemType::m_EmitRate)
		.def_readonly("m_AwakeTime", &CParticleSystemType::m_AwakeTime)
		.def_readonly("m_SleepTime", &CParticleSystemType::m_SleepTime)
		.def_readonly("m_Life", &CParticleSystemType::m_Life)

		.def_readonly("m_StartingAngle", &CParticleSystemType::m_StartingAngle)
		.def_readonly("m_StartingAngularSpeed", &CParticleSystemType::m_StartingAngularSpeed)
		.def_readonly("m_AngularAcceleration", &CParticleSystemType::m_AngularAcceleration)

		.def_readonly("m_StartingSpeed1", &CParticleSystemType::m_StartingSpeed1)
		.def_readonly("m_StartingSpeed2", &CParticleSystemType::m_StartingSpeed2)
		.def_readonly("m_StartingAcceleration1", &CParticleSystemType::m_StartingAcceleration1)
		.def_readonly("m_StartingAcceleration2", &CParticleSystemType::m_StartingAcceleration2)

		.def_readonly("m_Color1", &CParticleSystemType::m_Color1)
		.def_readonly("m_Color2", &CParticleSystemType::m_Color2)

		.def("GetControlPointColorTimeLuaAddress", &CParticleSystemType::GetControlPointColorTimeLuaAddress)
		.def("GetControlPointColorColor1LuaAddress", &CParticleSystemType::GetControlPointColorColor1LuaAddress)
		.def("GetControlPointColorColor2LuaAddress", &CParticleSystemType::GetControlPointColorColor2LuaAddress)

		.def("GetControlPointSizeTimeLuaAddress", &CParticleSystemType::GetControlPointSizeTimeLuaAddress)
		.def("GetControlPointSizeSizeLuaAddress", &CParticleSystemType::GetControlPointSizeSizeLuaAddress)
	];

	module(LUA_STATE)
	[
		class_<CParticleSystemType::ControlPointColor>("ControlPointColor")
		.def(constructor<CColor, CColor, Vect2f>())
		.def_readonly("m_Color1", &CParticleSystemType::ControlPointColor::m_Color1)
		.def_readonly("m_Color2", &CParticleSystemType::ControlPointColor::m_Color2)
		.def_readonly("m_Time", &CParticleSystemType::ControlPointColor::m_Time)
	/*	.def("GetTimeLuaAddress", &CParticleSystemType::ControlPointColor::GetTimeLuaAddress)
		.def("GetColor1LuaAddress", &CParticleSystemType::ControlPointColor::GetColor1LuaAddress)
		.def("GetColor2LuaAddress", &CParticleSystemType::ControlPointColor::GetColor2LuaAddress)
		.def("GetThisLuaAddress", &CParticleSystemType::ControlPointColor::GetThisLuaAddress)*/
	];

	module(LUA_STATE)
	[
		class_<CParticleSystemType::ControlPointSize>("ControlPointSize")
		.def(constructor<Vect2f, Vect2f>())
		.def_readonly("m_Size", &CParticleSystemType::ControlPointSize::m_Size)
		.def_readonly("m_Time", &CParticleSystemType::ControlPointSize::m_Time)
		/*.def("GetSizeLuaAddress", &CParticleSystemType::ControlPointSize::GetSizeLuaAddress)
		.def("GetTimeLuaAddress", &CParticleSystemType::ControlPointSize::GetTimeLuaAddress)
		.def("GetThisLuaAddress", &CParticleSystemType::ControlPointSize::GetThisLuaAddress)*/
	];

	module(LUA_STATE)
	[
		class_<CParticleSystemInstance, CRenderableObject>("CParticleSystemInstance")
		.def(constructor<CXMLTreeNode>())
		.def("Render", &CParticleSystemInstance::Render)
		.def("Update", &CParticleSystemInstance::Update)
		.def("GetType", &CParticleSystemInstance::GetType)

		.def("GetEmissionBoxHalfSizeLuaAddress", &CParticleSystemInstance::GetEmissionBoxHalfSizeLuaAddress)
	];


	module(LUA_STATE)
	[
		class_<CTemplatedMapManager<CParticleSystemType>::TMapResource::iterator>("iterator")
	];

	module(LUA_STATE)
	[
		class_<CTemplatedMapManager<CParticleSystemType>::TMapResource>("TMapResources")
		.def("size", &CTemplatedMapManager<CParticleSystemType>::TMapResource::size)
	];

	module(LUA_STATE)
	[
		class_<CTemplatedMapManager<CParticleSystemType>>("CTemplatedMapManager")
		.def("GetResource", &CTemplatedMapManager<CParticleSystemType>::GetResource)
		.def("AddResource", &CTemplatedMapManager<CParticleSystemType>::AddResource)
		.def("GetResourcesMap", &CTemplatedMapManager<CParticleSystemType>::GetResourcesMap)
	];

	module(LUA_STATE)
	[
		class_< CParticleManager, CTemplatedMapManager<CParticleSystemType>>("CParticleManager")
		.def("Load", &CParticleManager::Load)
		.def("Reload", &CParticleManager::Reload)
		.def("GetLUAParticles", &CParticleManager::GetLUAParticles, return_stl_iterator)
		.def("GetDefaultType", &CParticleManager::GetDefaultType)
	];

}

void CLuabindManager::RegisterRenderableObjects()
{
	module(LUA_STATE)
	[
		class_<CTemplatedVectorMapManager<CRenderableObjectsManager>::TVectorResources>("TMapResources")
		.def("size", &CTemplatedVectorMapManager<CRenderableObjectsManager>::TVectorResources::size)
	];

	module(LUA_STATE)
	[
		class_<CTemplatedVectorMapManager<CRenderableObjectsManager>>("CTemplatedVectorMapManager")
		.def("GetResource", &CTemplatedVectorMapManager<CRenderableObjectsManager>::GetResource)
		.def("GetResourceById", &CTemplatedVectorMapManager<CRenderableObjectsManager>::GetResourceById)
		.def("AddResource", &CTemplatedVectorMapManager<CRenderableObjectsManager>::AddResource)
		.def("RemoveResource", &CTemplatedVectorMapManager<CRenderableObjectsManager>::RemoveResource)
		.def("GetResourcesMap", &CTemplatedVectorMapManager<CRenderableObjectsManager>::GetResourcesMap)
		.def("GetResourcesVector", &CTemplatedVectorMapManager<CRenderableObjectsManager>::GetResourcesVector)
	];

	module(LUA_STATE)
	[
		class_< CLayerManager, CTemplatedVectorMapManager<CRenderableObjectsManager>>("CLayerManager")
		.def("Update", &CLayerManager::Update)
		//.def("render", &CLayerManager::Render) 
		//.def("render", (void(*)(*CRenderManager))&CLayerManager::Render)
		.def("Destroy", &CLayerManager::Destroy)
		.def("Load", &CLayerManager::Load)
		.def("Reload", &CLayerManager::Reload)
		.def("AddParticleSystemInstance", &CLayerManager::AddParticleSystemInstance)
		/*.def("AddMeshInstance", &CLayerManager::AddMeshInstance)
		.def("AddAnimatedInstanceModel", &CLayerManager::AddAnimatedInstanceModel)*/
		
	/*	.def("RemoveLayerComponent", &CLayerManager::RemoveLayerComponent)
		.def("RemoveLayerComponents", &CLayerManager::RemoveLayerComponents)
		.def("RemoveComponent", &CLayerManager::RemoveComponent)
		.def("RemoveComponents", &CLayerManager::RemoveComponents)
		
		.def("RemoveLayerLuaComponents", &CLayerManager::RemoveLayerLuaComponents)
		.def("RemoveLuaComponents", &CLayerManager::RemoveLuaComponents)*/
		
		
	];

	module(LUA_STATE)
	[
		class_<CRenderableObjectTechnique, CNamed>("CRenderableObjectTechnique")
		.def(constructor<const std::string, CEffectTechnique*>())
		.def("SetEffectTechnique", &CRenderableObjectTechnique::SetEffectTechnique)
		.def("GetEffectTechnique", &CRenderableObjectTechnique::GetEffectTechnique)

	];

	module(LUA_STATE)
	[
		class_<CRenderableObjectTechniqueManager>("CRenderableObjectTechniqueManager")
		.def("InsertRenderableObjectTechnique", &CRenderableObjectTechniqueManager::InsertRenderableObjectTechnique)
		.def("Destroy", &CRenderableObjectTechniqueManager::Destroy)
		.def("Load", &CRenderableObjectTechniqueManager::Load)
		.def("Reload", &CRenderableObjectTechniqueManager::Reload)
	];

	module(LUA_STATE)
	[
		class_<CTemplatedMapManager<CRenderableObjectTechniqueManager>>("CTemplatedMapManager")
		.def("GetResource", &CTemplatedMapManager<CRenderableObjectTechniqueManager>::GetResource)
		.def("AddResource", &CTemplatedMapManager<CRenderableObjectTechniqueManager>::AddResource)
	];

	module(LUA_STATE)
	[
		class_<CRenderableObject, bases<CComponent, C3DElement>>("CRenderableObject")
		//.def(constructor<>())
		.def("Update", &CRenderableObject::Update)
		.def("Render", &CRenderableObject::Render)
		.def("GetThisLuaAddress", &CRenderableObject::GetThisLuaAddress)
		
		.def("GetClassType", &CRenderableObject::GetClassType)
		.enum_("TRenderableObjectType")
		[
			value("MESH_INSTANCE", 0),
			value("ANIMATED_INSTANCE", 1),
			value("CINEMATIC", 2),
			value("PARTICLE_EMITER", 3)
		]
	];

	module(LUA_STATE)
	[
		class_<CMeshInstance, CRenderableObject>("CMeshInstance")
		.def(constructor<CXMLTreeNode>())
		.def(constructor<CGameObject*, const std::string, const std::string, const Vect3f, float, float, float>())
		.def("Render", &CMeshInstance::Render)
		.def("SetParent", &CMeshInstance::SetParent)
		.def("GetStaticMesh", &CMeshInstance::GetStaticMesh)
	];

	//GUIA TEMPLATED VECTOR
	module(LUA_STATE)
	[
		class_<CTemplatedVectorMapManager<CRenderableObject>::TVectorResources>("TMapResources")
		.def("size", &CTemplatedVectorMapManager<CRenderableObject>::TVectorResources::size)
	];

	module(LUA_STATE)
	[
		class_<CTemplatedVectorMapManager<CRenderableObject>>("CTemplatedVectorMapManager")
		.def("GetResource", &CTemplatedVectorMapManager<CRenderableObject>::GetResource)
		.def("GetResourceById", &CTemplatedVectorMapManager<CRenderableObject>::GetResourceById)
		.def("AddResource", &CTemplatedVectorMapManager<CRenderableObject>::AddResource)
		.def("RemoveResource", &CTemplatedVectorMapManager<CRenderableObject>::RemoveResource)
		.def("GetResourcesMap", &CTemplatedVectorMapManager<CRenderableObject>::GetResourcesMap)
		.def("GetResourcesVector", &CTemplatedVectorMapManager<CRenderableObject>::GetResourcesVector)
	];

	module(LUA_STATE)
	[
		class_< CRenderableObjectsManager, bases<CTemplatedVectorMapManager<CRenderableObject>, CNamed>>("CRenderableObjectsManager")
		.def("Update", &CRenderableObjectsManager::Update)
		.def("Render", &CRenderableObjectsManager::Render)
		/*.def("RemoveRenderableObjectsLuaComponents", &CRenderableObjectsManager::RemoveRenderableObjectsLuaComponents)
		.def("RemoveRenderableObjectsComponent", &CRenderableObjectsManager::RemoveRenderableObjectsComponent)
		.def("RemoveRenderableObjectsComponents", &CRenderableObjectsManager::RemoveRenderableObjectsComponents)*/
	];

	//Static Meshes
	module(LUA_STATE)
	[
		class_<CStaticMesh, CNamed>("CStaticMesh")
		.def(constructor<const CXMLTreeNode&>())
		.def("Load", &CStaticMesh::Load)
		.def("Reload", &CStaticMesh::Reload)
		.def("Render", &CStaticMesh::Render)
		.def("GetThisLuaAddress", &CStaticMesh::GetThisLuaAddress)
		.def("GetBoundingBoxMin", &CStaticMesh::GetBoundingBoxMin)
		.def("GetBoundingBoxMax", &CStaticMesh::GetBoundingBoxMax)
		.def("GetBoundingBoxSize", &CStaticMesh::GetBoundingBoxSize)
		.def("GetBoundingSphereCenter", &CStaticMesh::GetBoundingSphereCenter)
		.def("GetBoundingSphereRadius", &CStaticMesh::GetBoundingSphereRadius)
		.def("GetCapsuleHalfHeight", &CStaticMesh::GetCapsuleHalfHeight)
		.def("GetCapsuleRadius", &CStaticMesh::GetCapsuleRadius)
	];

	module(LUA_STATE)
	[
		class_<CTemplatedMapManager<CStaticMesh>>("CTemplatedMapManager")
		.def("GetResource", &CTemplatedMapManager<CStaticMesh>::GetResource)
		.def("AddResource", &CTemplatedMapManager<CStaticMesh>::AddResource)
	];

	module(LUA_STATE)
	[
		class_< CStaticMeshManager, CTemplatedMapManager<CStaticMesh>>("CStaticMeshManager")
		.def("Load", &CStaticMeshManager::Load)
		.def("Reload", &CStaticMeshManager::Reload)
		.def("GetLUAStaticMeshes", &CStaticMeshManager::GetLUAStaticMeshes, return_stl_iterator)
	];
}

void CLuabindManager::RegisterSceneRendererCommands()
{
	module(LUA_STATE)
	[
		class_<CTemplatedVectorMapManager<CSceneRendererCommand>>("CTemplatedVectorMapManager")
		.def("GetResource", &CTemplatedVectorMapManager<CSceneRendererCommand>::GetResource)
		.def("GetResourceById", &CTemplatedVectorMapManager<CSceneRendererCommand>::GetResourceById)
		.def("AddResource", &CTemplatedVectorMapManager<CSceneRendererCommand>::AddResource)
		.def("RemoveResource", &CTemplatedVectorMapManager<CSceneRendererCommand>::RemoveResource)
		.def("GetResourcesMap", &CTemplatedVectorMapManager<CSceneRendererCommand>::GetResourcesMap)
		.def("GetResourcesVector", &CTemplatedVectorMapManager<CSceneRendererCommand>::GetResourcesVector)
	];

	module(LUA_STATE)
	[
		class_< CSceneRendererCommandManager, CTemplatedVectorMapManager<CSceneRendererCommand>>("CSceneRendererCommandManager")
		.def("Reload", &CSceneRendererCommandManager::Reload)
		.def("Load", &CSceneRendererCommandManager::Load)
		.def("Execute", &CSceneRendererCommandManager::Execute)
		.def("GetLUASceneRendererCommands", &CSceneRendererCommandManager::GetLUASceneRendererCommands, return_stl_iterator)
	];

	module(LUA_STATE)
	[
		class_<CActive>("CActive")
		.def(constructor<CXMLTreeNode>())
		.def("SetActive", &CActive::SetActive)
		.def("GetActive", &CActive::GetActive)
		.def("GetActiveLuaAddress", &CActive::GetActiveLuaAddress)
	];

	module(LUA_STATE)
	[
		class_<CSceneRendererCommand, bases<CActive, CNamed>>("CSceneRendererCommand")
		.def("execute", &CSceneRendererCommand::Execute)
		.def("GetThisLuaAddress", &CSceneRendererCommand::GetThisLuaAddress)
	];
}
