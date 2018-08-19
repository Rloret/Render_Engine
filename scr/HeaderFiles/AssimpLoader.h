#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



#include <windows.h>
#include <iostream>
#include <vector>
#include <map>
#include "MeshData.h"


#define MeshPair std::pair<std::string, MeshData*>
#define MeshPairsList std::vector<MeshPair>

/*
	This class is fundamentally used to load objects from 3d model file formats 
*/
class AssimpLoader
{
private:
	std::vector<aiNode*> nodesWithMesh;
	void obtainNodeData(aiNode * node, std::vector<aiNode*>& nodesWithMesh);

	MeshPairsList retrieveSceneData(const aiScene * scene);
	MeshPairsList getAndLoadSceneData(const char * path);
public:


	AssimpLoader();
	MeshPairsList load3DFile(const char* filepath);
	
	~AssimpLoader();

};
