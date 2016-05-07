--Idle_State
function OnEnter_Idle_BruteEnemy(RenderableObject, ElapsedTime)
	
end

function OnUpdate_Idle_BruteEnemy(RenderableObject, ElapsedTime)
		
	-- if RenderableObject:IsPlayerInsideVisionRange(GetPlayer():GetPosition()) then   
		-- g_AnimatorController:SetBool("IsPlayerInsideVisionRange", true)  
	-- else
		-- l_ElapsedTime =  l_ElapsedTime + ElapsedTime
	-- end
	
	-- if l_ElapsedTime > 1.5 then
		-- g_AnimatorController:SetBool("DelayToPatrol", true)
	-- end
	
end

function OnExit_Idle_BruteEnemy(RenderableObject, ElapsedTime)
	--l_ElapsedTime = 0.0
end

--Charge_State
function OnEnter_Charge_BruteEnemy(RenderableObject, ElapsedTime)
	--l_PosToCharge = GetPlayer():GetPosition()
end

function OnUpdate_Charge_BruteEnemy(RenderableObject, ElapsedTime)
	
	--if RenderableObject:PointInsideCircle(RenderableObject:GetPosition(), l_PosToCharge, 0.3) then
	--	g_AnimatorController:SetTrigger("GoBackToIdle")
	--end
	--RenderableObject:Charge(l_PosToCharge,ElapsedTime)
end

function OnExit_Charge_BruteEnemy(RenderableObject, ElapsedTime)
	--l_ElapsedTime = 0.0
	--g_AnimatorController:SetBool("IsPlayerInsideVisionRange", false)
end

--Patrol_State
function OnEnter_Patrol_BruteEnemy(RenderableObject, ElapsedTime)
	--local l_PatrolWayPoints = {1, 4, 9}
	--RenderableObject:FollowTriangleWayPoints(ElapsedTime, l_PatrolWayPoints) 
end

function OnUpdate_Patrol_BruteEnemy(RenderableObject, ElapsedTime)
	
	--if IsPlayerInsideVisionRange(GetPlayer():GetPosition()) then 
	--	g_AnimatorController:SetBool("IsPlayerInsideVisionRange", true)  
	--end
end

function OnExit_Patrol_BruteEnemy(RenderableObject, ElapsedTime)
	--l_ElapsedTime = 0.0
end

