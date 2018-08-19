#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

//Stores values related to a triangle indices structure.

class MeshData
{
public:
	MeshData(int nfaces, int ncoordinates, unsigned int* facesbuffer, float* coordbuffer, float* colorbuffer, float* normalbuffer, float* tcoordbuffer, float* tangentBuffer);
	MeshData(std::string &name);
	~MeshData();

	//buffers Size;
	int GetFacesSize();
	int GetCoordinatesSize();

	//Mesh data buffers
	float* GetCoordinates() const;
	float* GetColors()const;
	float* GetNormals()const;
	float* GetTangents()const;
	float* GetTCoords()const;
	unsigned int* GetFaces()const;

	void setImportedModel(glm::mat4 & m);
	
private:

	//buffers Size;
	int facesSize;
	int coordinatesSize;

	//Mesh data buffers
	float* coordinates;
	float* colors;
	float* normals;
	float* tangents;
	float* tCoords;
	unsigned int* faces;

	glm::mat4 importedModel;
};

