#include "../HeaderFiles/Curve.h"
#include "../HeaderFiles/FBO.h"
#include <iostream>
#include <algorithm>

float Curve::maxHeight = 15;
bool Curve::hasDebug = true;
Curve::Curve(std::vector<glm::vec2> points, float curveTotalTime)
{
	this->points = points;
	this->time = curveTotalTime;
	this->initialPoints = points.size();
	segmentLengths = std::vector<float>(points.size());
	debugPoints = std::vector<float>(1);
	this->Heights = nullptr;
	assert(points.size() > 3 && std::string("You must provide at least 4 points ").c_str());
}

void Curve::setInitialPoint(glm::vec2 p)
{
	point = p;
	if (points.size() > initialPoints) {
		points[points.size() - 1] = p;
	}
	else {
		points.push_back(p);
		segmentLengths.push_back(0);
	}
	computeCurveLength();
	debugPoints = Evaluate(1000);
	
}

glm::vec3 Curve::interPolate(float currenttime)
{
	//check if it has already looped around the curve.
	float elapsedTime = currenttime > time ? currenttime - (floor(currenttime / time) * time):currenttime;
	
	int indexOfFirstPointInSegment = makeCircular(-1);
	float timeUntilSegment = 0;
	float timeOfSegment;

	//We check, using the time in which segment we are, we are using the last position, since supposedly the curve connects
	//with the COP of the camera, which is the last point in the array
	for (size_t i = 0; i < segmentLengths.size(); i++)
	{
		int j = makeCircular(i - 1);
		timeOfSegment = (segmentLengths[j] / curveLength)*time;
		timeUntilSegment += timeOfSegment;
		if (elapsedTime <= timeUntilSegment) {
			indexOfFirstPointInSegment = j;
			timeUntilSegment -= timeOfSegment;
			break;
		}

	}
	//Once we know which segment we are we estimate the interpolator value using the computed length
	//and the percent of whole curve length. This way, shorter paths are traversed faster than longer paths.
	float percentInSegment = (elapsedTime - timeUntilSegment) / timeOfSegment;
	
	//std::cout << percentInSegment << std::endl;
	//std::cout << " \t interval " << indexOfFirstPointInSegment << " \tpercent :" << percentInSegment << std::endl;
	//std::cout << "Time of segment" << timeOfSegment << std::endl;
	//std::cout << "Time until segment" << timeUntilSegment << std::endl;

	glm::vec2 pos = interPolate(percentInSegment, indexOfFirstPointInSegment);
	glm::vec3 finalPos = glm::vec3(pos.x, maxHeight, pos.y);
	if (this->Heights) {
		glm::vec2 p = glm::vec2(clip(pos.x, 0, dimensions.x), clip(pos.y, 0, dimensions.y));
		//float height = Heights->readPixels(0, p.x / dimensions.x, p.y / dimensions.y);
		finalPos.y =  maxHeight ;
		
	}

	//std::cout << finalPos.x << " " << finalPos.y << " " << finalPos.z<< std::endl;
	return finalPos;
	


}

glm::vec2 Curve::interPolate(float t, int a)
{

	glm::vec2 P0 = points[makeCircular(a - 1)];
	glm::vec2 P1 = points[makeCircular(a)];
	glm::vec2 P2 = points[makeCircular(a+1)];
	glm::vec2 P3 = points[makeCircular(a+2)];

	glm::vec2 point;
	point = t*t*t*((-1.0f) * P0 + 3.0f * P1 - 3.0f * P2 + P3) / 2.0f;
	point += t*t*(2.0f * P0 - 5.0f * P1 + 4.0f * P2 - P3) / 2.0f;
	point += t*((-1.0f) * P0 + P2) / 2.0f;
	point += P1;

	return point;
}

std::vector<float> Curve::Evaluate(int numPoints)
{
	computeCurveLength();
	int numPointsPerSegment;
	std::vector<float> points;
	points.reserve(numPoints * 3);

	for (size_t i = 0; i < segmentLengths.size(); i++)
	{
		int j = makeCircular(i - 1);
		float nump = round((segmentLengths[j] / curveLength)*numPoints);
		numPointsPerSegment =nump ;

		for (size_t k = 0; k < numPointsPerSegment; k++)
		{
			float percentInSegment = (float)k / numPointsPerSegment;
			glm::vec2 pos = interPolate(percentInSegment, j);
			points.push_back(pos.x);
			if (this->Heights) {
				glm::vec2 p = glm::vec2(clip(pos.x, 0, dimensions.x), clip(pos.y, 0, dimensions.y));
				float height = Heights->readPixels(0, p.x / dimensions.x, p.y / dimensions.y);
				points.push_back(2 * maxHeight *height);
			}
			else points.push_back(maxHeight);
			points.push_back(pos.y);
		}
		
	}

	return points;
}

void Curve::computeCurveLength()
{
	curveLength = 0;
	for (size_t i = 0; i < points.size(); i++)
	{
		segmentLengths[i] = computeSegmentLength(i);
		curveLength += segmentLengths[i];
	}

}
/*
void Curve::addTextureLookUp(FBO * fbo, float maxHeight, glm::vec2 dimensions)
{
	this->Heights = fbo;
	this->maxHeight = maxHeight;
	this->dimensions = dimensions;
}*/

float Curve::computeSegmentLength(int a)
{
	int samples = 10;
	float length=0;
	glm::vec2 currentPoint = points[a];
	glm::vec2 nextPoint;
	
	for (size_t i = 1; i <= samples; i++)
	{
		float t = i*1.0f / samples;
		nextPoint = interPolate(t, a);
		float lengthSofar = glm::distance(nextPoint , currentPoint);
		length += lengthSofar;
		currentPoint = nextPoint;
	}
	return length;
}

int Curve::makeCircular(int i)
{
	bool asd = i > (points.size() - 1);
	if (i < 0) return (points.size() - 1);
	else if (i > (points.size() - 1)) {
		return i - points.size();
	}
	else return i;
}

float Curve::clip(float n, float lower, float upper) {
	return std::max(lower, std::min(n, upper));
}