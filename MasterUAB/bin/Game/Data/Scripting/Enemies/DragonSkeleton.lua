
function CDragonComponent:CreateSkeleton()
	
	if  #self.m_PhysicsHead > 0 then
		for i, value in ipairs(self.m_PhysicsHead) do
			if self.m_PhysicsHead[i].m_BoneType == "box" then
				self:CreateBoxTypeBone(self.m_PhysicsHead[i])
			elseif self.m_PhysicsHead[i].m_BoneType == "sphere" then
				self:CreateSphereTypeBone(self.m_PhysicsHead[i])
			end
		end	
	end	
	
	if  #self.m_PhysicsTail > 0 then
		for i, value in ipairs(self.m_PhysicsTail) do 
			if self.m_PhysicsTail[i].m_BoneType == "box" then
				self:CreateBoxTypeBone(self.m_PhysicsTail[i])
			elseif self.m_PhysicsTail[i].m_BoneType == "sphere" then
				self:CreateSphereTypeBone(self.m_PhysicsTail[i])
			end
		end
	end
	
	if  #self.m_PhysicsBody > 0 then
		for i, value in ipairs(self.m_PhysicsBody) do
			if self.m_PhysicsBody[i].m_BoneType == "box" then
				self:CreateBoxTypeBone(self.m_PhysicsBody[i])
			elseif self.m_PhysicsBody[i].m_BoneType == "sphere" then
				self:CreateSphereTypeBone(self.m_PhysicsBody[i])
			end
		end
	end	

end

function CDragonComponent:CreateBoxTypeBone(Bone)
	local l_bone = Bone
	local l_BoneSize = l_bone.m_BoneSize * 2
	local l_BoneWorldPos = l_bone:GetWorldPos()
	--local l_BoneRotation = l_bone:GetRotation()
	local l_BoneRotation = Quatf(-0.688528, -0.161031, 0.146242, -0.691818)	
	
	--local l_result = g_PhysXManager:CreateBoxTrigger(l_bone.m_BoneName, l_bone.m_BoneName, l_BoneSize ,l_bone.m_BoneName.."mat", "GROUP3",l_BoneWorldPos,l_BoneRotation, "kinematic")
	--CreateBoxLua(const std::string &ShapeName, const Vect3f &Size, const std::string MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, bool IsExclusive)
	g_PhysXManager:CreateBoxLua(l_bone.m_BoneName, l_BoneSize, l_bone.m_BoneName.."mat", 10.0, 20.0, 1.0, "", true)
	local l_result = g_PhysXManager:CreateDynamicActor(l_bone.m_BoneName, l_bone.m_BoneName, l_BoneWorldPos, l_BoneRotation, 5.0, true);
	if l_result then
		-- g_LogManager:Log(l_bone.m_BoneName.." BoxSkeleton Creado OK ") 
	else
		 g_LogManager:Log(l_bone.m_BoneName.." BoxSkeleton Creado Mal ")
	end
end

function CDragonComponent:CreateSphereTypeBone(Bone)
	
	local l_bone = Bone
	local l_BoneSize = l_bone.m_BoneSize
	local l_BoneWorldPos = l_bone:GetWorldPos()
	local l_BoneRotation = l_bone:GetRotation()		
	--local l_result = g_PhysXManager:CreateSphereTrigger(l_bone.m_BoneName, l_bone.m_BoneName, l_BoneSize ,l_bone.m_BoneName.."mat", "GROUP3",l_BoneWorldPos,l_BoneRotation, "kinematic")
	g_PhysXManager:CreateSphereLua(l_bone.m_BoneName, l_BoneSize, l_bone.m_BoneName.."mat", 10.0, 20.0, 1.0, "", true)
	local l_result = g_PhysXManager:CreateDynamicActor(l_bone.m_BoneName, l_bone.m_BoneName, l_BoneWorldPos, l_BoneRotation, 5.0, true);
	if l_result then
		 --g_LogManager:Log(l_bone.m_BoneName.." SphereSkeleton Creado OK ")
	else
		 g_LogManager:Log(l_bone.m_BoneName.." SphereSkeleton Creado Mal ")
	end
end	

function CDragonComponent:UpdatePxSkeleton()
	
	--if  #self.m_PhysicsTail > 0 then
		--for i, value in ipairs(self.m_PhysicsTail) do
			--if self.m_PhysicsTail[i].m_BoneType == "box" then
				--self.m_PhysicsTail[i]:SetTransform()
			--end
		--end
	--end 
	
end

function CDragonComponent:AddPxPiece(Index, Piece)
	
	if Index == "head" then
		--g_LogManager:Log("Agregando Dragon Piece: "..Piece.m_BoneName.." Tipo: "..Index)
		table.insert(self.m_PhysicsHead, Piece)
	elseif Index == "body" then
		--g_LogManager:Log("Agregando Dragon Piece: "..Piece.m_BoneName.." Tipo: "..Index)
		table.insert(self.m_PhysicsBody, Piece)
	elseif Index == "tail" then
		--g_LogManager:Log("Agregando Dragon Piece: "..Piece.m_BoneName.." Tipo: "..Index)
		table.insert(self.m_PhysicsTail, Piece)
	else
		g_LogManager:Log("Error al crear un hueso del dragon, indice no reconocido")
	end 
	--g_LogManager:Log("Dragon Piece: "..Piece.m_BoneName.." Agregada")
end

function CDragonComponent:GetBone(BoneName)
	
	if  #self.m_PhysicsTail > 0 then
		for i, value in ipairs(self.m_PhysicsTail) do
			if self.m_PhysicsTail[i].m_BoneName == BoneName then
				return self.m_PhysicsTail[i]
			end
		end
	elseif #self.m_PhysicsBody > 0 then
		for i, value in ipairs(self.m_PhysicsBody) do
			if self.m_PhysicsBody[i].m_BoneName == BoneName then
				return self.m_PhysicsBody[i]
			end
		end
	elseif #self.m_PhysicsHead > 0 then
		for i, value in ipairs(self.m_PhysicsHead) do
			if self.m_PhysicsHead[i].m_BoneName == BoneName then
				return self.m_PhysicsHead[i]
			end
		end
	else
		g_LogManager:Log("Metodo CDragonComponent:GetBone(BoneName) invocado pero no consiguio el hueso")
	end 
	
end