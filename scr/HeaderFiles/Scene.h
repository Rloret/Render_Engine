#pragma once

#include <unordered_map>
#include "..\HeaderFiles\modelobject.h"
#include "..\HeaderFiles\FBO.h"

#include "Camera.h"
#include "Light.h"
#include "Texture.h"



class ParticleSystem;

//The scene class holds the main GLUT events and is responsible for rendering eacho f the objects and calling each of the materials
class Scene {

	std::unordered_map<std::string, MeshData*> sceneMeshes;
	std::vector<Texture*> sceneTextures;
	std::vector <Shader*> sceneShaders;
	std::vector <Material*> sceneMaterials;
	std::vector<std::vector<IRenderable*>> sceneObjects;

	std::vector<Light*>sceneLights;
	Camera * sceneCamera;
	


	std::pair<int, int> lightsBounds;
	float deltatime;
	float time=0;
	float ambientLight = 0.5;
	
	
public:
	
	Scene();


	void AddProgram(Shader* s);


	void AddMaterial(Material* m);
	void AddModelObject( int shader, IRenderable* model);
	void AddParticleSystem(ParticleSystem* system, int computeShader, int renderShader);

	void AddLight(glm::vec4& P, glm::vec4& D, glm::vec3& I, glm::vec3& att, float angle, int id);

	Camera* InitSceneCamera(glm::vec3 & cop, glm::vec2 windowSize);
	void InitSceneObjects();
	void SetLightBounds(int min, int max);
	void ResizeWindowEvent(int width, int height);
	void IdleEvent();
	void KeyboardEvent(unsigned char key, int x, int y);
	void MouseEvent(int button, int state, int x, int y);
	void MouseMotionEvent(int x, int y);
	void DisplayEvent();
	void setDeltaTime(float time);

	
};

