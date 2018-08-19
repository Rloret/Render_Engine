#include "..\HeaderFiles\Transform.h"

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	this->translation = position;
	this->rotation = rotation;
	this->scale = scale;
	updateModel();
}



void Transform::Translate(glm::vec3 translation)
{
	this->translation += translation;
	updateModel();
}

void Transform::Rotate(glm::vec3 Rotation)
{
	this->rotation += Rotation;
	updateModel();
}

void Transform::Scale(glm::vec3 scale)
{
	this->scale += scale;
	updateModel();
}

void Transform::SetTranslate(glm::vec3 translation)
{
	this->translation = translation;
	updateModel();

}

void Transform::SetRotate(glm::vec3 Rotation)
{
	this->rotation = Rotation;
	updateModel();

}

void Transform::SetScale(glm::vec3 scale)
{
	this->scale = scale;
	updateModel();

}

void Transform::SetModel(glm::mat4 model)
{
	this->model = model;
}

glm::vec3 Transform::GetPosition()
{
	return translation;
}

glm::vec3 Transform::GetRotation()
{
	return rotation;
}

glm::vec3 Transform::GetScale()
{
	return scale;
}

glm::mat4 Transform::GetModel()
{
	return model;
}

glm::vec3 Transform::Forward()
{
	return glm::vec3(model[2][0], model[2][1], model[2][2]);
}

glm::vec3 Transform::Right()
{
	return glm::vec3(model[0][0], model[0][1], model[0][2]);
}

void Transform::updateModel()
{

	glm::mat4 xRot(1.0f);
	glm::mat4 yRot(1.0f);
	glm::mat4 zRot(1.0f);

	glm::mat4 rotMatrix(1.0f);
	glm::mat4 tMatrix(1.0f);
	glm::mat4 sMatrix(1.0f);

	xRot = glm::rotate(xRot, rotation.x, glm::vec3(1, 0, 0));
	yRot = glm::rotate(yRot, rotation.y, glm::vec3(0, 1, 0));
	zRot = glm::rotate(zRot, rotation.z, glm::vec3(0, 0, 1));

	rotMatrix = xRot*yRot*zRot;

	sMatrix = glm::scale(sMatrix, scale);
	//we apply the inverse of the translation
	tMatrix = glm::translate(tMatrix, translation);

	model = tMatrix*rotMatrix*sMatrix;
}
