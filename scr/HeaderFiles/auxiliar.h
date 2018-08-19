#pragma once

//Loads a texture and return width and height

unsigned char *loadTexture(const char* fileName, unsigned int &w, unsigned int &h);

//Carga un fichero en una cadena de caracteres
char *loadStringFromFile(const char *fileName, unsigned int &fileLen);

int nextPowerOfTwo(int v);

//this is here cause I did not know where else to put it. Sorry I guess.
enum RenderPolygonMode
{
	E_TRIS = 0, E_EDGES = 1, E_POINTS = 2, E_DEFAULT = 3
};