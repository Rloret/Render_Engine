#include "..\HeaderFiles\FBO.h"

FBO::FBO(float width, float height, bool dynamic)
{
	glGenFramebuffers(1, &fbo);

	glGenTextures(1, &depthBuffTexId);


	colorBuffer  = new Texture(GL_TEXTURE_2D, GL_RGBA8			  ,GL_RGBA  ,GL_UNSIGNED_BYTE, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST, true, width, height);
	normalBuffer = new Texture(GL_TEXTURE_2D, GL_RGB32F			  , GL_RGBA , GL_UNSIGNED_BYTE, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST, true, width, height);
	uvBuffer = new Texture(GL_TEXTURE_2D, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST, true, width, height);
	vertexBuffer = new Texture(GL_TEXTURE_2D, GL_RGB32F			  , GL_RGBA , GL_UNSIGNED_BYTE, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST, true, width, height);

	this->width = width;
	this->height = height;
	this->dynamic = dynamic;
}

void FBO::Resize(float width, float height)
{

	colorBuffer->Resize(GL_TEXTURE_2D, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_LINEAR,true ,width, height);
	normalBuffer->Resize(GL_TEXTURE_2D, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_LINEAR,true,  width, height);
	vertexBuffer->Resize(GL_TEXTURE_2D, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_LINEAR,true, width, height);
	uvBuffer->Resize(GL_TEXTURE_2D, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_LINEAR,true, width, height);



	glBindTexture(GL_TEXTURE_2D, depthBuffTexId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	


	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer->GetId(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, vertexBuffer->GetId(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, normalBuffer->GetId(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, uvBuffer->GetId(), 0);


	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffTexId, 0);

	GLenum buffs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };

	glDrawBuffers(4, buffs);

	if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{
		std::cerr << "Error configurando el FBO" << std::endl;
		exit(-1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

float FBO::readPixels(int attachment, float u, float v)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	float pixel[3];
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glReadPixels(roundl(u*width), roundl(v*height), 1, 1, GL_RGB, GL_FLOAT, pixel);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (pixel[0] < 0) {
		std::cout << "neg" << std::endl;
		pixel[0] = 0;
	}
	return pixel[0];
	//std::cout << "[" << pixel[0] << "," << pixel[1] << "," << pixel[2] << "]" << std::endl;
}

GLuint FBO::GetId()
{
	return fbo;
}

FBO::~FBO()
{
	delete colorBuffer;
	//delete depthBuffer;
	delete vertexBuffer;
	delete normalBuffer;
}
