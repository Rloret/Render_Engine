#include "..\HeaderFiles\Shader.h"
#include "..\HeaderFiles\auxiliar.h"
#include "..\HeaderFiles\Uniform.h"
#include "..\HeaderFiles\Engine.h"
#include <iostream>



Shader::Shader(const char * vname, const char * fname, bool verbose)
{
	uniformMap = uMAP();
	attributeMap = aMap();
	GLuint vshader;
	GLuint fshader;
	vshader = loadShader(vname, GL_VERTEX_SHADER);
	fshader = loadShader(fname, GL_FRAGMENT_SHADER);

	program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);


	glLinkProgram(program);

	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		//glDeleteProgram(program);
		//program = 0;
		exit(-1);
	}

	GLint i;
	GLint count;

	GLint size; // size of the variable
	GLenum type; // type of the variable (float, vec3 or mat4, etc)

	const GLsizei bufSize = 32; // maximum name length
	GLchar name[bufSize]; // variable name in GLSL
	GLsizei length; // name length

	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
	if(verbose)printf("Active Uniforms: %d\n", count);

	for (i = 0; i < count; i++)
	{
		glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, name);
		Uniform *u = new Uniform(this, (GLuint)i, (GLuint)size, (char*)&name, type);
		char* n = (char*)&name;
		uMAP::const_iterator it = uniformMap.find(n);

		if (it == uniformMap.end()) {
			uniformMap.insert(std::pair<char*,Uniform*>((char*)&name,u));
			if(verbose)printf("AUniform #%d Type: %u Name: %s\n", i, type, name);
		}
		
		
	}
	count = 0;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
	if(verbose)printf("Active Attributes: %d\n", count);

	int loc = 0;
	for (i = 0; i < count; i++)
	{
		glGetActiveAttrib(program, (GLuint)i, bufSize, &length, &size, &type, name);
		loc = glGetAttribLocation(program, name);
		attributeMap.insert(std::pair<std::string, int>(name, loc));
		if(verbose)printf("Attribute #%d Type: %u Name: %s\n", i, type, name);

	}

	glDetachShader(program, vshader);
	glDetachShader(program, fshader);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glUseProgram(0);
	inUse = false;
}

Shader::Shader(const char * vname, const char * geometrySource, const char * fname, bool verbose)
{
	uniformMap = uMAP();
	attributeMap = aMap();
	GLuint vshader;
	GLuint fshader;
	GLuint gshader;
	vshader = loadShader(vname, GL_VERTEX_SHADER);
	fshader = loadShader(fname, GL_FRAGMENT_SHADER);
	gshader = loadShader(geometrySource, GL_GEOMETRY_SHADER);
	program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glAttachShader(program, gshader);

	glLinkProgram(program);

	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		//glDeleteProgram(program);
		//program = 0;
		exit(-1);
	}

	GLint i;
	GLint count;

	GLint size; // size of the variable
	GLenum type; // type of the variable (float, vec3 or mat4, etc)

	const GLsizei bufSize = 32; // maximum name length
	GLchar name[bufSize]; // variable name in GLSL
	GLsizei length; // name length

	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
	printf("Active Uniforms: %d\n", count);

	for (i = 0; i < count; i++)
	{
		glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, name);
		Uniform *u = new Uniform(this, (GLuint)i, (GLuint)size, (char*)&name, type);
		char* n = (char*)&name;
		uMAP::const_iterator it = uniformMap.find(n);

		if (it == uniformMap.end()) {
			uniformMap.insert(std::pair<char*, Uniform*>((char*)&name, u));
			if (verbose)printf("AUniform #%d Type: %u Name: %s\n", i, type, name);
		}


	}
	count = 0;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
	printf("Active Attributes: %d\n", count);

	int loc = 0;
	for (i = 0; i < count; i++)
	{
		glGetActiveAttrib(program, (GLuint)i, bufSize, &length, &size, &type, name);
		loc = glGetAttribLocation(program, name);
		attributeMap.insert(std::pair<std::string, int>(name, loc));
		if (verbose)printf("Attribute #%d Type: %u Name: %s\n", i, type, name);

	}

	glDetachShader(program, vshader);
	glDetachShader(program, fshader);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glUseProgram(0);
	inUse = false;
}
Shader::Shader(const char * vname, const char * tcsSource, const char * tesSource, const char * fname, bool verbose)
{
	uniformMap = uMAP();
	attributeMap = aMap();
	GLuint vshader;
	GLuint fshader;
	GLuint tcsShader;
	GLuint tesShader;
	vshader = loadShader(vname, GL_VERTEX_SHADER);
	fshader = loadShader(fname, GL_FRAGMENT_SHADER);
	tcsShader = loadShader(tcsSource, GL_TESS_CONTROL_SHADER);
	tesShader = loadShader(tesSource, GL_TESS_EVALUATION_SHADER);

	program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glAttachShader(program, tesShader);
	glAttachShader(program, tcsShader);

	glLinkProgram(program);

	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		//glDeleteProgram(program);
		//program = 0;
		exit(-1);
	}

	GLint i;
	GLint count;

	GLint size; // size of the variable
	GLenum type; // type of the variable (float, vec3 or mat4, etc)

	const GLsizei bufSize = 32; // maximum name length
	GLchar name[bufSize]; // variable name in GLSL
	GLsizei length; // name length

	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
	printf("Active Uniforms: %d\n", count);

	for (i = 0; i < count; i++)
	{
		glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, name);
		Uniform *u = new Uniform(this, (GLuint)i, (GLuint)size, (char*)&name, type);
		char* n = (char*)&name;
		uMAP::const_iterator it = uniformMap.find(n);

		if (it == uniformMap.end()) {
			uniformMap.insert(std::pair<char*, Uniform*>((char*)&name, u));
			if (verbose)printf("AUniform #%d Type: %u Name: %s\n", i, type, name);
		}


	}
	count = 0;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
	printf("Active Attributes: %d\n", count);

	int loc = 0;
	for (i = 0; i < count; i++)
	{
		glGetActiveAttrib(program, (GLuint)i, bufSize, &length, &size, &type, name);
		loc = glGetAttribLocation(program, name);
		attributeMap.insert(std::pair<std::string, int>(name, loc));
		if (verbose)printf("Attribute #%d Type: %u Name: %s\n", i, type, name);

	}

	glDetachShader(program, vshader);
	glDetachShader(program, fshader);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glUseProgram(0);
	inUse = false;

}
Shader::Shader(const char * vname,const char* tcsSource, const char * tesSource, const char * geometrySource, const char * fname, bool verbose)
{
	uniformMap = uMAP();
	attributeMap = aMap();
	GLuint vshader;
	GLuint fshader;
	GLuint gshader;
	GLuint tcsShader;
	GLuint tesShader;
	vshader = loadShader(vname, GL_VERTEX_SHADER);
	fshader = loadShader(fname, GL_FRAGMENT_SHADER);
	gshader = loadShader(geometrySource, GL_GEOMETRY_SHADER);
	tcsShader = loadShader(tcsSource, GL_TESS_CONTROL_SHADER);
	tesShader = loadShader(tesSource, GL_TESS_EVALUATION_SHADER);

	program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glAttachShader(program, gshader);
	glAttachShader(program, tesShader);
	glAttachShader(program, tcsShader);

	glLinkProgram(program);

	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		//glDeleteProgram(program);
		//program = 0;
		exit(-1);
	}

	GLint i;
	GLint count;

	GLint size; // size of the variable
	GLenum type; // type of the variable (float, vec3 or mat4, etc)

	const GLsizei bufSize = 32; // maximum name length
	GLchar name[bufSize]; // variable name in GLSL
	GLsizei length; // name length

	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
	printf("Active Uniforms: %d\n", count);

	for (i = 0; i < count; i++)
	{
		glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, name);
		Uniform *u = new Uniform(this, (GLuint)i, (GLuint)size, (char*)&name, type);
		char* n = (char*)&name;
		uMAP::const_iterator it = uniformMap.find(n);

		if (it == uniformMap.end()) {
			uniformMap.insert(std::pair<char*, Uniform*>((char*)&name, u));
			if (verbose)printf("AUniform #%d Type: %u Name: %s\n", i, type, name);
		}


	}
	count = 0;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
	printf("Active Attributes: %d\n", count);

	int loc = 0;
	for (i = 0; i < count; i++)
	{
		glGetActiveAttrib(program, (GLuint)i, bufSize, &length, &size, &type, name);
		loc = glGetAttribLocation(program, name);
		attributeMap.insert(std::pair<std::string, int>(name, loc));
		if (verbose)printf("Attribute #%d Type: %u Name: %s\n", i, type, name);

	}

	glDetachShader(program, vshader);
	glDetachShader(program, fshader);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glUseProgram(0);
	inUse = false;
}

Shader::Shader(const char * computeName, bool verbose)
{

	uniformMap = uMAP();
	attributeMap = aMap();
	GLuint cShader;

	cShader = loadShader(computeName, GL_COMPUTE_SHADER);


	program = glCreateProgram();
	glAttachShader(program, cShader);



	glLinkProgram(program);

	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		exit(-1);
	}

	GLint i;
	GLint count;

	GLint size; // size of the variable
	GLenum type; // type of the variable (float, vec3 or mat4, etc)

	const GLsizei bufSize = 32; // maximum name length
	GLchar name[bufSize]; // variable name in GLSL
	GLsizei length; // name length

	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
	printf("Active Uniforms: %d\n", count);

	for (i = 0; i < count; i++)
	{
		glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, name);
		Uniform *u = new Uniform(this, (GLuint)i, (GLuint)size, (char*)&name, type);
		char* n = (char*)&name;
		uMAP::const_iterator it = uniformMap.find(n);

		if (it == uniformMap.end()) {
			uniformMap.insert(std::pair<char*, Uniform*>((char*)&name, u));
			if (verbose)printf("AUniform #%d Type: %u Name: %s\n", i, type, name);
		}


	}
	count = 0;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
	printf("Active Attributes: %d\n", count);

	int loc = 0;
	for (i = 0; i < count; i++)
	{
		glGetActiveAttrib(program, (GLuint)i, bufSize, &length, &size, &type, name);
		loc = glGetAttribLocation(program, name);
		attributeMap.insert(std::pair<std::string, int>(name, loc));
		if (verbose)printf("Attribute #%d Type: %u Name: %s\n", i, type, name);

	}

	glDetachShader(program, cShader);
	glDeleteShader(cShader);
	glUseProgram(0);
	inUse = false;

}

GLuint Shader::loadShader(const char * fileName, GLenum type)
{
	unsigned int fileLen;
	char *source = loadStringFromFile(fileName, fileLen);

	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar **)&source, (const GLint *)&fileLen);
	glCompileShader(shader);
	delete[] source;


	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetShaderInfoLog(shader, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		glDeleteShader(shader);
		exit(-1);
	}


	return shader;
	
}

int Shader::GetProgram()
{
	return this->program;
}

Uniform * Shader::GetUniform(char* name)
{
	try{
		return uniformMap.at(name);
	}
	catch (const std::out_of_range& e) {
		//std::cout << "Your program does not use the variable "<<name<<std::endl;
		return nullptr;
	}
}
Uniform * Shader::GetUniform(std::string name)
{
	try {
		return uniformMap.at(name);
	}
	catch (const std::out_of_range& e) {
		//std::cout << "Your program does not use the variable " << name << std::endl;
		return nullptr;
	}
}


int Shader::GetAttribute(char * name)
{
	try {
		int v = attributeMap.at(name);
		return v;
	}
	catch (const std::out_of_range& e) {
		//std::cout << "Your program does not use the variable " << name << std::endl;
		return -1;
		//engine stop();
	}
}

void Shader::UseProgram()
{
	if (polygonMode == Engine::RenderMode || polygonMode == E_DEFAULT) {
		
		glUseProgram(this->program);
		if (polygonMode == E_POINTS) {
			glEnable(GL_POINTS);
			glPointSize(2);
		}
		setUsed(true);
	}
	
}

bool Shader::BeingUsed()
{
	return inUse;
}
void Shader::setUsed(bool b)
{
	inUse=b;
}