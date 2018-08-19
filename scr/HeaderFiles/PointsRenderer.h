#pragma once
#include "..\HeaderFiles\IRenderable.h"
class Curve;

//This class is mainly used to debug a set of points.
class PointsRenderer: public IRenderable
{
public:
	PointsRenderer(Curve* c);
	~PointsRenderer();
	void LoadDataToGPU(Shader* s) override;
	void Render(glm::mat4& view, glm::mat4& proj, Shader* s)override;


	Curve* curve;
private:
	unsigned int vertexVBO;
	unsigned int vertexVAO;
	unsigned int facesVBO;
};

