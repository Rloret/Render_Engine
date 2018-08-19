#pragma once
#include "GL\glew.h"


class Texture {
private:
	int id;

public:
		//Default 2D texture, check the implementation, not flexible yet.
		Texture(const char* filename);
		//Normal 2D texture uploading: includes mipmapping
		Texture(const char* filename, GLenum min_filter,GLenum mag_filter, bool wrap);
		//USE this for 1D textures
		Texture(const char * filename, bool wrap);
		//Use this for color attachments of FBOs since it does not upload source data.
		Texture(GLenum textureType, GLenum encoding,GLenum format, GLenum type, GLenum min_filter, GLenum mag_filter, bool wrap, float width, float height);
		//Use this for CubeMap Textures
		Texture(const char* CubeMapTexturePlusLocation[6], GLenum encoding, GLenum format, GLenum type, GLenum min_filter, GLenum mag_filter, bool wrap);

		int GetId();
		void Resize(GLenum textureType, GLenum encoding, GLenum format, GLenum type, GLenum min_filter, GLenum mag_filter, bool wrap, float width, float height);
};