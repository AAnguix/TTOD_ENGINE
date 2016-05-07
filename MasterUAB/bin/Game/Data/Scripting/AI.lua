function InitializeAI()
	LoadAStarPoints()
end

function LoadAStarPoints()
	g_EnemyWayPoints[0] = Vect3f(0.0,0.0,0.0)
	g_EnemyWayPoints[1] = Vect3f(2.0,0.0,1.0)
	g_EnemyWayPoints[2] = Vect3f(3.0,0.0,2.0)
	g_EnemyWayPoints[4] = Vect3f(4.0,0.0,4.0)
end

function PatrolWayPoints(Waypoints, ElapsedTime, Enemy, EnemySpeed, Radius)

	local l_EnemyPos = Enemy:GetPosition()
	local l_Vector = VectorBetweenPoints(l_EnemyPos,Waypoints[g_CurrentGoingPoint])
	local l_PlayerPos = GetPlayer():GetPosition()
	
	-- if PointInsideCircle(l_EnemyPos, l_PlayerPos, g_AttackRange) then    
		-- local a=0
	-- else
	if PointInsideCone(l_EnemyPos, l_PlayerPos, Enemy:GetForward(), g_VisionAngle) then
		local a=0
	else 
		if PointInsideCircle(l_EnemyPos, Waypoints[g_CurrentGoingPoint], Radius) == false then
			Enemy:SetPosition(l_EnemyPos+(l_Vector*ElapsedTime*EnemySpeed))
		else
			g_CurrentGoingPoint=g_CurrentGoingPoint+1
		end
		
		if g_CurrentGoingPoint >= g_MaxWayPoint then 
			g_CurrentGoingPoint = 0
		end
	end
end

-- function GoPoint(Enemy, Point, Vector, ElapsedTime, EnemySpeed)
	-- l_EnemyPos = Enemy:GetPosition()
	-- Enemy:SetPosition(l_EnemyPos+(Vector*ElapsedTime*EnemySpeed))
-- end

function PointInsideCircle(OriginPoint, DestinyPoint, Radius)
	l_Vector = DestinyPoint - OriginPoint
	l_Distance = l_Vector:Length()

	if l_Distance < Radius then
		return true
	end
		return false
end

function PointInsideCone(OriginPoint, DestinyPoint, ForwardVector, VisionAngle)
	local l_DiffVector = DestinyPoint - OriginPoint
	local l_Dot = l_DiffVector * ForwardVector
	local l_Angle=math.acos(l_Dot)
	if l_Angle <= VisionAngle then
		return true
	end
		return false
end

function VectorBetweenPoints(OriginPoint, DestinyPoint)
	l_Vector = DestinyPoint - OriginPoint
	--l_Distance = l_Vector:Length()
	l_Vector:Normalize(1)
	return l_Vector
end

