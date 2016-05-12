class 'CDoorComponent' (CLUAComponent)
function CDoorComponent:__init(CRenderableObject)
CLUAComponent.__init(self,"door")	
	self.m_RObject = CRenderableObject
end

function CDoorComponent:Initialize(XMLTreeNode)
	-- self.m_Function = XMLTreeNode:GetPszProperty("function", "", false)
	g_LogManager:Log("Door "..self.m_RObject:GetName().." created...")
end

function CDoorComponent:OnTriggerEnter()
	
end


