#include "..\HeaderFiles\Light.h"
#include "..\HeaderFiles\Uniform.h"
#include <gl/glew.h>


Light::Light(glm::vec4& position, glm::vec4& direction, glm::vec3& intensity,
			 glm::vec3& att, float angle, int ID)
{
	lightPos = position;
	this->direction = direction;
	I = intensity;
	this->att = att;
	cosbeta = angle;
	id = ID;
}


int Light::GetId()
{
	return id;
}


glm::vec4 Light::GetPosition()
{
	return lightPos;
}

glm::vec4 Light::Getdirection()
{
	return direction;
}

glm::vec3 Light::GetIntensity()
{
	return I;
}

glm::vec3 Light::Getattenuation()
{
	return att;
}

float Light::getAngle()
{
	return cosbeta;
}



void Light::Render(glm::mat4 view,Shader* s)
{

	std::string base = GLOBAL_SHADER_LIGHTS_ARRAY_NAME +std::to_string(id);

	Uniform* u;

	glm::vec4 lightViewPos = lightPos ;
	glm::vec4 viewDirection = direction;

	//we first know if the light is  where the camera is discarding if it is a torch or not
	if (lightViewPos.x != 0 || lightViewPos.y != 0 && lightViewPos.z != 0) {
		//we then check whether its located at infinity
		if(lightViewPos.w==0)lightViewPos = -1.0f* view * lightViewPos;
		//or if it is just a normal light
		else lightViewPos = view * lightViewPos;
		viewDirection = view*viewDirection;
	}

	setVec4(lightViewPos, s,base+ GLOBAL_SHADER_UNIF_VAR_LIGHT_POSITION);
	setVec4(viewDirection, s, base+ GLOBAL_SHADER_UNIF_VAR_LIGHT_DIRECTION);
	setVec3(I, s,  base + GLOBAL_SHADER_UNIF_VAR_LIGHT_iNTENSITY);
	setVec3(att,s,base + GLOBAL_SHADER_UNIF_VAR_LIGHT_ATTENUATION);
	setFloat(cosbeta,s,base + GLOBAL_SHADER_UNIF_VAR_LIGHT_ANGLE);

	
}
void Light::setVec4(glm::vec4 v4, Shader* s,std::string st) {

	Uniform* u = s->GetUniform(st);
	if (u != nullptr) ((u4Float*)u)->Set(v4);
}


void Light::setVec3(glm::vec3 v3, Shader* s, std::string st) {

	Uniform* u = s->GetUniform(st);
	if (u != nullptr) ((u3Float*)u)->Set(v3);
}
void Light::setFloat(float f, Shader* s, std::string st) {

	Uniform* u = s->GetUniform(st);
	if (u != nullptr) ((uFloat*)u)->Set(f);
}
void Light::setInt(float i, Shader* s, std::string st) {

	Uniform* u = s->GetUniform(st);
	if (u != nullptr) ((uInt*)u)->Set(i);
}
