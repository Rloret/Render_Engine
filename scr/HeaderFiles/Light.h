#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GlobalVariables.h"
#include "..\HeaderFiles\Shader.h"
#include <string>




class Light {

	glm::vec4 lightPos;
	glm::vec4 direction;
	glm::vec3 I;
	glm::vec3 att;
	
	float cosbeta;
	int id;



	void setVec4(glm::vec4 v4, Shader* s, std::string st);
	void setVec3(glm::vec3 v3, Shader* s, std::string st);
	void setFloat(float v, Shader*s, std::string st);
	void setInt(float v, Shader*s, std::string st);

public:


	Light(glm::vec4& position, glm::vec4& direction, glm::vec3& intensity,
		glm::vec3& att, float angle, int ID);

	

	glm::vec4 GetPosition();
	glm::vec4 Getdirection();
	glm::vec3 GetIntensity();
	glm::vec3 Getattenuation();

	int GetId();
	float getAngle();


	void Render(glm::mat4 view, Shader* s);

};

