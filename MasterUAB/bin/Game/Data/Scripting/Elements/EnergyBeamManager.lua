class 'CEnergyBeamManager' (CLUAComponent)
function CEnergyBeamManager:__init()
CLUAComponent.__init(self,"EnergyBeamManagerScript")	
	self.m_Emitters = {}
end

function CEnergyBeamManager:AddEmitter(Emitter)
	table.insert(self.m_Emitters,Emitter)
end

function CEnergyBeamManager:Update(ElapsedTime)
end