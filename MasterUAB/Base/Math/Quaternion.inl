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

template<typename T>
inline Quaternion<T>::Quaternion(T heading, T attitude, T bank) //In radians
{
	 // Assuming the angles are in radians.
    double c1 = cos(heading/2);
    double s1 = sin(heading/2);
    double c2 = cos(attitude/2);
    double s2 = sin(attitude/2);
    double c3 = cos(bank/2);
    double s3 = sin(bank/2);

  	x = c1*c2*s3 + s1*s2*c3;
	y = s1*c2*c3 + c1*s2*s3;
	z = c1*s2*c3 - s1*c2*s3;
	w = c1*c2*c3 - s1*s2*s3;
}

template<typename T>
inline const Vector3<T> Quaternion<T>::GetRadians()
{
	T test = x*y + z*w;
	T heading, attitude, bank;

	if (test > 0.499) 
	{ // singularity at north pole
		heading = 2 * atan2(x,w);
		attitude = FLOAT_PI_VALUE/2;
		bank = 0;
		return Vector3<T>(heading,attitude,bank);
	}

	if (test < -0.499) 
	{ // singularity at south pole
		heading = -2 * atan2(x,w);
		attitude = - FLOAT_PI_VALUE/2;
		bank = 0;
		return Vector3<T>(heading,attitude,bank);
	}

	T x2 = x*x;
    T y2 = y*y;
    T z2 = z*z;

	heading = atan2(2*y*w - 2*x*z , 1 - 2*(y2) - 2*(z2));
	attitude = asin(2*x*y + 2*z*w); 
	bank = atan2(2*x*w - 2*y*z , 1 - 2*(x2) - 2*(z2));
	
	return Vector3<T>(heading,attitude,bank);
}



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