#include "..\HeaderFiles\Grid.h"
#include "..\HeaderFiles\Uniform.h"
#include "..\HeaderFiles\Material.h"
#include "..\HeaderFiles\Transform.h"
#include "..\HeaderFiles\GlobalVariables.h"
#include <gl/glew.h>

Grid::Grid(Transform* t, std::vector<Material*>& materials,float resolution)
{
	this->materials = materials;
	this->transform = t;
	this->resolution = resolution;
	vertexVBO = 0;
	vertexVAO = 0;
}

void Grid::LoadDataToGPU(Shader * s)
{
	GLfloat vertices_position[3] = {
				0.0,0,0,

	};
	int inPos = s->GetAttribute(GLOBAL_SHADER_ATR_VAR_INPOS);


	glGenVertexArrays(1, &vertexVAO);
	glBindVertexArray(vertexVAO);

	glGenBuffers(1, &vertexVBO);

	// Allocate space and upload the data from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_position), vertices_position, GL_STATIC_DRAW);

	glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(inPos);

	glBindVertexArray(0);
}

void Grid::Render(glm::mat4 & view, glm::mat4 & proj, Shader * s)
{


	for (Material* m : materials) {
		m->Render(s);
	}

	glBindVertexArray(vertexVAO);

	glm::vec3 tileSize = glm::vec3(1, 0, 1)*resolution;
	glm::vec3 gridOrigin = transform->GetPosition();
	glm::vec2 size =glm::vec2(transform->GetScale().x, transform->GetScale().z);
	//gridOrigin.x -= tileSize.x*size.x *0.5;
	//gridOrigin.z -= tileSize.z*size.y * 0.5;
	Uniform* uNormalMat = s->GetUniform(GLOBAL_SHADER_UNIF_VAR_NORMALMAT);
	Uniform* uModelViewMat = s->GetUniform(GLOBAL_SHADER_UNIF_VAR_MVMAT);
	Uniform* uModelViewProjMat = s->GetUniform(GLOBAL_SHADER_UNIF_VAR_MVPMAT);
	Uniform* uModel = s->GetUniform("Model");

	Uniform* utileSize = s->GetUniform("tileSize");
	Uniform* ugridSize = s->GetUniform("GridSize");
	Uniform* ugridOrigin = s->GetUniform("gridOrigin");



	glm::mat4 modelView = glm::mat4(1.0f);
	glm::mat4 model = this->transform->GetModel();
	modelView = view  *model;
	glm::mat4 modelViewProj = proj * view * model;
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));

	if (uModelViewMat != nullptr)
		((uMatrix4Float*)uModelViewMat)->Set(modelView, 0);
	if (uModelViewProjMat != nullptr) {
		((uMatrix4Float*)uModelViewProjMat)->Set(modelViewProj, 0);
	}
	if (uNormalMat != nullptr)
		((uMatrix4Float*)uNormalMat)->Set(normal, 0);
	if (uModel)((uMatrix4Float*)uModel)->Set(model, 0);
	if (utileSize)((u3Float*)utileSize)->Set(tileSize);
	if (ugridSize)((u2Float*)ugridSize)->Set(size);
	if (ugridOrigin)((u3Float*)ugridOrigin)->Set(gridOrigin);



	glPatchParameteri(GL_PATCH_VERTICES, 1);
	glDrawArraysInstanced(GL_PATCHES, 0, 1,size.x*size.y);
	//glEnable(GL_PROGRAM_POINT_SIZE);
	glBindVertexArray(0);
	for (Material* m : materials) {
		m->ResetState();
	}
	

}
