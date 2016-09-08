function OpenCameras()
	ClickOnElement("Cameras","InitializeDebugBar()")
	g_DebugHelper:RegisterButton("Reload cameras","g_AntTweakbarReloader:ReloadCameras()")
	
	local l_CameraControllers=g_CameraControllerManager:GetLUACameraControllers()
	for l_CameraController in l_CameraControllers do
		g_DebugHelper:RegisterExtendedButton(l_CameraController:GetName(),"OpenCamera",l_CameraController:GetThisLuaAddress(),"camera_controller")
	end
end

function OpenCamera(CameraController)
	ClickOnElement(CameraController:GetName(),"OpenCameras()")
	if CameraController:GetType() == CCameraController.THIRD_PERSON then
		g_DebugHelper:RegisterFloatParameter("Zoom", CameraController:GetZoomLuaAddress(), "step=0.01")
		g_DebugHelper:RegisterFloatParameter("Zoom speed", CameraController:GetZoomSpeedLuaAddress(), "step=0.01")
		g_DebugHelper:RegisterFloatParameter("Max. zoom", CameraController:GetMaxZoomLuaAddress(), "step=0.01")
		g_DebugHelper:RegisterFloatParameter("Min. zoom", CameraController:GetMinZoomLuaAddress(), "step=0.01")
		
		g_DebugHelper:RegisterFloatParameter("Speed", CameraController:GetSpeedLuaAddress(), "step=0.01")
		g_DebugHelper:RegisterFloatParameter("Fast speed", CameraController:GetFastSpeedLuaAddress(), "step=0.01")
		
		g_DebugHelper:RegisterFloatParameter("Yaw speed", CameraController:GetYawSpeedLuaAddress(), "step=0.01")
		g_DebugHelper:RegisterFloatParameter("Pitch speed", CameraController:GetPitchSpeedLuaAddress(), "step=0.01")
		g_DebugHelper:RegisterFloatParameter("Roll speed", CameraController:GetRollSpeedLuaAddress(), "step=0.01")
	
		g_DebugHelper:RegisterFloatParameter("Min look at pitch", CameraController:GetMinLookAtPitchLuaAddress(), "step=0.01")
		g_DebugHelper:RegisterFloatParameter("Max look at pitch", CameraController:GetMaxLookAtPitchLuaAddress(), "step=0.01")
	
	end
	g_DebugHelper:RegisterButton("Set camera","ChangeCameraController('"..CameraController:GetName().."')")
end

function ChangeCameraController(CameraControllerName)
	g_CameraControllerManager:SetCurrentCameraController(CameraControllerName)
end