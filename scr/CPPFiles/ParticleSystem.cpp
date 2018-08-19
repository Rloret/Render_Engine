#include "..\HeaderFiles\ParticleSystem.h"
#include "..\HeaderFiles\Shader.h"
#include "..\HeaderFiles\Uniform.h"
#include "..\HeaderFiles\Material.h"
#include "..\HeaderFiles\GlobalVariables.h"
#include "..\HeaderFiles\Transform.h"
#include "..\HeaderFiles\Engine.h"
#include <iostream>
#include <cmath>




ParticleSystem::ParticleSystem(int numParticles, int workgroups, glm::vec3 * initialVelocity,float emissionSize, std::vector<Material*>& materials, Transform * t)
{

	numberOfParticles = numParticles;
	this->materials = materials;
	this->transform = t;
	velocity = initialVelocity;
	this->emissionSize = emissionSize;
	buffers[0] = -1;
	buffers[1] = -1;
	vao = -1;
	WG = workgroups;
}

void ParticleSystem::LoadDataToGPU(Shader* s)
{

	renderShader = s;
	if (vao == -1) glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	int inParticlePos = s->GetAttribute("inParticlePos");
	int inParticleVel = s->GetAttribute("inParticleVel");



		glGenBuffers(1, &buffers[0]);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * numberOfParticles, NULL, GL_DYNAMIC_COPY);


		glm::vec4* mappedPositionsData = reinterpret_cast<glm::vec4*>(glMapBufferRange(GL_ARRAY_BUFFER, 0,
			numberOfParticles * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

		float r;
		glm::vec3 position = this->transform->GetPosition();
		for (int i = 0; i < numberOfParticles; i++)
		{
			r =  1.0f*(rand()) / (RAND_MAX);
			r= (r*2.0f - 1.0f);
			mappedPositionsData[i].x =r*emissionSize+position.x;
			r = 1.0f*(rand()) / (RAND_MAX);
			r = (r*2.0f - 1.0f);
			mappedPositionsData[i].y = r*emissionSize + position.y;
			mappedPositionsData[i].w = 2*(r+1.0);
			r = 1.0f*(rand()) / (RAND_MAX);
			r = (r*2.0f - 1.0f);
			mappedPositionsData[i].z = r*emissionSize + position.z;
			
			
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
	if (inParticlePos != -1) {
		glVertexAttribPointer(inParticlePos, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inParticlePos);
	}


		glGenBuffers(1, &buffers[1]);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * numberOfParticles, NULL, GL_DYNAMIC_COPY);
		
		glm::vec4* mappedVelocitiesData = reinterpret_cast<glm::vec4*>(glMapBufferRange(GL_ARRAY_BUFFER, 0,
			numberOfParticles * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));


		for (int i = 0; i < numberOfParticles; i++)
		{
			if (velocity==nullptr) {
				r = 1.0f*(rand()) / (RAND_MAX);
				r = (r*2.0f - 1.0f);
				mappedVelocitiesData[i].x = r;
				r = 1.0f*(rand()) / (RAND_MAX);
				r = (r*2.0f - 1.0f);
				mappedVelocitiesData[i].y = r;
				r = 1.0f*(rand()) / (RAND_MAX);
				r = (r*2.0f - 1.0f);
				mappedVelocitiesData[i].z = r;
			}
			else
			{
				mappedVelocitiesData[i].x = velocity->x;
				mappedVelocitiesData[i].y = velocity->y;
				mappedVelocitiesData[i].z = velocity->z;
			}
			mappedVelocitiesData[i].w = 0.0;

		}
		
		glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindVertexArray(0);
}

void ParticleSystem::Render(glm::mat4 & view, glm::mat4 & proj, Shader * s)
{

	if (s != renderShader) {
		Engine::startclock();
		//glUseProgram(0);
		for (Material* m : materials) {
			m->Render(s);
		}
		
		auto u = s->GetUniform("emisorPosition");
		if (u) {
        			((u3Float*)u)->Set(transform->GetPosition());
		}
		u = s->GetUniform("initialVelocity");
		if (u != nullptr) {
			glm::vec4 iniv = glm::vec4(0);
			if (velocity) {
				iniv.x = velocity->x;
				iniv.y = velocity->y;
				iniv.z = velocity->z;
				iniv.w = 1;
		}
			((u4Float*)u)->Set(iniv);
		}
		
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, buffers[0]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, buffers[1]);

		glDispatchCompute(numberOfParticles / 128  , 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		Engine::endClock("SHADER DE COMPUTO HA TARDADO",0);

	}
	else
	{
		Engine::startclock();
		for (Material* m : materials) {
			m->Render(s);
		}
		glBindVertexArray(vao);

		glm::mat4 VP = proj * view ;
		auto u =s->GetUniform(GLOBAL_SHADER_UNIF_VAR_VP);
		if (u)((uMatrix4Float*)u)->Set(VP,0);
		glPointSize(3.0);

		glDrawArrays(GL_POINTS, 0, numberOfParticles);
		glDisable(GL_BLEND);
		Engine::endClock("RENDER HA TARDADO",1);

	}
}
