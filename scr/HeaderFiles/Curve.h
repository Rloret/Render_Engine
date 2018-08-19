#pragma once
#include "glm\glm.hpp"
#include <vector>

class FBO;

/*
	Class dedicated to creation of a catmull rom defined curve
*/
class Curve
{
public:
	Curve(std::vector<glm::vec2> points,float curveTotalTime);
	~Curve();

	static float maxHeight;
	static bool hasDebug;

	void setInitialPoint(glm::vec2 p);
	//Tried to implement a funnction to modify the points according to the terrai,
	//however did not manage to finish it o n time so it is not a feature
	//	void addTextureLookUp(FBO* fbo, float maxHeight, glm::vec2 dimensions);

	//
	glm::vec3 interPolate(float currenttime);
	glm::vec2 interPolate(float t, int a);

	std::vector<float> Evaluate(int numPoints);
	std::vector<float>debugPoints;

private:
	float time;
	float curveLength;
	glm::vec2 dimensions;
	FBO* Heights;
	glm::vec2 point;
	std::vector<glm::vec2>points;
	std::vector<float>segmentLengths;
	int initialPoints;


	void computeCurveLength();
	float computeSegmentLength(int a);
	int makeCircular(int i);

	float clip(float n, float lower, float upper);
};

