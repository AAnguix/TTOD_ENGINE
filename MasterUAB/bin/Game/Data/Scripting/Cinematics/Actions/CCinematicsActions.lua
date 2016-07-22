class 'CCinematicsAction'

function CCinematicsAction:__init(XMLTreeNode)
	self.m_Time=XMLTreeNode:GetFloatProperty("time", 0.0,false)
	self.m_Duration=XMLTreeNode:GetFloatProperty("duration", 0.0,false)
	self.m_Active = false
end

--The time that the animation starts
function CCinematicsAction:GetTime() return self.m_Time end
function CCinematicsAction:SetTime(Value) self.m_Time=Value end

function CCinematicsAction:GetDuration()
	return self.m_Duration
end

function CCinematicsAction:IsActive() return self.m_Active end
function CCinematicsAction:SetActive(Value) self.m_Active = Value end

function CCinematicsAction:Update(ElapsedTime)

end

function CCinematicsAction:MustUpdate()
	return false
end

--Raised the first time the animation is called
function CCinematicsAction:Execute()
end

dofile("./Data/Scripting/Cinematics/Actions/CCinematicsGUIActions.lua")
dofile("./Data/Scripting/Cinematics/Actions/CCinematicCameraActions.lua")

