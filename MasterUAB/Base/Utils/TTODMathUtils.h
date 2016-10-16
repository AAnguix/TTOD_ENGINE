#ifndef _TTODMATHUTILS_H
#define _TTODMATHUTILS_H

#include <string>
#include "Math\Vector3.h"

#include "GameObject\LuaGameObjectHandle.h"
#include "Components\LuaComponent.h"

class CTTODMathUtils
{  

public:
	//static wchar_t* CharToWChar(const char* Text);
	static float AngleBetweenVectors(const Vect3f &VectorOne, const Vect3f &VectorTwo);
	static float GetAngleToFacePoint(const Vect3f &Forward, const Vect3f &OriginPoint, const Vect3f &PointToFace);
	static bool PointInsideCircle(const Vect3f &Point, const Vect3f &CircunferenceCenter, float Radius);
	static float CalculateNewAngle(float Angle, float CurrentYaw, float Velocity, float ElapsedTime);

	static float LogBaseBofX(float X, float Base);

	static float GetFixedAngle(float Angle);


	//Gameplay

	static bool FaceDirection(CLUAComponent self, CLuaGameObjectHandle LuaGameObjectHandle, float CurrentYaw, float YawBeforeFacing, float YawAfterFacing, float RotationAngle, float RotationDuration, float AngleMargin);
};

#endif