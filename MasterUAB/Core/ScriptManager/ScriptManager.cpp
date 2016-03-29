#include "ScriptManager.h"
#include <assert.h>
#include <luabind/luabind.hpp>

#include <luabind/operator.hpp>
#include <luabind/iterator_policy.hpp>

#include "Engine.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Utils\TemplatedMapManager.h"
#include "Utils\EmptyPointerClass.h"

#include "Materials\MaterialManager.h"
#include "Materials\TemplatedMaterialParameter.h"
#include "Effects\EffectManager.h"
#include "Textures\TextureManager.h"

#include "Particles\ParticleManager.h"
#include "Particles\ParticleSystemInstance.h"

#include "RenderableObjects\LayerManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "RenderableObjects\MeshInstance.h"

#include "SceneRendererCommands\SceneRendererCommandManager.h"

#include "Render\RenderManager.h"
#include "Camera\Camera.h"
#include "Camera\CameraControllerManager.h"
#include "Camera\FixedCameraController.h"
#include "Camera\ThirdPersonCameraController.h"

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
#include "testclass.h"
#include "Log.h"
#include "Application.h"

#include "DebugHelperImplementation.h"

#include "Input\InputManagerImplementation.h"
#include "Components\ComponentManager.h"

//Commands
#include "SceneRendererCommands\SetPoolRenderableObjectsTechniqueSceneRendererCommand.h"

using namespace luabind;

#define LUA_STATE CEngine::GetSingleton().GetScriptManager()->GetLuaState()
#define REGISTER_LUA_FUNCTION(FunctionName,AddrFunction) {luabind::module(LUA_STATE) [ luabind::def(FunctionName,AddrFunction) ];}

CScriptManager::CScriptManager()
: m_LS(NULL)
{
}

CScriptManager::~CScriptManager()
{
	Destroy();	
}

int Alert(lua_State * State) 
{  
	std::string l_Text;  
	int n = lua_gettop(State);  
	int i;  lua_getglobal(State, "tostring");  
	for (i=1; i<=n; i++) 
	{   const char *s;   
	lua_pushvalue(State, -1);   
	lua_pushvalue(State, i);   
	lua_call(State, 1, 1); 
  
	 s = lua_tostring(State, -1);   
	 if (s == NULL)     
		return luaL_error(State, "`tostring' must return a string to `print'");   
	 
	 if (i>1) l_Text += '\t';   
	 l_Text += s;   
	 
	 lua_pop(State, 1);   
}  

l_Text += '\n';  

CEngine::GetSingleton().GetLogManager()->Log(l_Text.c_str());
//Info( l_Text.c_str() );  
return true; 

}

int ShowLuaErrorDebugInfo(lua_State* L)
{
	lua_Debug d;
	lua_getstack(L, 1, &d);
	lua_getinfo(L, "Sln", &d);
	std::string err = lua_tostring(L, -1);
	lua_pop(L, 1);
	std::stringstream msg;
	msg << d.short_src << ":" << d.currentline;

	if (d.name != 0)
	{
	msg << "(" << d.namewhat << " " << d.name << ")";
	}
	msg << " " << err;
	CEngine::GetSingleton().GetLogManager()->Log(msg.str().c_str());
	return 0;
}

void CScriptManager::Initialize() 
{  
	m_LS=lua_open();
	luaopen_base(m_LS);
	luaopen_string(m_LS);
	luaopen_table(m_LS);
	luaopen_math(m_LS);
	luaopen_debug(m_LS);
	//Sobreescribimos la función _ALERT de LUA cuando se genere algún error al ejecutar código LUA 
	lua_register(m_LS,"_ALERT",Alert);

	//Initialize luabind
	luabind::open(m_LS);

	lua_atpanic(m_LS, ShowLuaErrorDebugInfo);
	luabind::set_pcall_callback(ShowLuaErrorDebugInfo);

	RegisterLUAFunctions();
	RunFile("./Data/scripting/LuaMain.lua");
	RunCode("LuaMain()");
} 

//Para desinicializar el motor de LUA 
void CScriptManager::Destroy() 
{  
	if(m_LS!=NULL)
	lua_close(m_LS); 
}  

//Para ejecutar un fragmento de código LUA 
void CScriptManager::RunCode(const std::string &Code) const 
{  
	if(luaL_dostring(m_LS,Code.c_str()))  
	{   
		const char *l_Str=lua_tostring(m_LS, -1);   
		//Info("%s",l_Str);
		CEngine::GetSingleton().GetLogManager()->Log(l_Str);
		//assert(!"must be log");
	} 
}  

//Para ejecutar un fichero de código LUA 
void CScriptManager::RunFile(const std::string &FileName) const 
{  
	if(luaL_dofile(m_LS, FileName.c_str()))  
	{   const char *l_Str=lua_tostring(m_LS, -1);   
		CEngine::GetSingleton().GetLogManager()->Log(l_Str);
		//Info("%s",l_Str); 
		//assert(!"must be log");
	} 
}   

void CScriptManager::RegisterLUAFunctions()
{
	RegisterBase();
	RegisterCore();
	RegisterGraphics();
	RegisterGUI();
	RegisterPhysics();
}

void CScriptManager::RegisterGraphics()
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
	];

	RegisterCamera();
	RegisterEffects();
	RegisterLights();
	RegisterMaterials();
	RegisterParticles();
	RegisterRenderableObjects();
	RegisterCinematics();
	RegisterAnimations();
	RegisterSceneRendererCommands();
}

void CScriptManager::RegisterAnimations()
{
	module(LUA_STATE) 
	[
		class_<CAnimatedCoreModel,CNamed>("CAnimatedCoreModel")
		.def(constructor<>())
		.def("GetMaterials", &CAnimatedCoreModel::GetMaterials)
		.def("GetCoreModel", &CAnimatedCoreModel::GetCoreModel)	
		.def("Load", &CAnimatedCoreModel::Load)	
	];

	module(LUA_STATE)
	[
		class_<CAnimatedInstanceModel,CRenderableObject>("CAnimatedInstanceModel")
		.def(constructor<CXMLTreeNode>())
		.def(constructor<const std::string>())
		.def("Initialize", &CAnimatedInstanceModel::Initialize)
		.def("Render", &CAnimatedInstanceModel::Render)	
		.def("Update", &CAnimatedInstanceModel::Update)	
		.def("Destroy", &CAnimatedInstanceModel::Destroy)	
		.def("ExecuteAction", &CAnimatedInstanceModel::ExecuteAction)	
		.def("BlendCycle", &CAnimatedInstanceModel::BlendCycle)	
		.def("ClearCycle", &CAnimatedInstanceModel::ClearCycle)	
		.def("IsCycleAnimationActive", &CAnimatedInstanceModel::IsCycleAnimationActive)	
		.def("IsActionAnimationActive", &CAnimatedInstanceModel::IsActionAnimationActive)	
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

void CScriptManager::RegisterCamera()
{
	module(LUA_STATE) 
	[
		class_<CCamera,CNamed>("CCamera")
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
		class_<CCameraController>("CCameraController")
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
		.enum_("t_camera_controller_type")
		[
			value("FIXED", 0),
			value("KEY", 1)
		]
	];

	module(LUA_STATE) 
	[
		class_<CFixedCameraController,CCameraController>("CFixedCameraController")
		.def(constructor<>())
		.def(constructor<CXMLTreeNode>())
		.def("SetCamera", &CFixedCameraController::SetCamera)
		.def("GetDirection", &CFixedCameraController::GetDirection)
	];

	module(LUA_STATE) 
	[
		class_<CFPSCameraController,CCameraController>("CFPSCameraController")
		.def(constructor<>())
		.def("Move", &CFPSCameraController::Move)
		.def("MoveUpDown", &CFPSCameraController::MoveUpDown)
		.def("SetCamera", &CFPSCameraController::SetCamera)
		.def("AddYaw", &CFPSCameraController::AddYaw)
		.def("AddPitch", &CFPSCameraController::AddPitch)
		.def("GetDirection", &CFPSCameraController::GetDirection)
		.def("GetType", &CFPSCameraController::GetType)
	];

		module(LUA_STATE) 
	[
		class_<CSphericalCameraController,CCameraController>("CSphericalCameraController")
		.def(constructor<>())
		.def("AddZoom", &CSphericalCameraController::AddZoom)
		.def("SetZoom", &CSphericalCameraController::SetZoom)
		.def("SetCamera", &CSphericalCameraController::SetCamera)
		.def("GetDirection", &CSphericalCameraController::GetDirection)
		.def("Update", &CSphericalCameraController::Update)
		.def("GetType", &CSphericalCameraController::GetType)
	];

	module(LUA_STATE) 
	[
		class_<CThirdPersonCameraController,CCameraController>("CThirdPersonCameraController")
		.def(constructor<>())
		.def(constructor<CXMLTreeNode>())
		.def("AddZoom", &CThirdPersonCameraController::AddZoom)
		.def("SetZoom", &CThirdPersonCameraController::SetZoom)
		.def("SetCamera", &CThirdPersonCameraController::SetCamera)
		.def("MoveUpDown", &CThirdPersonCameraController::MoveUpDown)
		.def("Move", &CThirdPersonCameraController::Move)
		.def("AddYaw", &CThirdPersonCameraController::AddYaw)
		.def("AddPitch", &CThirdPersonCameraController::AddPitch)
		.def("GetDirection", &CThirdPersonCameraController::GetDirection)
		.def("Update", &CThirdPersonCameraController::Update)
		.def("GetType", &CThirdPersonCameraController::GetType)
		.def("GetPlayerCameraAngleDif", &CThirdPersonCameraController::GetPlayerCameraAngleDif)
		.def("AddLookAtPitch", &CThirdPersonCameraController::AddLookAtPitch)
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
		.def("GetLocked", &CCameraControllerManager::GetLocked)
		.def("SetLocked", &CCameraControllerManager::SetLocked)
		.def("ChangeLockState", &CCameraControllerManager::ChangeLockState)
	];
}

void CScriptManager::RegisterCinematics()
{
	//Cinematics
	module(LUA_STATE) 
	[
		class_<CCameraKey>("CCameraKey")
		.def(constructor<CCameraInfo,float>())
	];

	module(LUA_STATE) 
	[
		class_<CCameraInfo>("CCameraInfo")
		.def(constructor<>())
		.def(constructor<Vect3f,Vect3f,Vect3f,Vect3f,float,float,float>())
		.def(constructor<CXMLTreeNode>())
	];

	module(LUA_STATE) 
	[
		class_<CCameraKeyController,CCameraController>("CCameraKeyController")
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
		class_<CCinematicObject,CCinematicPlayer>("CCinematicObject")
		.def(constructor<CXMLTreeNode>())
		.def("is_ok", &CCinematicObject::IsOk)	
		.def("add_cinematic_object_keyframe", &CCinematicObject::AddCinematicObjectKeyFrame)
		.def("update", &CCinematicObject::Update)
		.def("stop", &CCinematicObject::Stop)
		.def("on_restart_cycle", &CCinematicObject::OnRestartCycle)
	];

	module(LUA_STATE) 
	[
		class_<CCinematicObjectKeyFrame,C3DElement>("CCinematicObjectKeyFrame")
		.def(constructor<CXMLTreeNode>())
		.def("get_keyframe_time", &CCinematicObjectKeyFrame::GetKeyFrameTime)	
		.def("set_keyframe_time", &CCinematicObjectKeyFrame::SetKeyFrameTime)
	];
}

void CScriptManager::RegisterEffects()
{
	module(LUA_STATE) 
	[
		class_<CEffectTechnique,CNamed>("CEffectTechnique")
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
		.def("ReloadFile", &CEffectManager::ReloadFile) 
		.def("GetLUAEffects", &CEffectManager::GetLUAEffects,return_stl_iterator) 
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
		class_<CEffectPixelShader,CEffectShader>("CEffectPixelShader") 
		.def(constructor<CXMLTreeNode>())
		.def("Load", &CEffectPixelShader::Load)
		.def("SetConstantBuffer", &CEffectPixelShader::SetConstantBuffer) 
		.def("GetPixelShader", &CEffectPixelShader::GetPixelShader) 
	];

	module(LUA_STATE) 
	[ 
		class_<CEffectVertexShader,CEffectShader>("CEffectVertexShader") 
		.def(constructor<CXMLTreeNode>())
		.def("Load", &CEffectVertexShader::Load) 
		.def("SetConstantBuffer", &CEffectVertexShader::SetConstantBuffer) 
		.def("GetVertexShader", &CEffectVertexShader::GetVertexShader)
		.def("GetVertexLayout", &CEffectVertexShader::GetVertexLayout)
	];
}

void CScriptManager::RegisterLights()
{
	module(LUA_STATE) 
	[
		class_<CLight,bases<C3DElement,CNamed>>("CLight")
		.enum_("t_light_type")
        [
            value("omni", 0),
            value("directional", 1),
            value("spot", 2)
        ]
		.def("get_intensity", &CLight::GetIntensity)
		.def("set_intensity", &CLight::SetIntensity)
		.def("get_position", &CLight::GetPosition)
		.def("set_position", &CLight::SetPosition)
		.def("get_color", &CLight::GetColor)
		.def("set_color", &CLight::SetColor)
		.def("get_start_range_attenuation", &CLight::GetStartRangeAttenuation)
		.def("set_start_range_attenuation", &CLight::SetStartRangeAttenuation)
		.def("get_end_range_attenuation", &CLight::GetEndRangeAttenuation)
		.def("set_end_range_attenuation", &CLight::SetEndRangeAttenuation)
		.def("get_type", &CLight::GetType)
		.def("set_type", &CLight::SetType)
		.def("render", &CLight::Render)
		.def("GetActiveAddress", &CLight::GetActiveAddress)
		.def("GetIntensityAddress", &CLight::GetIntensityAddress)
		.def("GetPositionAddress", &CLight::GetPositionAddress)
		.def("GetColorAddress", &CLight::GetColorAddress)
		.def("GetStartRangeAttenuationAddress", &CLight::GetStartRangeAttenuationAddress)
		.def("GetEndRangeAttenuationAddress", &CLight::GetEndRangeAttenuationAddress)
		.scope
		[
			def("get_light_type_by_name", &CLight::GetLightTypeByName) 
		]
	];

	module(LUA_STATE) 
	[
		class_<CDirectionalLight,CLight>("CDirectionalLight")
		.def(constructor<>())
		.def(constructor<CXMLTreeNode>())
		.def("get_direction", &CDirectionalLight::GetDirection)
		.def("set_direction", &CDirectionalLight::SetDirection)
		.def("render", &CDirectionalLight::Render)
		.def("GetDirectionAddress", &CDirectionalLight::GetDirectionAddress)
	];

	module(LUA_STATE) 
	[
		class_<COmniLight,CLight>("COmniLight")
		.def(constructor<>())
		.def(constructor<CXMLTreeNode>())
		.def("render", &COmniLight::Render)
	];

	module(LUA_STATE) 
	[
		class_<CSpotLight,CDirectionalLight>("CSpotLight")
		.def(constructor<>())
		.def(constructor<CXMLTreeNode>())
		.def("get_angle", &CSpotLight::GetAngle)
		.def("get_falloff", &CSpotLight::GetFallOff)
		.def("render", &CSpotLight::Render)
		.def("GetAngleAddress", &CSpotLight::GetAngleAddress)
		.def("GetFallOffAddress", &CSpotLight::GetFallOffAddress)
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
		//.def("GetFogParametersAddress", &CLightManager::GetFogParametersAddress)
		.def("GetFogStartAddress", &CLightManager::GetFogStartAddress)
		.def("GetFogEndAddress", &CLightManager::GetFogEndAddress)
		.def("GetFogDensityAddress", &CLightManager::GetFogDensityAddress)
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

void CScriptManager::RegisterMaterials()
{
	module(LUA_STATE) 
	[
		class_<CMaterial,CNamed>("CMaterial")
		.def(constructor<CXMLTreeNode>())
		.def("Apply", &CMaterial::Apply)
		.def("GetRenderableObjectTechnique", &CMaterial::GetRenderableObjectTechnique)	
		//.def("GetParameters", &CMaterial::GetParameters)	
		.def("GetParameters", &CMaterial::GetParameters,return_stl_iterator) 
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
	];

	module(LUA_STATE) 
	[
		class_<CMaterialParameter,CNamed>("CMaterialParameter")
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
		.def("GetDescription",&CMaterialParameter::GetDescription)
	];


	module(LUA_STATE) 
	[  
		class_<CTemplatedMaterialParameter<float>,CMaterialParameter>("CTemplatedMaterialParameter")  
		
		.def("Apply", &CTemplatedMaterialParameter<float>::Apply) 
		.def("GetValueAddress", &CTemplatedMaterialParameter<float>::GetValueAddress) 
		.def(constructor<CMaterial*,CXMLTreeNode,const float,CMaterialParameter::TMaterialType>())
		.def("GetDescription",&CTemplatedMaterialParameter<float>::GetDescription)
		//.def(constructor<CMaterial*,CXMLTreeNode,const float,CMaterialParameter::TMaterialType>())
		
		//.def("get_resource", &CTemplatedMaterialParameter<CMaterialParameter>::GetResource) 
	//	.def("add_resource", &CTemplatedMaterialParameter<CMaterialParameter>::AddResource) 
		//.def("get_resources_map", &CTemplatedMaterialParameter<CMaterialParameter>::GetResourcesMap) 
	]; 

	module(LUA_STATE) 
	[  
		class_<CTemplatedMaterialParameter<Vect2f>,CMaterialParameter>("CTemplatedMaterialParameter")  
		
		.def("Apply", &CTemplatedMaterialParameter<Vect2f>::Apply) 
		.def("GetValueAddress", &CTemplatedMaterialParameter<Vect2f>::GetValueAddress) 
		.def(constructor<CMaterial*,CXMLTreeNode,const Vect2f,CMaterialParameter::TMaterialType>())
		.def("GetDescription",&CTemplatedMaterialParameter<Vect2f>::GetDescription)
	]; 

	module(LUA_STATE) 
	[  
		class_<CTemplatedMaterialParameter<Vect3f>,CMaterialParameter>("CTemplatedMaterialParameter")  
		
		.def("Apply", &CTemplatedMaterialParameter<Vect3f>::Apply) 
		.def("GetValueAddress", &CTemplatedMaterialParameter<Vect3f>::GetValueAddress) 
		.def(constructor<CMaterial*,CXMLTreeNode,const Vect3f,CMaterialParameter::TMaterialType>())
		.def("GetDescription",&CTemplatedMaterialParameter<Vect3f>::GetDescription)
	]; 

	module(LUA_STATE) 
	[  
		class_<CTemplatedMaterialParameter<Vect4f>,CMaterialParameter>("CTemplatedMaterialParameter")  
		
		.def("Apply", &CTemplatedMaterialParameter<Vect4f>::Apply) 
		.def("GetValueAddress", &CTemplatedMaterialParameter<Vect4f>::GetValueAddress) 
		.def(constructor<CMaterial*,CXMLTreeNode,const Vect4f,CMaterialParameter::TMaterialType>())
		.def("GetDescription",&CTemplatedMaterialParameter<Vect4f>::GetDescription)
	]; 

	/*module(LUA_STATE) 
	[ 
		class_< CTemplatedMaterialParameter<float>, CTemplatedMaterialParameter<CMaterialParameter>>("CTemplatedFloatMaterialParameter")   
	];*/

	module(LUA_STATE) 
	[
		class_<CTexture,CNamed>("CTexture")
		.def(constructor<>())
		.def("Load", &CTexture::Load)
		.def("Activate", &CTexture::Activate)
		.def("reload", &CTexture::Reload)	
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

void CScriptManager::RegisterParticles()
{
	/*module(LUA_STATE)
	[
		class_<CParticleSystemType, CNamed>("CParticleSystemType")
		.def(constructor<CXMLTreeNode>())
	];

	module(LUA_STATE)
	[
		class_<CParticleSystemInstance, CRenderableObject>("CParticleSystemInstance")
		.def("Render", &CParticleSystemInstance::Render)
		.def("Update", &CParticleSystemInstance::Update)
	];*/

}

void CScriptManager::RegisterRenderableObjects()
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
		.def("GetPlayer", &CLayerManager::GetPlayer)
	];
	
	module(LUA_STATE) 
	[
		class_<CRenderableObjectTechnique,CNamed>("CRenderableObjectTechnique")
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
		class_<CRenderableObject,bases<C3DElement,CNamed>>("CRenderableObject")
		//.def(constructor<>())
		.def("Update", &CRenderableObject::Update)
		.def("Render", &CRenderableObject::Render)
		.def("GetComponentManager", &CRenderableObject::GetComponentManager)
		.def("AddComponent", &CRenderableObject::AddComponent)
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
		class_<CMeshInstance,CRenderableObject>("CMeshInstance")
		.def(constructor<CXMLTreeNode>())
		.def(constructor<const std::string,const std::string>())
		.def("Render", &CMeshInstance::Render)
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
		class_< CRenderableObjectsManager, bases<CTemplatedVectorMapManager<CRenderableObject>,CNamed>>("CRenderableObjectsManager")   
		.def("Update", &CRenderableObjectsManager::Update) 
		.def("Render", &CRenderableObjectsManager::Render) 
		.def("AddMeshInstance", &CRenderableObjectsManager::AddMeshInstance) 
		.def("AddAnimatedInstanceModel", &CRenderableObjectsManager::AddAnimatedInstanceModel) 
		.def("Load", &CRenderableObjectsManager::Load) 
		.def("Reload", &CRenderableObjectsManager::Reload) 
	];

	//Static Meshes
	module(LUA_STATE) 
	[
		class_<CStaticMesh,CNamed>("CStaticMesh")
		.def(constructor<>())
		.def("Load", &CStaticMesh::Load)
		.def("Reload", &CStaticMesh::Reload)	
		.def("Render", &CStaticMesh::Render)	
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
		.def("GetLUAStaticMeshes", &CStaticMeshManager::GetLUAStaticMeshes,return_stl_iterator) 
	];
}

void CScriptManager::RegisterSceneRendererCommands()
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
	];

	module(LUA_STATE) 
	[
		class_<CActive>("CActive")
		.def(constructor<CXMLTreeNode>())
		.def("SetActive", &CActive::SetActive)
		.def("GetActive", &CActive::GetActive)
	];

	module(LUA_STATE) 
	[
		class_<CSceneRendererCommand,bases<CActive,CNamed>>("CSceneRendererCommand")
		.def("execute", &CSceneRendererCommand::Execute)
	];
}

void CScriptManager::Load(const std::string &XMLFile)
{

}