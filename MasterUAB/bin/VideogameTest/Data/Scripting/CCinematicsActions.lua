class 'CCinematicsAction'

function CCinematicsAction:__init(XMLTreeNode)
	self.m_Time=XMLTreeNode:GetFloatProperty("time", 0.0,false)
	self.m_Duration=XMLTreeNode:GetFloatProperty("duration", 0.0,false)
end

function CCinematicsAction:GetTime()
	return self.m_Time
end

function CCinematicsAction:SetTime(Value)
	self.m_Time=Value
end

function CCinematicsAction:GetDuration()
	return self.m_Duration
end

function CCinematicsAction:IsActive()
	--return self.m_Active
	return true
end

function CCinematicsAction:Update(ElapsedTime)
end

function CCinematicsAction:MustUpdate()
	return true
end

function CCinematicsAction:Execute()
end

--Cameras
class 'CCinematicsActionSetCamera' (CCinematicsAction)
function CCinematicsActionSetCamera:__init(XMLTreeNode)
	CCinematicsAction.__init(self, XMLTreeNode)
	self.m_Name=XMLTreeNode:GetPszProperty("name","",false)
end
function CCinematicsActionSetCamera:Execute()
	--local l_CameraControllerManager=g_Engine:GetCameraControllerManager()
	g_CameraControllerManager:SetCurrentCameraController(self.m_Name)
end

class 'CCinematicsActionPlayCinematic' (CCinematicsAction)
function CCinematicsActionPlayCinematic:__init(XMLTreeNode)
	CCinematicsAction.__init(self, XMLTreeNode)
	self.m_Name=XMLTreeNode:GetPszProperty("name","",false)
	self.m_Layer=XMLTreeNode:GetPszProperty("layer","",false)
end
function CCinematicsActionPlayCinematic:Execute()
	local l_Cinematic = g_LayerManager:GetResource(self.m_Layer):GetResource(self.m_Name)
	l_Cinematic:Play(true)
end

class 'CCinematicsActionAddCameraYaw' (CCinematicsAction)
function CCinematicsActionAddCameraYaw:__init(XMLTreeNode)
	CCinematicsAction.__init(self, XMLTreeNode)
	self.m_Radians=XMLTreeNode:GetFloatProperty("radians",0.0,false)
end
function CCinematicsActionAddCameraYaw:Execute()
	local l_CameraControllerManager=g_Engine:GetCameraControllerManager()
	l_CameraControllerManager:GetCurrentCameraController():AddYaw(self.m_Radians)
end

class 'CCinematicsActionAddCameraPitch' (CCinematicsAction)
function CCinematicsActionAddCameraPitch:__init(XMLTreeNode)
	CCinematicsAction.__init(self, XMLTreeNode)
	self.m_Radians=XMLTreeNode:GetFloatProperty("radians",0.0,false)
end
function CCinematicsActionAddCameraPitch:Execute()
	local l_CameraControllerManager=g_Engine:GetCameraControllerManager()
	l_CameraControllerManager:GetCurrentCameraController():AddPitch(self.m_Radians)
end

-- Animated Models
class 'CCinematicsActionBlendCycle' (CCinematicsAction)
function CCinematicsActionBlendCycle:__init(XMLTreeNode)
	CCinematicsAction.__init(self, XMLTreeNode)
	self.m_Layer=XMLTreeNode:GetPszProperty("layer", "",false)
	self.m_ModelName=XMLTreeNode:GetPszProperty("name", "",false)
	self.m_ID=XMLTreeNode:GetIntProperty("id",0,false)
	self.m_Weight=XMLTreeNode:GetFloatProperty("weight", 0.0, false)
	self.m_DelayIn=XMLTreeNode:GetFloatProperty("delay_in",0.1, false)
end
function CCinematicsActionBlendCycle:Execute()
	local l_ROManager=g_LayerManager:GetResource(self.m_Layer)
	local l_AnimatedModel = l_ROManager:GetResource(self.m_ModelName)
	l_AnimatedModel:BlendCycle(self.m_ID,self.m_Weight,self.m_DelayIn)
end

class 'CCinematicsActionClearCycle' (CCinematicsAction)
function CCinematicsActionClearCycle:__init(XMLTreeNode)
	CCinematicsAction.__init(self, XMLTreeNode)
	self.m_Layer=XMLTreeNode:GetPszProperty("layer", "",false)
	self.m_ModelName=XMLTreeNode:GetPszProperty("name", "",false)
	self.m_ID=XMLTreeNode:GetIntProperty("id",0,false)
	self.m_DelayOut=XMLTreeNode:GetFloatProperty("delay_out",0.1, false)
end
function CCinematicsActionClearCycle:Execute()
	local l_ROManager=g_LayerManager:GetResource(self.m_Layer)
	local l_AnimatedModel = l_ROManager:GetResource(self.m_ModelName)
	l_AnimatedModel:ClearCycle(self.m_ID,self.m_DelayOut)
end

class 'CCinematicsActionExecuteAction' (CCinematicsAction)
function CCinematicsActionExecuteAction:__init(XMLTreeNode)
	CCinematicsAction.__init(self, XMLTreeNode)
	self.m_Layer=XMLTreeNode:GetPszProperty("layer", "",false)
	self.m_ModelName=XMLTreeNode:GetPszProperty("name", "",false)
	self.m_ID=XMLTreeNode:GetIntProperty("id",0,false)
	self.m_DelayIn=XMLTreeNode:GetFloatProperty("delay_in",0.1,false)
	self.m_DelayOut=XMLTreeNode:GetFloatProperty("delay_out",0.1,false)
	self.m_Weight=XMLTreeNode:GetFloatProperty("weight",1.0,false)
	self.m_AutoLock=XMLTreeNode:GetBoolProperty("autolock",false,false)
end
function CCinematicsActionExecuteAction:Execute()
	local l_ROManager=g_LayerManager:GetResource(self.m_Layer)
	local l_AnimatedModel = l_ROManager:GetResource(self.m_ModelName)
	l_AnimatedModel:ExecuteAction(self.m_ID,m_DelayIn,m_DelayOut,m_Weight,self.AutoLock)
end

-- 3D Element
class 'CCinematicsActionAddYaw' (CCinematicsAction)
function CCinematicsActionAddYaw:__init(XMLTreeNode)
	CCinematicsAction.__init(self, XMLTreeNode)
	self.m_Layer=XMLTreeNode:GetPszProperty("layer", "",false)
	self.m_Name=XMLTreeNode:GetPszProperty("name","",false)
	self.m_Value=XMLTreeNode:GetFloatProperty("value",0.0,false)
end
function CCinematicsActionAddYaw:Update()
	local l_ROManager = g_LayerManager:GetResource(self.m_Layer)
	local l_3DElement = l_ROManager:GetResource(self.m_Name)
	g_PhysXManager:MoveKinematicActor(self.m_Name,Quatf(l_3DElement:GetYaw()+self.m_Value),l_3DElement:GetPitch(),l_3DElement:GetRoll()) 
end

class 'CCinematicsActionAddPitch' (CCinematicsAction)
function CCinematicsActionAddPitch:__init(XMLTreeNode)
	CCinematicsAction.__init(self, XMLTreeNode)
	self.m_Layer=XMLTreeNode:GetPszProperty("layer", "",false)
	self.m_Name=XMLTreeNode:GetPszProperty("name","",false)
	self.m_Value=XMLTreeNode:GetFloatProperty("value",0.0,false)
end
function CCinematicsActionAddPitch:Update()
	local l_ROManager = g_LayerManager:GetResource(self.m_Layer)
	local l_3DElement = l_ROManager:GetResource(self.m_Name)
	g_PhysXManager:MoveKinematicActor(self.m_Name,Quatf(l_3DElement:GetYaw(),l_3DElement:GetPitch()+self.m_Value,l_3DElement:GetRoll())) 
end

class 'CCinematicsActionAddRoll' (CCinematicsAction)
function CCinematicsActionAddRoll:__init(XMLTreeNode)
	CCinematicsAction.__init(self, XMLTreeNode)
	self.m_Layer=XMLTreeNode:GetPszProperty("layer", "",false)
	self.m_Name=XMLTreeNode:GetPszProperty("name","",false)
	self.m_Value=XMLTreeNode:GetFloatProperty("value",0.0,false)
end
function CCinematicsActionAddRoll:Update()
	local l_ROManager = g_LayerManager:GetResource(self.m_Layer)
	local l_3DElement = l_ROManager:GetResource(self.m_Name)
	g_PhysXManager:MoveKinematicActor(self.m_Name,Quatf(l_3DElement:GetYaw(),l_3DElement:GetPitch(),l_3DElement:GetRoll()+self.m_Value)) 
end

class 'CCinematicsActionAddYawPitchRoll' (CCinematicsAction)
function CCinematicsActionAddYawPitchRoll:__init(XMLTreeNode)
	CCinematicsAction.__init(self, XMLTreeNode)
	self.m_Layer=XMLTreeNode:GetPszProperty("layer", "",false)
	self.m_Name=XMLTreeNode:GetPszProperty("name","",false)
	self.m_Value=XMLTreeNode:GetVect3fProperty("value",Vect3f(0.0,0.0,0.0),false)
end
function CCinematicsActionAddYawPitchRoll:Execute()
	local l_ROManager = g_LayerManager:GetResource(self.m_Layer)
	local l_3DElement = l_ROManager:GetResource(self.m_Name)
	l_3DElement:SetYawPitchRoll(self.m_Value.x,self.m_Value.y,self.m_Value.z)
end

class 'CCinematicsActionSetPosition' (CCinematicsAction)
function CCinematicsActionSetPosition:__init(XMLTreeNode)
	CCinematicsAction.__init(self, XMLTreeNode)
	self.m_Layer=XMLTreeNode:GetPszProperty("layer", "",false)
	self.m_Name=XMLTreeNode:GetPszProperty("name","",false)
	self.m_Value=XMLTreeNode:GetVect3fProperty("value",Vect3f(0.0,0.0,0.0),false)
end
function CCinematicsActionSetPosition:Execute()
	local l_ROManager = g_LayerManager:GetResource(self.m_Layer)
	local l_3DElement = l_ROManager:GetResource(self.m_Name)
	l_3DElement:SetPosition(self.m_Value)
end


