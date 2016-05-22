#ifndef _TTODMATHUTILS_H
#define _TTODMATHUTILS_H

#include <string>
#include "Math\Vector3.h"

class CTTODMathUtils
{  
private:
	static float AngleBetweenVectors(const Vect3f &VectorOne, const Vect3f &VectorTwo);

public:
	//static wchar_t* CharToWChar(const char* Text);
	static float GetAngleToFacePoint(const Vect3f &Forward, const Vect3f &OriginPoint, const Vect3f &PointToFace);
	static bool PointInsideCircle(const Vect3f &Point, const Vect3f &CircunferenceCenter, float Radius);
};

#endif