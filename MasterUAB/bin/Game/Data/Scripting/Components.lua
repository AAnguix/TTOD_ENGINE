function InitializeRenderableObjectsScripts()
	InitCharacterController()
	InitEnemys()
end

function InitCharacterController()
	local l_Character = GetPlayer()
	AddScriptedComponent(l_Character, "ScriptedCharacterController", "FnOnCreatePlayer", "FnOnDestroyPlayer", "FnOnUpdatePlayer","","")
end

function FnOnCreatePlayer(RenderableObject, ElapsedTime)
end

function FnOnDestroyPlayer(RenderableObject, ElapsedTime)
end

function FnOnUpdatePlayer(RenderableObject, ElapsedTime)
	CharacterController(ElapsedTime)
end

function InitEnemys()
	-- local ROManager = CEngine.GetSingleton():GetLayerManager():GetResource("solid")
	
	-- local l_Enemys=ROManager:GetResourcesVector()
	-- for l_Enemy in l_Enemys do
		-- AddScriptedComponent(l_Enemy, l_Enemy:GetName().."Script","FnOnCreateEnemy", "FnOnDestroyEnemy", "FnOnUpdateEnemy","","")
	-- end
end

-- function FnOnCreateEnemy(RenderableObject, ElapsedTime)
-- end

-- function FnOnDestroyEnemy(RenderableObject, ElapsedTime)
-- end

-- function FnOnUpdateEnemy(RenderableObject, ElapsedTime)
	-- EnemyController(ElapsedTime)
-- end

function AddScriptedComponent(RObject, ScriptComponentName, FnOnCreate, FnOnDestroy, FnOnUpdate, FnOnRender, FnOnRenderDebug)
	if RObject~=nil then
		local l_Component=RObject:GetComponentManager():GetResource(ScriptComponentName)
		if l_Component==nil then
			local l_Component=CScript.AddScript(ScriptComponentName, RObject, FnOnCreate, FnOnDestroy, FnOnUpdate, FnOnRender, FnOnRenderDebug)
			RObject:AddComponent(l_Component)
		end
	end
end
