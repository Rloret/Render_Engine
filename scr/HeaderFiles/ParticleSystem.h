#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "..\HeaderFiles\IRenderable.h"
#include <gl/glew.h>

class Shader;
class MeshData;

//Not used.
class ParticleSystem : public IRenderable
{
public:
	ParticleSystem(int numParticles, int workgroups, glm::vec3 * initialVelocity1, float emissionSize, std::vector<Material*>& materials, Transform* t);
	void LoadDataToGPU(Shader* s);
	void Render(glm::mat4& view, glm::mat4& proj, Shader* s);


protected:

	int numberOfParticles;
	float emissionSize;
	float lifetime;
	GLuint vao;
	GLuint buffers[2];
	Shader* renderShader;
	glm::vec3* velocity;
	float WG;

	std::vector<int> registeredBuffers;
};

