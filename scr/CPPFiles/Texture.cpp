#include "..\HeaderFiles\Texture.h"
#include "..\HeaderFiles\auxiliar.h"

#include <iostream>
Texture::Texture(const char * filename)
{
	unsigned char *map;
	unsigned int w, h;
	map = loadTexture(filename, w, h);

	if (!map)
	{
		std::cout << "Error cargando el fichero: "
			<< filename << std::endl;
		exit(-1);
	}

	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)map);

	id = texId;
	delete[] map;

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxaniso);

	
}

Texture::Texture(const char * filename, GLenum min_filter, GLenum mag_filter, bool wrap)
{

	unsigned char *map;
	unsigned int w, h;
	map = loadTexture(filename, w, h);

	if (!map)
	{
		std::cout << "Error cargando el fichero: "
			<< filename << std::endl;
		exit(-1);
	}

	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)map);


	delete[] map;

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	if (wrap) {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	}
	else{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	}
	id = texId;
}
//USE this for 1D textures
Texture::Texture(const char * filename, bool wrap)
{

	unsigned char *map;
	unsigned int w, h;
	map = loadTexture(filename, w, h);

	if (!map)
	{
		std::cout << "Error cargando el fichero: "
			<< filename << std::endl;
		exit(-1);
	}

	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_1D, texId);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, w, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)map);


	delete[] map;

	glGenerateMipmap(GL_TEXTURE_1D);


	if (wrap) {
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	}
	else {
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	}
	id = texId;
}
Texture::Texture(GLenum textureType, GLenum encoding, GLenum format, GLenum type, GLenum min_filter, GLenum mag_filter, bool wrap, float width, float height)
{
	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(textureType, texId);
	glTexImage2D(textureType, 0, encoding, width, height, 0, format, type, NULL);


	glGenerateMipmap(textureType);

	glTexParameterf(textureType, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameterf(textureType, GL_TEXTURE_MAG_FILTER, mag_filter);
	if (wrap) {
		glTexParameterf(textureType, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameterf(textureType, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	}
	else {
		glTexParameterf(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	}
	id = texId;
	glBindTexture(textureType, 0);

}
//In order:  right,left,top,bottom,back,front
Texture::Texture(const char* CubeMapTexturePlusLocation[6], GLenum encoding, GLenum format, GLenum type, GLenum min_filter, GLenum mag_filter, bool wrap)
{
	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

	for (size_t i = 0; i < 6; i++)
	{
		unsigned char *map;
		unsigned int w, h;
		map = loadTexture(CubeMapTexturePlusLocation[i], w, h);

		if (!map)
		{
			std::cout << "Error cargando el fichero: "
				<< CubeMapTexturePlusLocation[i] << std::endl;
			exit(-1);
		}
		else { glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, encoding, w, h, 0, format, type, map); }



	}


	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, mag_filter);
	if (wrap) {
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	}
	else {
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	}
	id = texId;
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}
int Texture::GetId()
{
	return id;
}



void Texture::Resize(GLenum textureType, GLenum encoding, GLenum format, GLenum type, GLenum min_filter, GLenum mag_filter, bool wrap, float width, float height){

	glBindTexture(textureType, id);
	glTexImage2D(textureType, 0, encoding, width, height, 0, format, type,NULL);
	glGenerateMipmap(textureType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);


	
}
