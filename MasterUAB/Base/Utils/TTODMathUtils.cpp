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
	if (l_CrossProduct.y < 0.0f)
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