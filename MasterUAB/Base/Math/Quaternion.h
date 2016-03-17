//-----------------------------------------------------------------------
// Quat class
// This class defines a Quaternion
//-----------------------------------------------------------------------
#ifndef __QUAT_H__
#define __QUAT_H__

template<typename T>
class Quaternion
{
public:
	T x, y, z, w;
	
	inline Quaternion();
	inline Quaternion(T tx, T ty, T tz, T tw);
	inline Quaternion(const Quaternion<T>& otro);
	inline Quaternion(T heading, T attitude, T bank); //Radians
	//inline Quaternion(float Yaw, float Pitch, float Roll); //Radians
	//inline Quaternion(double Yaw, double Pitch, double Roll); //Radians

	inline const Vector3<T> GetRadians();
	// Operadores aritméticos   
  //inline Quaternion<T>  operator - (const Quaternion<T>& otro) const;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fichero con las definiciones de las funciones inline
#include "Quaternion.inl"

typedef Quaternion<float> Quatf;
typedef Quaternion<double> Quatd;

#endif
