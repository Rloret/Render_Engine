#pragma once

#include <glm/gtc/matrix_transform.hpp>


//Analogous, kind of, at least in functionallity to Unitys
class Transform
{
public:
	Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	void Translate(glm::vec3 translation);
	void Rotate(glm::vec3 Rotation);
	void Scale(glm::vec3 scale);

	void SetTranslate(glm::vec3 translation);
	void SetRotate(glm::vec3 Rotation);
	void SetScale(glm::vec3 scale);
	void SetModel(glm::mat4 model);


	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();
	glm::mat4 GetModel();
	glm::vec3 Forward();
	glm::vec3 Right();

private:

	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 model;

	void updateModel();
};
