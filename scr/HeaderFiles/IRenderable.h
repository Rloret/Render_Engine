#pragma once
#include <glm/gtc/matrix_transform.hpp>

#include "..\HeaderFiles\Shader.h"


class Material;
class Transform;

//This is the interface for all of the renderable objects.
class IRenderable
{
protected:
	std::vector<Material*> materials;
	Transform*  transform;
public:

	virtual void LoadDataToGPU(Shader* s);
	virtual void Render(glm::mat4& view, glm::mat4& proj, Shader* s);

};



