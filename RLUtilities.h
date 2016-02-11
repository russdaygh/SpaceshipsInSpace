#pragma once

namespace RustyLib
{
	namespace Utils
	{
		const static float PI = 3.14159265f;
		//Degrees to radians conversion scalar
		const static float PIover180 = PI/180;
		static inline float DegToRad(const float theta)
		{
			return theta * PIover180;
		}
		//Machine Epsilon (highest error in the floating point standard)
		const static float EPSILON = 5.96e-08f;

		const static unsigned int LOOKUP_ACCURACY = 1440;

		//Look Up Tables
		static float SIN[LOOKUP_ACCURACY];
		static float COS[LOOKUP_ACCURACY];
		//Table Generation bool
		static bool tablesGenerated = false;
		//Value Generation method
		static void GenerateLookUpTables(void)
		{
			if(!tablesGenerated)
			{
				for(int i = 0; i < LOOKUP_ACCURACY; i++)
				{
					SIN[i] = sinf(i);
					COS[i] = cosf(i);
				}
				tablesGenerated = true;
			}
			return;
		}

		static inline float Lerp(float a, float b, float t)
		{
			return ((a-b)*t)+b;
		}

		//Value accessor methods
		/*static float sin(float x)
		{ 
			if(!tablesGenerated)
			{
				GenerateLookUpTables();			
			}
			return SIN[(int)x];
		}
		static float cos(float x)
		{ 
			if(!tablesGenerated)
			{
				GenerateLookUpTables();			
			}
			return COS[(int)x]; 
		}*/

		/*int MinFloatToInt(float a, float b, float c)
		{
			return min(a,min(b,c));
		}

		int MaxFloatToInt(float a, float b, float c)
		{
			return max(a,max(b,c));
		}*/
	}
}