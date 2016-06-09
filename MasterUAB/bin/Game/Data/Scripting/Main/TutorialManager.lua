class 'CTutorialManager' (CLUAComponent)
function CTutorialManager:__init(CRenderableObject)
CLUAComponent.__init(self,"TutorialManagerScript")	
	self.m_RObject = CRenderableObject
	self.m_ForwardPressed = false
	self.m_BackwardsPressed = false
	self.m_RightPressed = false
	self.m_LeftPressed = false
	self.m_MouseLeftPressed = false
	self.m_MouseRightPressed = false
	
	self.m_Delay = 2.0
	self.m_StepOneDone = false
	self.m_StepTwoDone = false
	
	g_GUIManager:AddImage("tutorial_first_step_image","tutorial_first_step_image")
	
end

function CTutorialManager:Update(ElapsedTime)
	if self.m_StepOneDone == false
		self:CheckStepOne()
	else if self.m_StepOneDone == false
		self:CheckStepTwo()
	else self:TutorialFinished()
	end	
end

function CTutorialManager:CheckStepOne()
	--Show gui image explaining
	if (self.m_ForwardPressed~=false and
	self.m_BackwardsPressed~=false and
	self.m_RightPressed~=false and
	self.m_LeftPressed~=false and)
	then
		self:AddTime()
		if(self:GetTimer() > self.m_Delay)
			self.m_StepOneDone = true
			self.m_RObject:GetAudioSource():PlayEvent("step_one_done")
		end
	end
end

function CTutorialManager:CheckStepTwo()

end

function CTutorialManager:TutorialFinished()

end

