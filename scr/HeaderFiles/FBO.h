#pragma once
#include "..\HeaderFiles\Texture.h"
#include <iostream>
#include "string.h"

/*This class holds the daa of a typical G-buffer vertex,color, normals,uvs and depth of the visible objects
Although is used as a general FBO containerS*/
class FBO
{
public:
	FBO(float width, float height,bool dynamic);
	void Resize(float width, float height);
	float readPixels(int attachment,float u, float v);
	GLuint GetId();
	Texture* colorBuffer;

	Texture* vertexBuffer;
	Texture* normalBuffer;
	Texture* uvBuffer;
	~FBO();
	GLuint colorBuffTexId;
	GLuint depthBuffTexId;
	GLuint vertexBuffTexId;

private:
	GLuint fbo;

	float width, height;
	bool dynamic;




};

