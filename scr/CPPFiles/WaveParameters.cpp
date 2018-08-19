#include "..\HeaderFiles\WaveParameters.h"

#include <random>

#define clamp(n, min, max) if (n < min) n= min; else if (n > max) n= max;
#define rand ((double)rand() / (RAND_MAX))
#define remap(r,a,b,c,d) ((r-a) *((d-c)/(b-a)+c))

#define PI 3.14159265358979323846264338327950288419716f
#define DEG2RAD PI/180
WaveParameters::WaveParameters() {};
WaveParameters::WaveParameters(float Amplitude, float WaveLength, float Steepness, float angle)
{
	A = Amplitude;
	this->angle = angle;
	Q = Steepness>1?1:Steepness;
	L = WaveLength;


}


WaveParameters::~WaveParameters()
{
}
