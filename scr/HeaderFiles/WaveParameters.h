#pragma once
#include "glm\glm.hpp"
class WaveParameters
{
public:
	WaveParameters();
	WaveParameters(float Amplitude, float WaveLength, float Steepness, float angle);
	~WaveParameters();

	float A;
	float Q;
	float L;
	float angle;


};

