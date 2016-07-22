class 'CCinematicsActionManager'

function CCinematicsActionManager:__init()
	self.m_Actions={}
	self.m_MaxTime=0
	self.m_CurrentTime=0.0
	self.m_CurrentAction=1
	self.m_CurrentExecutedAction=1
end

function CCinematicsActionManager:Remove(i)
	table.remove(self.m_Actions,i)
end

function CCinematicsActionManager:Update(ElapsedTime)
	self.m_CurrentTime=self.m_CurrentTime+ElapsedTime
	local l_CurrentActionId=self.m_CurrentAction
	
	if l_CurrentActionId>#(self.m_Actions) then
		return
	end
	
	-- g_LogManager:Log("1. "..self.m_Actions[self.m_CurrentExecutedAction]:GetTime())
	-- g_LogManager:Log("2. "..self.m_CurrentTime)
	-- g_LogManager:Log("3. "..self.m_CurrentExecutedAction)
	-- g_LogManager:Log("4. "..#self.m_Actions)
	
	while self.m_CurrentExecutedAction<=(#self.m_Actions) and
	self.m_Actions[self.m_CurrentExecutedAction]:GetTime()<=self.m_CurrentTime do
		self.m_Actions[self.m_CurrentExecutedAction]:Execute()
		self.m_CurrentExecutedAction=self.m_CurrentExecutedAction+1
	end

	for i=1, (#self.m_Actions) do
	
		local l_Time = self.m_Actions[i]:GetTime()
		local l_Acti = self.m_Actions[i]:IsActive()
		local l_CurrentT = self.m_CurrentTime
		if((l_Acti==false) and (l_Time<=l_CurrentT)) then
			g_LogManager:Log("Activamos "..i.." "..self.m_Actions[i]:GetTime().." "..self.m_CurrentTime)
			self.m_Actions[i]:SetActive(true)
		end
	
		if self.m_Actions[i]:IsActive() or self.m_Actions[i]:MustUpdate() then
			
			self.m_Actions[i]:Update(ElapsedTime)
		
			if(self.m_CurrentTime>(self.m_Actions[i]:GetTime()+self.m_Actions[i]:GetDuration())) then
				self:Remove(i)
			end
			
		end
	end
end

function CCinematicsActionManager:LoadXML(Filename)
	self.m_MaxTime=0
	self.m_CurrentTime=0
	self.m_Actions={}
	self.m_CurrentAction=1
	self.m_CurrentExecutedAction=1
	local l_XMLTreeNode=CXMLTreeNode()
	local l_Loaded=l_XMLTreeNode:LoadFile(Filename)
	
	if l_Loaded then
		for i=0, l_XMLTreeNode:GetNumChildren() do
			local l_Element=l_XMLTreeNode:GetChild(i)
			local l_ElemName=l_Element:GetName()
			
			if l_ElemName=="on" or l_ElemName=="after_last_action" then
				local l_ActionName=l_Element:GetPszProperty("action", "", false)
				local l_Action=nil
				
				if l_ActionName=="set_camera" then
					l_Action=CCinematicsActionSetCamera(l_Element)
				elseif l_ActionName=="add_camera_yaw" then
					l_Action=CCinematicsActionAddCameraYaw(l_Element)
				elseif l_ActionName=="add_camera_pitch" then
					l_Action=CCinematicsActionAddCameraPitch(l_Element)	
				elseif l_ActionName=="play_cinematic" then
					l_Action=CCinematicsActionPlayCinematic(l_Element)	
					
				elseif l_ActionName=="blend_cycle" then
					l_Action=CCinematicsActionBlendCycle(l_Element)
				elseif l_ActionName=="clear_cycle" then
					l_Action=CCinematicsActionClearCycle(l_Element)
				elseif l_ActionName=="execute_action" then
					l_Action=CCinematicsActionExecuteAction(l_Element)
						
				elseif l_ActionName=="add_yaw" then
					l_Action=CCinematicsActionAddYaw(l_Element)
				elseif l_ActionName=="add_pitch" then
					l_Action=CCinematicsActionAddPitch(l_Element)	
				elseif l_ActionName=="add_roll" then
					l_Action=CCinematicsActionAddRoll(l_Element)
				elseif l_ActionName=="set_yaw_pitch_roll" then
					l_Action=CCinematicsActionAddYawPitchRoll(l_Element)
				elseif l_ActionName=="set_position" then
					l_Action=CCinematicsActionSetPosition(l_Element)	
					
				elseif l_ActionName=="draw_slide" then
					l_Action=CCinematicsActionDrawSlide(l_Element)
				else 
					g_LogManager:Log("Can't create cinematic action "..l_ActionName..". It doesn't exists.")
				end
				
				if l_Action~=nil then
					if l_ElemName=="after_last_action" then
						local l_LastActionTime=0
						local l_LastActionDuration=0
						if (#self.m_Actions)>0 then
							l_LastActionTime=self.m_Actions[(#self.m_Actions)]:GetTime()
							l_LastActionDuration=self.m_Actions[(#self.m_Actions)]:GetDuration()
						end
						local l_NewTime = l_Action:GetTime()+l_LastActionTime+l_LastActionDuration
						l_Action:SetTime(l_NewTime)
						self.m_MaxTime=l_Action:GetTime()+l_LastActionTime+l_LastActionDuration
					else
						l_Action:SetTime(l_Action:GetTime())
					end
					table.insert(self.m_Actions, l_Action)
					--sort actions by time
				end
			end 
		end
		g_LogManager:Log("Cinematic actions loaded")
	else
		g_LogManager:Log(Filename.." can't be loaded")
	end
end 