//-----------------------------------------------------------------------
// Quaternion inline
/// Definiciones de funciones inline de la clase 'Quaternion'
/// Este fichero es realmente parte de la cabecera 'Quaternion.h' 
//-----------------------------------------------------------------------

template<typename T>
inline Quaternion<T>::Quaternion()
{
}

template<typename T>
inline Quaternion<T>::Quaternion(T tx, T ty, T tz, T tw)
: x(tx)
, y(ty)
, z(tz)
, w(tw)
{
}

template<typename T>
inline Quaternion<T>::Quaternion(const Quaternion<T>& otro)
: x(otro.x)
, y(otro.y)
, z(otro.z)
, w(otro.w)
{
}

/*
template<typename T>
inline Quaternion<T>::Quaternion(T Yaw, T Pitch, T Roll) //In radians
{
	T h = Yaw * 180 / FLOAT_PI_VALUE; //Radians to Euler Angles
		
	T p = Pitch * 180 / FLOAT_PI_VALUE;
		
	T b = Roll * 180 / FLOAT_PI_VALUE;

	T l_X = cos(h/2)*cos(p/2)*cos(b/2) + sin(h/2)*sin(p/2)*sin(b/2);
		
	T l_Y = cos(h/2)*sin(p/2)*cos(b/2) + sin(h/2)*cos(p/2)*sin(b/2);
		
	T l_Z = sin(h/2)*cos(p/2)*cos(b/2) - cos(h/2)*sin(p/2)*sin(b/2);

	T l_W = cos(h/2)*cos(p/2)*sin(b/2) - sin(h/2)*sin(p/2)*cos(b/2);

	x=l_X;
	y=l_Y;
	z=l_Z;
	w=l_W;
}*/

//template<typename T>
//inline Quaternion<T>::Quaternion(T heading, T attitude, T bank) //In radians
//{
//	 // Assuming the angles are in radians.
//    double c1 = cos(heading/2);
//    double s1 = sin(heading/2);
//    double c2 = cos(attitude/2);
//    double s2 = sin(attitude/2);
//    double c3 = cos(bank/2);
//    double s3 = sin(bank/2);
//
//  	x = c1*c2*s3 + s1*s2*c3;
//	y = s1*c2*c3 + c1*s2*s3;
//	z = c1*s2*c3 - s1*c2*s3;
//	w = c1*c2*c3 - s1*s2*s3;
//}

template<typename T>
inline Quaternion<T>::Quaternion(T yaw, T pitch, T roll) //In radians
{
	float num = roll * 0.5f;
	float num2 = (float)sin((double)num);
	float num3 = (float)cos((double)num);
	float num4 = pitch * 0.5f;
	float num5 = (float)sin((double)num4);
	float num6 = (float)cos((double)num4);
	float num7 = yaw * 0.5f;
	float num8 = (float)sin((double)num7);
	float num9 = (float)cos((double)num7);
	x = num9 * num5 * num3 + num8 * num6 * num2;
	y = num8 * num6 * num3 - num9 * num5 * num2;
	z = num9 * num6 * num2 - num8 * num5 * num3;
	w = num9 * num6 * num3 + num8 * num5 * num2;
}

template<typename T>
inline const Vector3<T> Quaternion<T>::GetRadians()
{
	float p, h, b;
	
	// Extract sin(pitch)
	float sp = -2.0f * (y*z - w*x);
	// Check for Gimbal lock, giving slight tolerance for numerical imprecision
	if (fabs(sp) > 0.9999f) {
		// Looking straight up or down
		p = 1.570796f * sp; // pi/2
		// Compute heading, slam bank to zero
		h = atan2(-x*z + w*y, 0.5f - y*y - z*z);
		b = 0.0f;
	}
	else {
		// Compute angles
		p = asin(sp);
		h = atan2(x*z + w*y, 0.5f - x*x - y*y);
		b = atan2(x*y + w*z, 0.5f - x*x - z*z);
	}

	return Vector3<T>(h, p, b);
}

//template<typename T>
//inline const Vector3<T> Quaternion<T>::GetRadians()
//{
//	float p, h, b;
//
//	float sp = (-2.0f) * (y*z + w*x);
//	// Check for Gimbal lock, giving slight tolerance for numerical imprecision
//	if (fabs(sp) > 0.9999f) {
//		// Looking straight up or down
//		p = 1.570796f * sp; // pi/2
//		// Compute heading, slam bank to zero
//		h = atan2(-x*z - w*y, 0.5f - y*y - z*z);
//		b = 0.0f;
//	}
//	else {
//		// Compute angles
//		p = asin(sp);
//		h = atan2(x*z - w*y, 0.5f - x*x - y*y);
//		b = atan2(x*y - w*z, 0.5f - x*x - z*z);
//	}
//
//	return Vector3<T>(h, p, b);
//}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: suma de quaternions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*template<typename T>
inline Quaternion<T> Quaternion<T>::operator - (const Quaternion<T>& otro) const
{   
  return (Quaternion<T>(x,
                     y,
                     z,
					 w));
}*/