#include "TTODMathUtils.h"

//wchar_t* CTools::CharToWChar(const char* Text)
//{
//	/*size_t size = strlen(Text) + 1;
//	wchar_t* wa = new wchar_t[size];
//	mbstowcs(wa, Text, size);
//	return wa;*/
//	size_t size = strlen(Text) + 1;
//	wchar_t* portName = new wchar_t[size];
//	//wchar_t* portName = new wchar_t[size];
//	size_t outSize;
//	mbstowcs_s(&outSize, portName, size, Text, size - 1);
//	return portName;
//}

/*
Returns true when the objects reaches the YawAfterFacing value.
*/
bool CTTODMathUtils::FaceDirection(CLUAComponent self, CLuaGameObjectHandle LuaGameObjectHandle, float CurrentYaw, float YawBeforeFacing, float YawAfterFacing, float RotationAngle, float RotationDuration, float AngleMargin)
{
	if (RotationDuration == 0.0f)
		return false;

	float l_PI_2 = FLOAT_PI_VALUE / 2.0f;
	float l_DoublePi = FLOAT_PI_VALUE * 2.0f;

	float l_ThreeQuarter = FLOAT_PI_VALUE + l_PI_2;
	float l_Difference = YawAfterFacing - CurrentYaw;

	if ((CurrentYaw > l_ThreeQuarter) && (YawAfterFacing < l_PI_2))
	{
		l_Difference = l_DoublePi - CurrentYaw + YawAfterFacing;
	}
	else
	{
		l_Difference = abs(abs(CurrentYaw) - abs(YawAfterFacing));
	}

	if (l_Difference > AngleMargin)
	{
		float l_Angle = (self.GetTimer()*RotationAngle) / RotationDuration;
		float l_NewYaw = GetFixedAngle(YawBeforeFacing + l_Angle);

		if (l_NewYaw<0.0 && YawAfterFacing < 0.0f)
		{
			if (RotationAngle<0.0 && l_NewYaw < YawAfterFacing)
			{
				LuaGameObjectHandle.SetYaw(YawAfterFacing);
				return true;
			}
			else if(RotationAngle>0.0 && l_NewYaw>YawAfterFacing)
			{
				LuaGameObjectHandle.SetYaw(YawAfterFacing);
				return true;
			}
		}
		else if(l_NewYaw > 0.0 && YawAfterFacing>0.0)
		{
			if (RotationAngle<0.0 && l_NewYaw < YawAfterFacing)
			{
				LuaGameObjectHandle.SetYaw(YawAfterFacing);
				return true;
			}
			else if(RotationAngle > 0.0 && l_NewYaw>YawAfterFacing)
			{
				LuaGameObjectHandle.SetYaw(YawAfterFacing);
				return true;
			}
		}
		LuaGameObjectHandle.SetYaw(l_NewYaw);
	}
	else
	{
		LuaGameObjectHandle.SetYaw(YawAfterFacing);
		return true;
	}
	return false;
}

float CTTODMathUtils::GetFixedAngle(float Angle)
{
	if (Angle > (2 * FLOAT_PI_VALUE))
	{
		return (Angle - (2 * FLOAT_PI_VALUE));
	}
	else if (Angle < (FLOAT_PI_VALUE*(-2.0f)))
	{
		return (Angle + (2 * FLOAT_PI_VALUE));
	}
	else { return Angle; }
}

float  CTTODMathUtils::AngleBetweenVectors(const Vect3f &VectorOne, const Vect3f &VectorTwo)
{
	float l_Value = 0.0f;
	float l_Numerator = VectorOne * VectorTwo;
	
	float l_Denominator = sqrt((pow(VectorOne.x, 2)) + (pow(VectorOne.y, 2)) + (pow(VectorOne.z, 2))) *sqrt((pow(VectorOne.x, 2)) + (pow(VectorOne.y, 2)) + (pow(VectorOne.z, 2)));
	l_Value = l_Numerator / l_Denominator;
	if (l_Value > 1.0f)
		l_Value = 1.0f;
	else if (l_Value < -1.0f)
		l_Value = -1.0f;

	float l_Angle = acos(l_Value);
	Vect3f l_CrossProduct = VectorOne^VectorTwo;
	if (l_CrossProduct.y > 0.0f)
	{
		l_Angle = l_Angle*(-1.0f);
	}
	return l_Angle;
}

float CTTODMathUtils::GetAngleToFacePoint(const Vect3f &Forward, const Vect3f &OriginPoint, const Vect3f &PointToFace)
{
	float l_NewAngle = 0.0f;
	Vect3f l_VectorToDestinyPoint = PointToFace - OriginPoint;
	l_VectorToDestinyPoint.Normalize();
	return AngleBetweenVectors(l_VectorToDestinyPoint, Forward);
}

bool CTTODMathUtils::PointInsideCircle(const Vect3f &Point, const Vect3f &CircunferenceCenter, float Radius)
{
	Vect3f l_Vector = Point - CircunferenceCenter;
	float l_Distance = l_Vector.Length();

	if (l_Distance < Radius)
		return true;

	return false;
}

float CTTODMathUtils::CalculateNewAngle(float Angle, float CurrentYaw, float Velocity, float ElapsedTime)
{
	float l_Result = 0.0f;
	if (Angle < 0.0f)
	{
		if ((Velocity*(-1)*ElapsedTime) < Angle)
		{
			l_Result = CurrentYaw + (Angle*ElapsedTime);
		}
		else { l_Result = CurrentYaw + (Velocity*(-1)*ElapsedTime); }
	}
	else
	{
		if ((Velocity*ElapsedTime) > Angle)
		{
			l_Result = CurrentYaw + (Angle*ElapsedTime);
		}
		else { l_Result = CurrentYaw + (Velocity*ElapsedTime); }
	}
	return l_Result;
}

float CTTODMathUtils::LogBaseBofX(float X, float Base)
{
	return log10(X) / log10(Base);
}