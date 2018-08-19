#include "..\HeaderFiles\AssimpLoader.h"


AssimpLoader::AssimpLoader()
{

}

MeshPairsList AssimpLoader::load3DFile(const char * filepath)
{
	 return getAndLoadSceneData(filepath);
}


AssimpLoader::~AssimpLoader()
{


}


//following http://assimp.sourceforge.net/lib_html/usage.html
MeshPairsList AssimpLoader::getAndLoadSceneData(const char * path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	if (!scene)
	{
		std::cout << importer.GetErrorString() << std::endl << std::endl << std::endl << ":(" << std::endl;;
		return MeshPairsList();
	}

	return retrieveSceneData(scene);

	
}


MeshPairsList AssimpLoader::retrieveSceneData(const aiScene * scene)
{
	auto it = scene->mRootNode;
	nodesWithMesh = std::vector<aiNode*>();
	//Recursive method that obtains leafs nodes

	obtainNodeData(it, nodesWithMesh);

	std::cout << "found " << nodesWithMesh.size() << " nodes" << std::endl;

	MeshPairsList objectList = MeshPairsList();
	objectList.reserve(nodesWithMesh.size());

	for (auto node : nodesWithMesh) {
		//There is a flaw in here not completely generic
		//I am doing this for specific models with just one mesh per object
		unsigned int idMesh = node->mMeshes[0];
		auto mesh = scene->mMeshes[idMesh];
		std::cout << "processing " << node->mName.C_Str() << std::endl;
		int i = 0;
		//Extract faces ids
		int facesSize = mesh->mNumFaces;
		unsigned int* faces = new unsigned int[facesSize * 3];
		std::cout << " F" << facesSize;

		for (i = 0; i < facesSize; i++) {
			faces[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
			faces[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			faces[i * 3 + 2] = mesh->mFaces[i].mIndices[2];

			//std::cout << "[" << (int)faces[i * 3 + 0] << " , " << (int)faces[i * 3 + 1] << " , " << (int)faces[i * 3 + 2] << "]"<<std::endl;
		}

		//Extract vertex coordinates
		int coordinatesSize = mesh->mNumVertices;
		float* coordinates = new float[coordinatesSize * 3];
		float* color = new float[coordinatesSize * 3];
		float* normal = new float[coordinatesSize * 3];
		float* tangents = new float[coordinatesSize * 3];
		float* tCoord = new float[coordinatesSize * 2];
		std::cout << " V" << coordinatesSize << std::endl;

		for (i = 0; i < coordinatesSize; i++) {
			coordinates[i * 3 + 0] = mesh->mVertices[i].x;
			coordinates[i * 3 + 1] = mesh->mVertices[i].y;
			coordinates[i * 3 + 2] = mesh->mVertices[i].z;
			if (mesh->HasNormals()) {
				normal[i * 3 + 0] = mesh->mNormals[i].x;
				normal[i * 3 + 1] = mesh->mNormals[i].y;
				normal[i * 3 + 2] = mesh->mNormals[i].z;
			}
			if (mesh->HasTangentsAndBitangents()) {
				tangents[i * 3 + 0] = mesh->mTangents[i].x;
				tangents[i * 3 + 1] = mesh->mTangents[i].y;
				tangents[i * 3 + 2] = mesh->mTangents[i].z;
			}

			if (mesh->HasTextureCoords(0)) {
				tCoord[i * 2 + 0] = mesh->mTextureCoords[0][i].x;
				tCoord[i * 2 + 1] = mesh->mTextureCoords[0][i].y;

			}




			//std::cout <<i<< "\t[" << coordinates[i * 3 + 0] << " , " << coordinates[i * 3 + 1] << " , " << coordinates[i * 3 + 2] << "]" << std::endl;
		}

		MeshData* m = new MeshData(mesh->mNumFaces, mesh->mNumVertices, faces, coordinates, color, normal, tCoord, tangents);
		glm::mat4 model = glm::mat4(node->mTransformation.a1, node->mTransformation.a2, node->mTransformation.a3, node->mTransformation.a4,
							 node->mTransformation.b1, node->mTransformation.b2, node->mTransformation.b3, node->mTransformation.b4,
							 node->mTransformation.c1, node->mTransformation.c2, node->mTransformation.c3, node->mTransformation.c4,
							 node->mTransformation.d1, node->mTransformation.d2, node->mTransformation.d3, node->mTransformation.d4) ;

		m->setImportedModel(model);
		objectList.push_back(MeshPair(std::string(node->mName.C_Str()),m));
		
		//std::cout << "created" << node->mName.C_Str() << std::endl;
	}
	return objectList;

}

void AssimpLoader::obtainNodeData(aiNode * node, std::vector<aiNode*>& nodesWithMesh)
{
	aiNode* it = node;
	if (it == nullptr)
	{
		return;
	}
	else
	{
		int children = it->mNumChildren;
		int nmeshes = it->mNumMeshes;
		if (children == 0 && nmeshes>0)
		{
			//store mesh to obtain its data
			nodesWithMesh.push_back(it);
			//std::cout << "Found " << it->mName.C_Str() << std::endl;

		}
		else {
			for (int i = 0; i < children; i++)
			{
				obtainNodeData(it->mChildren[i], nodesWithMesh);
			}
		}
	}

}


