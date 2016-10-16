
-------------------- COMBAT_IDLE -------------------- 
function OnEnter_CombatIdle_Player(Player)
end

function OnUpdate_CombatIdle_Player(Player, ElapsedTime)
	local l_Lgo = Player:GetLuaGameObject()
	Player:CheckForSurroundingEnemies()
end

function OnExit_CombatIdle_Player(Player)
end

-------------------- IDLE -------------------- 
function OnEnter_Idle_Player(PlayerComponent)
end

function OnUpdate_Idle_Player(Player, ElapsedTime)
	local l_Lgo = Player:GetLuaGameObject()
	Player:CheckForSurroundingEnemies()
end

function OnExit_Idle_Player(Player)
end

-------------------- WALK -------------------- 
function OnEnter_Walk_Player(Player)
	-- g_Engine:GetRenderManager():GetContextManager():SetFullScreen(false, 1920, 1080)
end

function OnUpdate_Walk_Player(Player, ElapsedTime)
	
	local l_NewForward = Vect3f(0.0,0.0,0.0)
	
	if Player.m_Forward then
		if Player.m_Right then
			l_NewForward = Player.m_LuaGameObject:GetForward() + Player.m_LuaGameObject:GetRight()
		elseif Player.m_Left then
			l_NewForward = Player.m_LuaGameObject:GetForward() +(Player.m_LuaGameObject:GetRight()*(-1.0))
		else
			l_NewForward = Player.m_LuaGameObject:GetForward()	
		end	
	elseif Player.m_Backwards then
		if Player.m_Right then
			l_NewForward = (Player.m_LuaGameObject:GetForward()*(-1.0)) + Player.m_LuaGameObject:GetRight()
		elseif Player.m_Left then
			l_NewForward = (Player.m_LuaGameObject:GetForward()*(-1.0)) +(Player.m_LuaGameObject:GetRight()*(-1.0))
		else
			l_NewForward = Player.m_LuaGameObject:GetForward()*(-1.0)
		end	
	elseif Player.m_Right then
		l_NewForward = Player.m_LuaGameObject:GetRight()
	elseif Player.m_Left then 
		l_NewForward = Player.m_LuaGameObject:GetRight()*(-1.0) 	
	end
	
	l_NewForward:Normalize(1)
	
	local l_CameraForward = g_CameraControllerManager:GetCurrentCameraController():GetForward():Normalize(1)
	Player:FaceDirectionWhileWalking(l_NewForward, l_CameraForward, ElapsedTime)
	
	local l_Lgo = Player:GetLuaGameObject()
	Player:CheckForSurroundingEnemies()
end

function OnExit_Walk_Player(Player)
end

-------------------- ATTACK -------------------- 
function OnEnter_Attack_Player(Player)
	Player:SetAttacking(true)
	Player:SetAttackFinished(false)
	Player:Lock()
	
	local l_DirectionToFace = g_CameraControllerManager:GetCurrentCameraController():GetForward():Normalize(1)
	local l_ForwardBeforeFacing = Player:GetLuaGameObject():GetForward():Normalize(1)
	
	-- g_LogManager:Log("DirToFace:")
	-- g_LogManager:Log(l_DirectionToFace)
	-- g_LogManager:Log("ForwardBeforeFacing:")
	-- g_LogManager:Log(l_ForwardBeforeFacing)
	
	Player:SetAttackFacingValues(l_ForwardBeforeFacing,l_DirectionToFace)
	
	-- g_Engine:GetRenderManager():GetContextManager():SetFullScreen(true, 1920, 1080)
	
	Player:GetLuaGameObject():PlayEvent("PlayerBasicAttack")
	
	-- Player:TakeDamage(nil,400)
end

function OnUpdate_Attack_Player(Player, ElapsedTime)
	Player:CalculateAttackDirection(ElapsedTime)
	local l_PlayerForward = Player.m_LuaGameObject:GetForward()    
	Player.m_Velocity = Player.m_Velocity + (l_PlayerForward*Player.m_AttackDisplacement)
	Player:CheckForSurroundingEnemies()
end

function OnExit_Attack_Player(Player)
	Player.m_FacingFinished = false
	Player:SetAttacking(false)
	Player:SetAttackFinished(true)
	Player:Unlock()
end

-------------------- INTERACT -------------------- 
function OnEnter_Interact_Player(Player)
	Player:SetInteracting(true)
	Player:Lock()
end

function OnUpdate_Interact_Player(Player, ElapsedTime)
	Player:CheckForSurroundingEnemies() 
	local l_Forward = Player.m_LuaGameObject:GetForward():Normalize(1)
	
	if (not Player.m_FacingFinished) then
		Player:FaceDirection(ElapsedTime)
	end
end

function OnExit_Interact_Player(Player)
	Player.m_FacingFinished = false
	Player:SetInteracting(false)
	Player:Unlock()
end

-------------------- BLOCK -------------------- 
function OnEnter_Block_Player(Player)
	Player:SetBlocking(true)
	Player:Lock()
	local l_DirectionToFace = g_CameraControllerManager:GetCurrentCameraController():GetForward():Normalize(1)
	local l_ForwardBeforeFacing = Player:GetLuaGameObject():GetForward():Normalize(1)
	Player:SetAttackFacingValues(l_ForwardBeforeFacing,l_DirectionToFace)
end

function OnUpdate_Block_Player(Player, ElapsedTime)
	Player:CalculateAttackDirection(ElapsedTime)
	Player:CheckForSurroundingEnemies()
end

function OnExit_Block_Player(Player)
	Player.m_FacingFinished = false
	Player:SetBlocking(false)
	Player:Unlock()
end

-------------------- TOSSED -------------------- 
function OnEnter_Tossed_Player(PlayerComponent)
	PlayerComponent:IsBeingTossed(true)
	Player:CheckForSurroundingEnemies()
end

function OnUpdate_Tossed_Player(Player, ElapsedTime)
end

function OnExit_Tossed_Player(Player)
	PlayerComponent:IsBeingTossed(false)
end


