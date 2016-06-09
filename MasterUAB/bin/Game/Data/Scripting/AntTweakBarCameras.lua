function OpenCameras()
	ClickOnElement("Camera")
	g_DebugHelper:RegisterButton("Reload","ReloadCameras()")
	local l_Camera = g_CameraControllerManager:GetCurrentCameraController()
	g_DebugHelper:RegisterFloatParameter("Zoom", l_Camera:GetZoomLuaAddress(), "step=0.01")
	g_DebugHelper:RegisterFloatParameter("Zoom speed", l_Camera:GetZoomSpeedLuaAddress(), "step=0.01")
	g_DebugHelper:RegisterFloatParameter("Max. zoom", l_Camera:GetMaxZoomLuaAddress(), "step=0.01")
	g_DebugHelper:RegisterFloatParameter("Min. zoom", l_Camera:GetMinZoomLuaAddress(), "step=0.01")
	
	g_DebugHelper:RegisterFloatParameter("Speed", l_Camera:GetSpeedLuaAddress(), "step=0.01")
	g_DebugHelper:RegisterFloatParameter("Fast speed", l_Camera:GetFastSpeedLuaAddress(), "step=0.01")
	
	g_DebugHelper:RegisterFloatParameter("Yaw speed", l_Camera:GetYawSpeedLuaAddress(), "step=0.01")
	g_DebugHelper:RegisterFloatParameter("Pitch speed", l_Camera:GetPitchSpeedLuaAddress(), "step=0.01")
	g_DebugHelper:RegisterFloatParameter("Roll speed", l_Camera:GetRollSpeedLuaAddress(), "step=0.01")
end