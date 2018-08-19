
#include "..\HeaderFiles\Engine.h"
Engine* renderEngine;


int main(int argc, char** argv)
{
	

	renderEngine = new Engine();
	renderEngine->Start(argc, argv);
	return 0;
}
