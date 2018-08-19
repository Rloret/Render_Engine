#pragma once


#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <fstream>

#include "..\HeaderFiles\IRenderable.h"


class MeshData;

//This is the first Irenderable implemented, makes use of a meshData and draws the polygons using a vao. Sanother Irenderables were needed
//to extend the rendering type

class ModelObject : public IRenderable
{

private:
	
	MeshData* mData;

	int patches;
	std::vector<GLuint> registeredVBOs;
	std::string objectName;
	
	//void initTextures();
public:
	
	
	unsigned int vao;


	/*Instantiiates a modelObject to be rendered with the meshdata passed as parameter,triangle by triangle*/
	ModelObject(MeshData* data,Transform* transform, std::vector<Material*>& materials);
	/*Instantiiates a modelObject to be rendered with tesselation, with number of patches numberOfPatches*/
	ModelObject(MeshData* data, Transform* transform, std::vector<Material*>& materials, int numberOfPatches);
	
	~ModelObject();
	void LoadDataToGPU(Shader* s) override;
	void Render(glm::mat4& view, glm::mat4& proj, Shader* s)override;
	//void render(Camera* c,Material* m);

};

