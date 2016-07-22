function ExponentialDisplacement(Displacement,Constant,Base,Exponent)
	local l_Exp = Displacement*(Base^Exponent)
	return l_Exp*Constant
end

--Returns if a 3D Point is inside a circunference arround a point.
function IsPointInsideCircunferenceLUA(CircunferenceCenter,Point,Radius)
	local l_Vector = Point - CircunferenceCenter
	local l_Distance = l_Vector:Length()

	if l_Distance < Radius then
		return true
	end

	return false
end

--Returns the angle that a object needs to rotate to face a 3D point
function CalculateAngleToFacePoint(Forward, OriginPoint, PointToFace)

	local l_NewAngle = 0.0
	local l_VectorToDestinyPoint = PointToFace - OriginPoint
	l_VectorToDestinyPoint:Normalize(1.0)
	l_NewAngle = AngleBetweenVectors(l_VectorToDestinyPoint, Forward)
	return l_NewAngle
end

function AngleBetweenVectors(VectorOne, VectorTwo)
	local l_Value = 0.0
	local l_Numerator = VectorOne * VectorTwo
	local l_Denominator = (math.sqrt((VectorOne.x^2) + (VectorOne.y^2) + (VectorOne.z^2)))*(math.sqrt((VectorTwo.x^2) + (VectorTwo.y^2) + (VectorTwo.z^2)))
	l_Value = l_Numerator/l_Denominator
	if l_Value>1.0 then l_Value=1.0 end
	local l_Angle = math.acos(l_Value)
	local l_CrossProduct = VectorOne:cross(VectorTwo)
	if l_CrossProduct.y < 0.0 then
		l_Angle = l_Angle*(-1.0)
	end 
	return l_Angle
end