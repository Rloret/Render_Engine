#include "..\HeaderFiles\MeshData.h"
#include "..\HeaderFiles\GlobalVariables.h"
#include "..\HeaderFiles\AssimpLoader.h"

MeshData::MeshData(int nfaces, int ncoordinates, unsigned int * facesbuffer, float * coordbuffer, float * colorbuffer, float * normalbuffer, float * tcoordbuffer, float * tangentbuffer)
{
	facesSize = nfaces;
	coordinatesSize = ncoordinates;

	//Mesh data buffers
	coordinates = coordbuffer;
	colors = colorbuffer;
	normals = normalbuffer;
	tCoords = tcoordbuffer;
	faces = facesbuffer;
	tangents = tangentbuffer;

}

MeshData::MeshData(std::string & name)
{

}


MeshData::~MeshData()
{

	delete[] coordinates;
	delete[] colors;
	delete[] normals;
	delete[] tangents;
	delete[] tCoords;
}

int MeshData::GetFacesSize()
{
	return this->facesSize;
}

int MeshData::GetCoordinatesSize()
{
	return this->coordinatesSize;
}

float * MeshData::GetCoordinates()const
{
	return coordinates;
}

float * MeshData::GetColors()const
{
	return this->colors;
}

float * MeshData::GetNormals()const
{
	return this->normals;
}

float * MeshData::GetTangents()const
{
	return this->tangents;
}

float * MeshData::GetTCoords()const
{
	return this->tCoords;
}

unsigned int * MeshData::GetFaces()const
{
	return this->faces;
}

void MeshData::setImportedModel(glm::mat4& m)
{
	importedModel = m;
}
