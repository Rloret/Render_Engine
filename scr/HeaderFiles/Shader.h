#pragma once
#include <unordered_map>
#include <string>
#include <gl/glew.h>
#include <string>
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "..\HeaderFiles\auxiliar.h"

class Uniform;

#define uMAP std::unordered_map<std::string,Uniform*>
#define aMap std::unordered_map<std::string,int>

//This class is used as a program and shader linker that also holds information about the variables used in the shader.
class Shader {
public:
	Shader(const char* vname,const char*fname,bool verbose);
	Shader(const char* vname,  const char*geometrySource, const char*fname, bool verbose);
	Shader(const char * vname, const char* tcsSource, const char * tesSource, const char * fname, bool verbose);

	Shader(const char * vname, const char* tcsSource, const char * tesSource, const char * geometrySource, const char * fname, bool verbose);
	Shader(const char * computeName, bool verbose);

	GLuint loadShader(const char *fileName, GLenum type);
	

	int GetProgram();
	Uniform*  GetUniform(char* name);
	Uniform*  GetUniform(std::string name);
	int GetAttribute(char* name);
	void UseProgram();
	void setUsed(bool b);
	bool BeingUsed();

	RenderPolygonMode polygonMode = RenderPolygonMode::E_DEFAULT;



private:
	int program;
	bool inUse;


	uMAP uniformMap;
	aMap attributeMap;

};