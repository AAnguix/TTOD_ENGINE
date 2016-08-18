#ifndef _PERLINNOISE_H
#define _PERLINNOISE_H

#include <vector>

class CPerlinNoise 
{
	private:
		static void normalize2(float v[3]);
		static void normalize3(float v[3]);
		static void init(void);
		static double noise1(double arg);
		static float noise2(float vec[2]);
		static float noise3(float vec[3]);

	public:
		
		static float noise2_parser(float a, float b) 
		{
			float f[2];
			f[0] = a;
			f[1] = b;
			return noise2(f);
		}
		static float noise3_parser(float a, float b, float c)
		{
			float f[3];
			f[0] = a;
			f[1] = b;
			f[2] = c;
			return noise2(f);
		}
};

#endif