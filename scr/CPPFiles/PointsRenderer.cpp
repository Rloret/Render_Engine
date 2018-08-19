#include "..\HeaderFiles\PointsRenderer.h"

#include "..\HeaderFiles\Curve.h"
#include "..\HeaderFiles\GlobalVariables.h"

PointsRenderer::PointsRenderer(Curve * c)
{
	this->curve = c;
}

PointsRenderer::~PointsRenderer()
{
}

void PointsRenderer::LoadDataToGPU(Shader * s)
{

	std::vector<float>points =curve->Evaluate(1000);

	int inPos = s->GetAttribute(GLOBAL_SHADER_ATR_VAR_INPOS);


	glGenVertexArrays(1, &vertexVAO);
	glBindVertexArray(vertexVAO);

	glGenBuffers(1, &vertexVBO);

	// Allocate space and upload the data from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points[0]) * points.size() , &points[0], GL_STATIC_DRAW);

	glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(inPos);

	glBindVertexArray(0);
}

void PointsRenderer::Render(glm::mat4 & view, glm::mat4 & proj, Shader * s)
{
	if (Curve::hasDebug) {
		glBindVertexArray(vertexVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(curve->debugPoints[0]) * curve->debugPoints.size(), &curve->debugPoints[0], GL_STATIC_DRAW);
		glDrawArrays(GL_POINTS, 0, 1000);
		glEnable(GL_POINTS);
		glPointSize(20);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}
