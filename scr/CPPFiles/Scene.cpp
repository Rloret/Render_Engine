#include "..\HeaderFiles\Scene.h"

#include "..\HeaderFiles\AssimpLoader.h"
#include "..\HeaderFiles\Uniform.h"

#include "..\HeaderFiles\Material.h"
#include "..\HeaderFiles\ParticleSystem.h"
#include <cmath>


void Scene::AddProgram(Shader* s)
{
	auto finder = std::find(sceneShaders.begin(), sceneShaders.end(), s);
	if (finder == sceneShaders.end()) {
		sceneShaders.push_back(s);
	}

}


void Scene::AddMaterial(Material * m)
{
	sceneMaterials.push_back(m);
}

void  Scene::AddModelObject(int shader, IRenderable * model)
{
	while (sceneObjects.size()<shader + 1) {
		sceneObjects.push_back(std::vector<IRenderable*>());
	}

	sceneObjects[shader].push_back(model);
	
}





void Scene::AddParticleSystem(ParticleSystem* system, int computeShader, int renderShader)
{

	while (sceneObjects.size()< fmax(computeShader,renderShader) + 1) {
		sceneObjects.push_back(std::vector<IRenderable*>());
	}
	sceneObjects[computeShader].push_back(system);
	sceneObjects[renderShader].push_back(system);

}




void Scene::AddLight(glm::vec4& P, glm::vec4& D, glm::vec3& I, glm::vec3& att,float angle, int id)
{
	sceneLights.push_back(new Light(P, D, I, att,angle, id));
}



Camera* Scene::InitSceneCamera(glm::vec3 & cop, glm::vec2 windowSize)
{
	sceneCamera->setCOP(cop); sceneCamera->ResizeEvent(windowSize.x,windowSize.y);
	return sceneCamera;
}

void Scene::InitSceneObjects()
{
	for (int i = 0; i < sceneObjects.size(); i++) {
		
		for (int j = 0; j < sceneObjects[i].size(); j++)
		{
			sceneObjects[i][j]->LoadDataToGPU(sceneShaders[i]);
		}

	}

}

void Scene::SetLightBounds(int min, int max)
{
	lightsBounds = std::pair<int, int>(min, max);
}

Scene::Scene()
{

	sceneMeshes = std::unordered_map<std::string, MeshData*>();
	sceneShaders = std::vector <Shader*>();
	sceneObjects = std::vector<std::vector<IRenderable*>>();
	sceneCamera = new Camera();
	sceneLights = std::vector<Light*>();
	sceneTextures = std::vector<Texture*>();

	ambientLight = 0.5f;
	
	
	
}


void Scene::ResizeWindowEvent(int width, int height)
{
	
	for (size_t i = 0; i <sceneMaterials.size(); i++)
	{
		sceneMaterials[i]->Resize(width, height);
	}
	sceneCamera->ResizeEvent(width, height);
}

void Scene::IdleEvent()
{
	if (!sceneCamera->freeMovement) sceneCamera->UpdateCamera();
}

void Scene::KeyboardEvent(unsigned char key, int x, int y)
{
	sceneCamera->KeyBoardEvent(key,x, y,deltatime);
	for (int i = 0; i < sceneMaterials.size(); i++)
	{
		sceneMaterials[i]->KeyboardEvent(key);
	}

	if (key == 'n' &&ambientLight>0) {
		ambientLight -= 0.01;
	}
	else if (key == 'm' &&ambientLight<1) {
		ambientLight += 0.01;
	}
}

void Scene::MouseEvent(int button, int state, int x, int y)
{
	sceneCamera->MouseEvent(x, y);
}

void Scene::MouseMotionEvent(int x, int y)
{
	sceneCamera->MouseMotionEvent(x, y);
}

void Scene::DisplayEvent()
{

	for (size_t i = 0; i <sceneMaterials.size(); i++)
	{
		sceneMaterials[i]->Clear();
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	time += deltatime;
	Shader* s;
	Uniform* u;
	//the user might have added more shaders than he is using, the only thing 
	//we can do to make sure we render in the correct order is iterate over the matrix of objects
	for (int i = 0; i < sceneObjects.size(); i++) {
		s = sceneShaders[i];
		
		s->UseProgram();
		if (s->BeingUsed()) {

			u = s->GetUniform(GLOBAL_SHADER_UNIF_VAR_MAXLIGHTS);
			if (u != nullptr)((uInt*)u)->Set(lightsBounds.second);
			u = s->GetUniform(GLOBAL_SHADER_UNIF_VAR_MINLIGHTS);
			if (u != nullptr)((uInt*)u)->Set(lightsBounds.first);
			u = s->GetUniform("ambientLightModifier");
			if (u != nullptr)((uFloat*)u)->Set(ambientLight);
			u = s->GetUniform("Proj");
			if (u != nullptr)((uMatrix4Float*)u)->Set(sceneCamera->get_Projection(), false);
			u = s->GetUniform("View");
			if (u != nullptr)((uMatrix4Float*)u)->Set(sceneCamera->get_View(), false);
			u = s->GetUniform("View_Inv");
			if (u != nullptr)((uMatrix4Float*)u)->Set(sceneCamera->get_ViewInverse(), false);
			u = s->GetUniform("viewportSize");
			if (u != nullptr)((u2Float*)u)->Set(sceneCamera->get_windowSize());
			u = s->GetUniform("near_Plane");
			if (u != nullptr)((uFloat*)u)->Set(sceneCamera->get_NearPlane());
			u = s->GetUniform("far_Plane");
			if (u != nullptr)((uFloat*)u)->Set(sceneCamera->get_FarPlane());
			u = s->GetUniform("CamPos");
			if (u != nullptr)((u3Float*)u)->Set(sceneCamera->get_Position());


			for (int k = lightsBounds.first; k < lightsBounds.second; k++)
			{
				sceneLights[k]->Render(sceneCamera->get_View(), s);
			}
			for (int j = 0; j < sceneObjects[i].size(); j++)
			{
				sceneObjects[i][j]->Render(sceneCamera->get_View(), sceneCamera->get_Projection(), s);
			}

			s->setUsed(false);
		}
	}




}

void Scene::setDeltaTime(float time)
{
	this->deltatime = time;
}


