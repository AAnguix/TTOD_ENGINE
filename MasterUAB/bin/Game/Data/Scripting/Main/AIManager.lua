class 'CAIManager'
function CAIManager:__init()
	self.m_AStar = CAStar()
end

function CAIManager:Destroy()
end

function CAIManager:SearchAStarPath(PointA,PointB,Log)
	return self.m_AStar:SearchPath(PointA,PointB)
	-- if Log then
	
	-- end
end

function CAIManager:LoadLevel(Level)
	self.m_AStar:LoadNodes("Data/Level"..Level.."/astar_nodes.xml")
end
