#pragma once
#include "..\HeaderFiles\IRenderable.h"


//This class configures the GPU to accept NxN 1 dimensional patches instanced to generate a mesh on the fly

class Grid : public IRenderable
{
public:
	Grid( Transform* t, std::vector<Material*>& materials,float resolution);
	void LoadDataToGPU(Shader* s) override;
	void Render(glm::mat4& view, glm::mat4& proj, Shader* s)override;

	float resolution;
private:

	unsigned int vertexVBO;
	unsigned int vertexVAO;
	unsigned int facesVBO;

	
};