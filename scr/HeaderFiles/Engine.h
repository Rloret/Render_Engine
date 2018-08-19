#pragma once
#include "Scene.h"
#include "..\HeaderFiles\auxiliar.h"
#include <ctime>
#include <chrono>
class Engine {
public:
	Engine();


	void Start(int argc, char** argv);
	float MaxAnisotropy;
	static std::chrono::steady_clock::time_point beginTime;
	static  std::chrono::steady_clock::time_point endTime;
	static double totalRenderTime;
	static double totalComputeTime;

	static long int totalIterations;

	static Scene* currentScene;
	static ModelObject*  movable;
	static std::vector<Scene*> Scenes;
	static float DeltaTime;
	static float LastTime;



	static RenderPolygonMode RenderMode;

	static void ResizeWindowEvent(int width, int height);
	static void idleEvent();
	static void KeyboardEvent(unsigned char key, int x, int y);
	static void MouseEvent(int button, int state, int x, int y);
	static void MouseMotionEvent(int x, int y);
	static void DisplayEvent();
	static void startclock();
	static void endClock(std::string text,int render);
	static void restartCounting();

private:

	bool anisotropicSupported;



	void initContext(int argc, char** argv);
	void Engine::initOGL();

	
};