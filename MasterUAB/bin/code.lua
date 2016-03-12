function LuaMain()
	local l_Lights=UABEngine:GetLightManager():GetLights()
	local l_AntweakBar=UABEngine:GetAntweakBar()
	for i=1, l_Lights:GetSize() do
		local l_Light=l_Lights:GetResourceById(i)
		l_AntweakBar:AddButton("Light "..l_Light:GetName(), "RegisterLight('"..l_Light:GetName().."')")
	end
end

function RegisterLight(LightName)
	local l_AntweakBar=UABEngine:GetAntweakBar()
	local l_Lights=UABEngine:GetLightManager():GetLights()
	local l_Light=l_Lights:GetResource(LightName)
	l_AntweakBar:RegisterParameter("color", "color_type", l_Light:GetColorAddress())
	l_AntweakBar:RegisterParameter("start range attenuation", "float_type", l_Light:GetStartRangeAttenuation())
	l_AntweakBar:RegisterParameter("end range attenuation", "float_type", l_Light:GetEndRangeAttenuation())
end

------

C++
.h
	std::vector<std:string> m_LuaParameter;
	void RegisterLUAButton(const std::string &ButtonName, const std::string &Script);
	static void TW_CALL CallLuaFunction(void *ClientData);
.cpp
void TW_CALL CDebugHelper::CallLuaFunction(void *ClientData)
{
	const char *l_Script=(const char *)ClientData;
	UABEngine->GetScriptManager()->RunCode(l_Script);
}

vooid CDebugHelper::ResetButtons()
{
	m_LuaParameter.clear();
}

void CDebugHelper::RegisterLUAButton(const std::string &ButtonName, const std::string &Script)
{
	m_LuaParameter.push_back(Script);
l_AntweakBar->RegisterButton("Light 01", &CDebugHelper::CallLuaFunction, (void *)m_LuaParameter[m_LuaParameter.size()-1].c_str());
}

void CDebugHelper::RegisterFloatParameter(const std::string &Name, CEmptyPointerClass *Data, const std::string &Description)
{
	void *l_Data=(void *)Data;
	TWAddVarRW(Bar, Name.c_str(), TW_TYPE_FLOAT, l_Data, Description.c_str());
}