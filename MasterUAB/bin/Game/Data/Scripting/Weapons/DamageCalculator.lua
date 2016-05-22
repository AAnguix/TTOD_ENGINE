class 'CDamageCalculator' 
function CDamageCalculator:__init()
	self.m_DamageReductionPct = {}
	self.m_Filename  = ""
end

function CDamageCalculator:CalculateDamage(ArmorName,WeaponName, Damage)
	local l_DamageReductionPercentage = self:GetPercentage(ArmorName,WeaponName)
	local l_Damage = Damage - (Damage*l_DamageReductionPercentage)
	return l_Damage
end

function CDamageCalculator:LoadXML(Filename)
	self.m_Filename = Filename
	local l_XMLTreeNode=CXMLTreeNode()
	local l_Loaded=l_XMLTreeNode:LoadFile(Filename)
	if l_Loaded then
		for i=0, l_XMLTreeNode:GetNumChildren() do
			local l_Element=l_XMLTreeNode:GetChild(i)
			local l_ElemName=l_Element:GetName()
			
			if l_ElemName=="damage_reduction_pair" then
				local l_Armor = l_Element:GetPszProperty("armor", "", false)
				local l_Weapon = l_Element:GetPszProperty("weapon", "", false)
				local l_Percentage = l_Element:GetFloatProperty("percentage", 0.0, false)
				self:AddValue(l_Armor,l_Weapon,l_Percentage)
			end
		end
		g_LogManager:Log("Damage reduction percentages loaded.")
	else
		print("File '"..Filename.."'not correctly loaded")
	end
	--g_LogManager:Log("Game entities loaded...")
end 

function CDamageCalculator:AddValue(Armor,Weapon,Percentage)
	if self:PairExists() == false then
		local l_DamageReductionPair = CDamageReductionPair(Armor,Weapon,Percentage)
		local l_Key = ""..Armor.."_"..Weapon
		self.m_DamageReductionPct[l_Key] = l_DamageReductionPair
	end
end

function CDamageCalculator:GetPercentage(Armor,Weapon)
	local l_Key = ""..Armor.."_"..Weapon
	return self.m_DamageReductionPct[l_Key]
end

function CDamageCalculator:PairExists(Armor, Weapon)
	for i=1, (#self.m_DamageReductionPct) do
		if self.m_DamageReductionPct[i].m_Armor == Armor and self.m_DamageReductionPct[i].m_Weapon == Weapon then
			return true
		end
	end
	return false
end

class 'CDamageReductionPair'
function CDamageReductionPair:__init(Armor,Weapon,Percentage)
	self.m_Armor = Armor
	self.m_Weapon = Weapon
	self.m_Percentage = Percentage
end
