#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "..\HeaderFiles\IRenderable.h"

class Material;
class Camera;
class Grid;
class Transform;
class QuadNode;


//One of the very first attemp	
class QuadTree : public IRenderable
{
public:
	QuadTree(Camera* cam,Transform* location,float initialScale,std::vector<Material*>mats,int maxDepth);
	~QuadTree();
	std::vector<Material*>mats;
	int maxDepth;
	void buildQuadTree();


	void LoadDataToGPU(Shader* s) override;
	void Render(glm::mat4& view, glm::mat4& proj, Shader* s)override;

	QuadNode* Root;
private:
	Camera* sceneCamera;
};

class QuadNode : public IRenderable
{
public:
	QuadNode(Transform* t, float scale,QuadTree* Tree);
	~QuadNode();
	
	QuadTree* tree;
	Grid* quadGrid;
	bool cameraInside(glm::vec3 position);
	void subdivide(int depth, int maxDepth, glm::vec3 & campos);
	void addGrid();
	void LoadDataToGPU(Shader* s) override;
	void Render(glm::mat4& view, glm::mat4& proj, Shader* s)override;
	Transform* Location;

private:
	float scale;
	QuadNode* children[4];
	QuadNode* father;


};

