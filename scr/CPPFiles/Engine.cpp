#include "..\HeaderFiles\Engine.h"
#include "..\HeaderFiles\Uniform.h"
#include "..\HeaderFiles\Material.h"
#include "..\HeaderFiles\AssimpLoader.h"
#include "..\HeaderFiles\Transform.h"
#include "..\HeaderFiles\quad.h"
#include "..\HeaderFiles\WaveParameters.h"
#include "..\HeaderFiles\Grid.h"
#include "..\HeaderFiles\Curve.h"
#include "..\HeaderFiles\auxiliar.h"
#include "..\HeaderFiles\PointsRenderer.h"
#include <windows.h>

#include "GL/glew.h"

#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <ratio>
#include <chrono>
#include <random>


#define WIDTH 1280		
#define HEIGHT 720

Scene* Engine::currentScene=nullptr;
std::vector<Scene*> Engine::Scenes = std::vector<Scene*>();
float Engine::DeltaTime = 0;
float Engine::LastTime = 0;
double Engine::totalRenderTime=0;
double Engine::totalComputeTime=0;

RenderPolygonMode Engine::RenderMode =RenderPolygonMode::E_TRIS;
long int Engine::totalIterations = 0;
std::chrono::steady_clock::time_point Engine::beginTime = std::chrono::steady_clock::time_point();
std::chrono::steady_clock::time_point Engine::endTime = std::chrono::steady_clock::time_point();


Engine::Engine()
{
}

void Engine::Start(int argc, char** argv)
{

	initContext(argc, argv);
	initOGL();

	DeltaTime = glutGet(GLUT_ELAPSED_TIME);
	LastTime = DeltaTime;
	//instantiate Scene
	currentScene = new Scene();
	//Intiscene
	Camera * camera = currentScene->InitSceneCamera(glm::vec3(150, 10, 150), glm::vec2(WIDTH, HEIGHT));
	

	//create Shader programs, note that if the shader requires communication with the CPU it will have its Material class in cpu
	Shader* s_perlinToTexture = new Shader("../Shaders/Deferred.vert", "../Shaders/PerlinToTexture.frag",true);
	Shader* s_GridTerrainnoGrass = new Shader("../Shaders/Grid.vert", "../Shaders/Grid.tcs", "../Shaders/Grid.tes", "../Shaders/GridPerlinFromTexture.geom", "../Shaders/PerlinFromTexture.frag", false);
	s_GridTerrainnoGrass->polygonMode = E_TRIS;
	Shader* s_GridTerrainnoGrassL = new Shader("../Shaders/Grid.vert", "../Shaders/Grid.tcs", "../Shaders/Grid.tes", "../Shaders/GridPerlinFromTextureL.geom", "../Shaders/PerlinFromTexture.frag", false);
	s_GridTerrainnoGrassL->polygonMode = E_EDGES;
	Shader* s_GridTerrainnoGrassP = new Shader("../Shaders/Grid.vert", "../Shaders/Grid.tcs", "../Shaders/Grid.tes", "../Shaders/GridPerlinFromTextureP.geom", "../Shaders/PerlinFromTexture.frag", false);
	s_GridTerrainnoGrassP->polygonMode = E_POINTS;

	Shader* s_GridTerrainGrass = new Shader("../Shaders/Grid.vert", "../Shaders/Grid.tcs", "../Shaders/Grid.tes", "../Shaders/GridPerlinFromTexturePlusGrass.geom", "../Shaders/PerlinFromTexturePlusGrass.frag", false);
	s_GridTerrainGrass->polygonMode = E_TRIS;
	Shader* s_GridTerrainGrassL = new Shader("../Shaders/Grid.vert", "../Shaders/Grid.tcs", "../Shaders/Grid.tes", "../Shaders/GridPerlinFromTexturePlusGrassL.geom", "../Shaders/PerlinFromTexturePlusGrass.frag", false);
	s_GridTerrainGrassL->polygonMode = E_EDGES;
	Shader* s_GridTerrainGrassP = new Shader("../Shaders/Grid.vert", "../Shaders/Grid.tcs", "../Shaders/Grid.tes", "../Shaders/GridPerlinFromTexturePlusGrassP.geom", "../Shaders/PerlinFromTexturePlusGrass.frag", false);
	s_GridTerrainGrassP->polygonMode = E_POINTS;


	Shader* s_GridWater = new Shader("../Shaders/Grid.vert", "../Shaders/Grid.tcs", "../Shaders/Grid.tes", "../Shaders/GridGerstner.geom", "../Shaders/Gerstner.frag", false);
	s_GridWater->polygonMode = E_TRIS;
	Shader* s_GridWaterL = new Shader("../Shaders/Grid.vert", "../Shaders/Grid.tcs", "../Shaders/Grid.tes", "../Shaders/GridGerstnerL.geom", "../Shaders/Gerstner.frag", false);
	s_GridWaterL->polygonMode = E_EDGES;
	Shader* s_GridWaterP = new Shader("../Shaders/Grid.vert", "../Shaders/Grid.tcs", "../Shaders/Grid.tes", "../Shaders/GridGerstnerP.geom", "../Shaders/Gerstner.frag", false);
	s_GridWaterP->polygonMode = E_POINTS;

	Shader* s_Defered = new Shader("../Shaders/Deferred.vert", "../Shaders/Deferred.frag", false);
    Shader* s_Skybox = new Shader("../Shaders/Cubemap.vert", "../Shaders/Cubemap.frag", false);
	s_Skybox->polygonMode = E_TRIS;
	Shader* s_points = new Shader("../Shaders/standardVertex.vert", "../Shaders/standardFrag.frag", false);
	
   //Add programs, the order is important!!

	currentScene->AddProgram(s_perlinToTexture);
	currentScene->AddProgram(s_GridTerrainnoGrass);
	currentScene->AddProgram(s_GridTerrainnoGrassL);
	currentScene->AddProgram(s_GridTerrainnoGrassP);
	currentScene->AddProgram(s_GridWater);
	currentScene->AddProgram(s_GridWaterL);
	currentScene->AddProgram(s_GridWaterP);
	currentScene->AddProgram(s_Defered);// until here is somewhat a defered shader it iluminates and gives back color to de front buffer and depth buffer
	currentScene->AddProgram(s_GridTerrainGrass);// transparent pass for grass
	currentScene->AddProgram(s_GridTerrainGrassL);// transparent pass for grass
	currentScene->AddProgram(s_GridTerrainGrassP);// transparent pass for grass

	currentScene->AddProgram(s_Skybox);
	currentScene->AddProgram(s_points);


	//Create the few geometry needed.

	MeshData* quadData = new MeshData(planeNFaces, planeNVertex, &planeFacesOrder[0], &planeVertexPos[0], nullptr, nullptr, nullptr, nullptr); // full Scree Quad
	Transform* TerrainTransform = new Transform(glm::vec3(0, -15, -0), glm::vec3(-0, 0, 0), glm::vec3(64, 1, 64));
	//Transform* WaterTransform = new Transform(glm::vec3(0, 0, -0), glm::vec3(-0, 0, 0), glm::vec3(32, 1, 32));

	
	FBO* terrainTexturesFBO = new FBO(1024, 1024, false);
	FBO* terrainFBO = new FBO(WIDTH, HEIGHT, true);
	FBO* waterFBO = new FBO(WIDTH, HEIGHT, true);

	
	Texture* terrainTexture = new Texture("../Textures/Terrain_Gradient.png", true);
	Texture* waterFoam = new Texture("../Textures/Water_Foam.png", GL_LINEAR, GL_LINEAR, true);

	srand(time(NULL));

	const char* locations[] = { "../Textures/Skybox/xneg.png",
		"../Textures/Skybox/xpos.png",
		"../Textures/Skybox/yneg.png",
		"../Textures/Skybox/ypos.png",
		"../Textures/Skybox/zneg.png",
		"../Textures/Skybox/zpos.png" };

	Texture* skybox = new Texture(locations, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false);

	//Create materials
	PerlinMaterial* perlinTerrainOpaque = new PerlinMaterial(8,30, 10, terrainTexture,terrainTexturesFBO,false);
	PerlinMaterial* perlinTerrainTransparent = new PerlinMaterial(8, 30, 10, terrainTexture, terrainTexturesFBO,true);

	RenderToFBOMaterial* PerlinToFBO = new RenderToFBOMaterial(terrainTexturesFBO);
	GridMaterial*  terrainGridMat = new GridMaterial(32);
	GridMaterial*  terrainGrassMat = new GridMaterial(30);

	GerstnerWavesMaterial* gerstnerMat = new GerstnerWavesMaterial(WaveParameters(10, 20, 1, 2), 32, 60);
	SkyboxMaterial* skyboxMat = new SkyboxMaterial(skybox);
	RenderToFBOMaterial* PerlinGeometryToFBO = new RenderToFBOMaterial(terrainFBO);
	RenderToFBOMaterial* GerstnerGeometryToFBO = new RenderToFBOMaterial(waterFBO);


	PostProccessMaterial* ppShader = new PostProccessMaterial(waterFBO,terrainFBO,waterFoam,skybox);

	//Add materials that are going to register events
	currentScene->AddMaterial(PerlinToFBO);
	currentScene->AddMaterial(perlinTerrainOpaque);
	currentScene->AddMaterial(gerstnerMat);
	currentScene->AddMaterial(perlinTerrainTransparent);
	currentScene->AddMaterial(PerlinGeometryToFBO);
	currentScene->AddMaterial(GerstnerGeometryToFBO);
	currentScene->AddMaterial(ppShader);
	currentScene->AddMaterial(skyboxMat);

	//Configure all the geometry used by the scene
	AssimpLoader* loader = new AssimpLoader();
	MeshData* cubeData = loader->load3DFile("../Meshes/Cube.fbx")[0].second;

	ModelObject* FSQ = new ModelObject(quadData, TerrainTransform, std::vector<Material*> {perlinTerrainOpaque,PerlinToFBO});
	Grid* terrainGrid = new Grid(TerrainTransform, std::vector<Material*> {perlinTerrainOpaque, terrainGridMat, PerlinGeometryToFBO}, 10);
	Grid* terrainGrass = new Grid(TerrainTransform, std::vector<Material*> {perlinTerrainTransparent, terrainGrassMat}, 10);
	Grid* waterGrid = new Grid(TerrainTransform, std::vector<Material*> {gerstnerMat, terrainGridMat, GerstnerGeometryToFBO}, 10);
	ModelObject* Cube = new ModelObject(cubeData, new Transform(glm::vec3(0), glm::vec3(0), glm::vec3(2000)), std::vector<Material*> {skyboxMat}); // The quad receives the instructions for post processing

	ModelObject* FSQ1 = new ModelObject(quadData, TerrainTransform, std::vector<Material*> {ppShader});

	std::vector<glm::vec2> Points;
	float percentOfGrid=0.5;
	float S = terrainGrid->resolution * TerrainTransform->GetScale().x ;
	glm::vec2 D = glm::vec2(1.0 - percentOfGrid)*S/2.0f;
	S *= percentOfGrid;
	Points.reserve(10);
	Points.push_back(D +S *glm::vec2(0,	 1));
	Points.push_back(D +S *glm::vec2(0.25, 0.75));
	Points.push_back(D +S *glm::vec2(0,	 0.5));
	Points.push_back(D +S *glm::vec2(0.25, 0.25));
	Points.push_back(D +S *glm::vec2(0,	 0));
	Points.push_back(D +S *glm::vec2(1,	 0));/*
	Points.push_back(D +S *glm::vec2(0.75, 0.25));
	Points.push_back(D +S *glm::vec2(1,	 0.5));
	Points.push_back(D +S *glm::vec2(0.75, 0.75));
	Points.push_back(D +S *glm::vec2(1,	1 ));
	*/


	Curve* CameraCurve = new Curve(Points, 40.0f);
	camera->addCurve(CameraCurve);

	PointsRenderer* points = new PointsRenderer(CameraCurve);
	//the index represents the shader, remember the order?
	currentScene->AddModelObject(0, FSQ);
	currentScene->AddModelObject(1, terrainGrid);
	currentScene->AddModelObject(2, terrainGrid);
	currentScene->AddModelObject(3, terrainGrid);
	currentScene->AddModelObject(4, waterGrid);
	currentScene->AddModelObject(5, waterGrid);
	currentScene->AddModelObject(6, waterGrid);
	currentScene->AddModelObject(7, FSQ1);
	currentScene->AddModelObject(8, terrainGrass);
	currentScene->AddModelObject(9, terrainGrass);
	currentScene->AddModelObject(10, terrainGrass);
	currentScene->AddModelObject(11, Cube);
	currentScene->AddModelObject(12, points);


	currentScene->AddLight(glm::vec4(20, -20, 0, 0), glm::vec4(0), glm::vec3(1), glm::vec3(1, 0.01, 0.001), 0, 0);
	currentScene->SetLightBounds(0, 1);

	currentScene->InitSceneObjects();
	Scenes.push_back(currentScene);

	// Scene to probe seamless

	Scene* s2 = new Scene();
	Shader* s_simplePerlin = new Shader("../Shaders/Grid.vert", "../Shaders/Grid.tcs", "../Shaders/Grid.tes", "../Shaders/GridPerlin.geom", "../Shaders/Perlin.frag", false);
	Shader* s_simpleGerstner = new Shader("../Shaders/Grid.vert", "../Shaders/Grid.tcs", "../Shaders/Grid.tes", "../Shaders/GridGerstner.geom", "../Shaders/Gerstner.frag", false);
	
	s2->AddProgram(s_simplePerlin);
	s2->AddProgram(s_simpleGerstner);
	s2->AddProgram(s_Defered);
	
	Transform* TerrainTransform2 = new Transform(glm::vec3(-64.05, -15, -0), glm::vec3(-0, 0, 0), glm::vec3(64, 1, 64));
	Transform* TerrainTransform3 = new Transform(glm::vec3(0, -15, -0), glm::vec3(-0, 0, 0), glm::vec3(64, 1, 64));

	
	FBO* terrain = new FBO(WIDTH, HEIGHT, true);

	RenderToFBOMaterial* PerlinToFBO2 = new RenderToFBOMaterial(terrain);
	Grid* terrainGrid2 = new Grid(TerrainTransform3, std::vector<Material*> {perlinTerrainOpaque, terrainGridMat}, 1);
	Grid* terrainGrid3 = new Grid(TerrainTransform2, std::vector<Material*> {perlinTerrainOpaque, terrainGridMat}, 1);

	FBO* water = new FBO(WIDTH, HEIGHT, true);

	RenderToFBOMaterial* water2FBO2 = new RenderToFBOMaterial(terrain);
	Grid* waterGrid2 = new Grid(TerrainTransform3, std::vector<Material*> {gerstnerMat, terrainGridMat}, 1);
	Grid* waterGrid3 = new Grid(TerrainTransform2, std::vector<Material*> {gerstnerMat, terrainGridMat}, 1);

	PostProccessMaterial* ppShader2 = new PostProccessMaterial(water, terrain, waterFoam, skybox);
	ModelObject* FSQ_1 = new ModelObject(quadData, TerrainTransform, std::vector<Material*> {ppShader2});

	s2->AddMaterial(perlinTerrainOpaque);
	s2->AddMaterial(PerlinToFBO2);
	s2->AddMaterial(terrainGridMat);

	s2->AddMaterial(gerstnerMat);
	s2->AddMaterial(water2FBO2);

	s2->AddModelObject(0, terrainGrid2);
	s2->AddModelObject(0, terrainGrid3);
	
	s2->AddModelObject(1, waterGrid2);
	s2->AddModelObject(1, waterGrid3);



	s2->AddLight(glm::vec4(20, -20, 0, 0), glm::vec4(0), glm::vec3(1), glm::vec3(1, 0.01, 0.001), 0, 0);
	s2->SetLightBounds(0, 1);

	Camera * camera2 = s2->InitSceneCamera(glm::vec3(-32, 20, 16), glm::vec2(WIDTH, HEIGHT));


	s2->InitSceneObjects();
	Scenes.push_back(s2);

	glutMainLoop();



}

void Engine::ResizeWindowEvent(int width, int height)
{
	glViewport(0, 0, width, height);
	if (Engine::currentScene)Engine::currentScene->ResizeWindowEvent(width,height);
	glutPostRedisplay();
}

void Engine::idleEvent()
{
	if(Engine::currentScene)Engine::currentScene->IdleEvent();
	glutPostRedisplay();
}

void Engine::KeyboardEvent(unsigned char key, int x, int y)
{
	if (key == '1') {
		currentScene = Scenes[0];
	}
	if (key == '2') {
		currentScene = Scenes[1];
	}
	
	if (key == 'T') {

		RenderMode = RenderPolygonMode::E_TRIS;
	}
	if (key == 'P') {

		RenderMode = RenderPolygonMode::E_POINTS;
	}
	if (key == 'E') {

		RenderMode = RenderPolygonMode::E_EDGES;
	}
	if (key == 'x') {
	
		//currentScene = nullptr;
	}
	if (Engine::currentScene)Engine::currentScene->KeyboardEvent(key,x,y);
}

void Engine::MouseEvent(int button, int state, int x, int y)
{
	if (Engine::currentScene)Engine::currentScene->MouseEvent(button,state, x, y);
}

void Engine::MouseMotionEvent(int x, int y)
{
	if (Engine::currentScene)Engine::currentScene->MouseMotionEvent( x, y);
}

void Engine::DisplayEvent()
{
	static float timeBase = 0;
	static int frames = 0;

	float currenTime = glutGet(GLUT_ELAPSED_TIME);
	
	DeltaTime = currenTime- LastTime  ;
	LastTime = currenTime;

	if (Engine::currentScene)currentScene->setDeltaTime(DeltaTime/1000.0f);
	if (Engine::currentScene)Engine::currentScene->DisplayEvent();

	frames++;
	if (currenTime - timeBase > 1000) {
		double fps = (frames * 1000.0 / (currenTime - timeBase));
		std::string title = "Render Engine  /  FPS: " + std::to_string(fps);
		glutSetWindowTitle(title.data());
		timeBase = currenTime;
		frames = 0;
	}
	glutSwapBuffers();
}

void Engine::startclock()
{
	Engine::beginTime = std::chrono::steady_clock::now();;
}

void Engine::endClock(std::string text,int render)
{
	Engine::endTime = std::chrono::steady_clock::now();
	auto v = std::chrono::duration_cast<std::chrono::milliseconds>(Engine::endTime - Engine::beginTime);
	std::cout << std::fixed;
	std::cout << std::setprecision(10);

	
	if (render==1) {
		Engine::totalRenderTime += v.count();
		Engine::totalIterations +=1;

		if (totalIterations == 1000) {
			
			std::ofstream myfile;
			myfile.open("..\Times.txt", std::ios::app);
			myfile << std::fixed;
			myfile << std::setprecision(10);
			myfile << "----\n";
			myfile << "AVERAGE COMPUTE TIME\t" << (Engine::totalComputeTime / Engine::totalIterations) << std::endl;
			myfile << "AVERAGE RENDER TIME\t" << (Engine::totalRenderTime / Engine::totalIterations) << std::endl;
			std::cout << "AVERAGE COMPUTE TIME\t" << (Engine::totalComputeTime / Engine::totalIterations) << std::endl;
			std::cout << "AVERAGE RENDER TIME\t" << (Engine::totalRenderTime / Engine::totalIterations) << std::endl;
			myfile.close();
			Engine::restartCounting();
		}
	}
	else {
		Engine::totalComputeTime += v.count();
	}
	
	
}

void Engine::restartCounting()
{
	Engine::totalRenderTime = 0;
	Engine::totalComputeTime = 0;
	Engine::totalIterations = 0;
}


void Engine::initContext(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitContextVersion(4, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	//Inicialización del contexto
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Render_Engine");

}


void Engine::initOGL() {


	//Initicialización de las extensiones
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}
	const GLubyte *oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;

	glutReshapeFunc(Engine::ResizeWindowEvent);
	glutDisplayFunc(Engine::DisplayEvent);
	glutIdleFunc(Engine::idleEvent);
	glutKeyboardFunc(Engine::KeyboardEvent);
	glutMouseFunc(Engine::MouseEvent);
	glutMotionFunc(Engine::MouseMotionEvent);



	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
		anisotropicSupported = true;

		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &MaxAnisotropy);
		std::cout << "supports max  anisotropic " << MaxAnisotropy << std::endl;
	}


}