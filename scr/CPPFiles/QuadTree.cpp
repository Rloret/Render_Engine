#include "..\HeaderFiles\QuadTree.h"
#include "..\HeaderFiles\Transform.h"
#include "..\HeaderFiles\Grid.h"
#include "..\HeaderFiles\Camera.h"
#include "..\HeaderFiles\Material.h"

QuadTree::QuadTree(Camera * cam, Transform * location, float initialScale, std::vector<Material*>mats,int maxDepth)
{
	Root = new QuadNode(location, initialScale,this);
	sceneCamera = cam;
	this->mats = mats;
	this->maxDepth = maxDepth;

	buildQuadTree();
}

QuadTree::~QuadTree()
{
	delete Root;
	sceneCamera = nullptr;

}

void QuadTree::buildQuadTree()
{
	glm::vec3 pos = sceneCamera->get_Position();
	bool contains = Root->cameraInside(pos);
	if (contains) {
		//if (Root->quadGrid) delete Root->quadGrid;
		Root->subdivide(0, maxDepth, pos);
	}
	else {
		
		//Root->addGrid();
	}
}

void QuadTree::LoadDataToGPU(Shader * s)
{
	Root->LoadDataToGPU(s);
}

void QuadTree::Render(glm::mat4 & view, glm::mat4 & proj, Shader * s)
{
	buildQuadTree();
	Root->LoadDataToGPU(s);
	Root->Render(view, proj, s);
}
	
QuadNode::QuadNode(Transform * t, float scale,QuadTree*tree)
{	
	this->Location = t;
	this->scale = scale;
	for (int i = 0; i < 4; i++) {
		children[i] = nullptr;
	}
	this->tree = tree;
	this->quadGrid = nullptr;
}

QuadNode::~QuadNode()
{
	delete this->Location;
	if (quadGrid) delete quadGrid;
	this->tree = nullptr;
}

bool QuadNode::cameraInside(glm::vec3 position)
{
	glm::vec3 botRight = this->Location->GetPosition();
	glm::vec3 topLeft = botRight + Location->GetScale()*scale;
	bool inside=	position.x <= topLeft.x &&
		position.x >= botRight.x &&
		position.z <= topLeft.z &&
		position.z >= botRight.z;
	//if (inside)std::cout << "inside " << botRight.x << " " << botRight.z << " and "<< topLeft.x<<" "<<topLeft.z << std::endl;

	return inside;
}

void QuadNode::subdivide(int depth, int maxDepth, glm::vec3 & campos)
{
	float scale = this->scale / 2.0;
	glm::vec3 lowerCorner = this->Location->GetPosition();

	glm::vec3 length = Location->GetScale()*this->scale;

	glm::vec3 upperCorner = lowerCorner + length;


	glm::vec3 p0 = lowerCorner ;
	glm::vec3 p2 = 0.5f*(lowerCorner + upperCorner);
	glm::vec3 p1 = glm::vec3(p2.x,0,p0.z);
	glm::vec3 p3 = glm::vec3(p0.x, 0, p2.z);

	glm::vec3 s = this->Location->GetScale();
	if (depth != 0) {
		for (int i = 0; i < 4; i++)
		{
			if(children[i]!=nullptr)delete children[i];
			children[i] = nullptr;
		}
	}
	children[0] = new QuadNode(new Transform(p0, glm::vec3(0), s),scale,tree);
	children[1] = new QuadNode(new Transform(p1, glm::vec3(0), s), scale, tree);
	children[2] = new QuadNode(new Transform(p2, glm::vec3(0), s), scale, tree);
	children[3] = new QuadNode(new Transform(p3, glm::vec3(0), s), scale, tree);
	

	for (int i = 0; i < 4; i++)
	{
		if ( children[i]->cameraInside(campos) && (depth + 1 <= maxDepth)) {
			
			children[i]->subdivide(depth + 1, maxDepth, campos);
			
		}
		else {
			children[i]->addGrid();

		}
		
	}


}

void QuadNode::addGrid()
{
	//std::cout<<"New grid at "<<Location->GetPosition().x << " " << Location->GetPosition().y << " " << Location->GetPosition().z << "of scale "<< scale * tree->Root->Location->GetScale().x<< std::endl;
	quadGrid = new Grid(Location, tree->mats, scale);

}

void QuadNode::LoadDataToGPU(Shader * s)
{

	if (this->quadGrid != nullptr)quadGrid->LoadDataToGPU( s);
	else {
		for (size_t i = 0; i < 4; i++)
		{
			if (children[i] != nullptr)children[i]->LoadDataToGPU(s);
		}
	}

}

void QuadNode::Render(glm::mat4 & view, glm::mat4 & proj, Shader * s)
{
	if (this->quadGrid != nullptr)quadGrid->Render(view,proj,s);
	else {
		for (size_t i = 0; i < 4; i++)
		{
			if (children[i] != nullptr)children[i]->Render(view, proj, s);
		}
	}
}
