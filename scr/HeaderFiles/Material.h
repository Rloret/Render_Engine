#pragma once

#include <gl/glew.h>

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "..\HeaderFiles\WaveParameters.h"
#include "Texture.h"
#include <vector>

class Shader;
class IRenderable;
class Transform;
class FBO;

/*
	This class holds a variety of properties of shaders. It is wrongly named material, since it was born like that, but now is a bridge class
	between the shaders and the objects, it holds the properties of the render and their uniform information, etc.

	Lots of types of materials can be seen, they were made for certain functionallity but now they are not used anymore
*/
class  Material
{
public:

	Material() {};

	virtual void Render(Shader*s) {};
	virtual void KeyboardEvent(unsigned char key) {};
	virtual void Resize(float width, float height) {};
	virtual void ResetState() {};
	virtual void Clear() {};


};

class PhongMaterial :public Material {
public:
	PhongMaterial(Texture* a, Texture*s, Texture*n, Texture* e);
	PhongMaterial(Texture* a, Texture*s, Texture*n, Texture* e, glm::vec3 kd, glm::vec3 ks, glm::vec3 ke, int N);
	PhongMaterial(glm::vec3 kd, glm::vec3 ks, glm::vec3 ke);
	void Render(Shader* s);
	void KeyboardEvent(unsigned char key);
private:
	Texture* Albedo;
	Texture* Specular;
	Texture* Normal;
	Texture* Emission;

	glm::vec3 kd;
	glm::vec3 ks;
	glm::vec3 ke;
	int N;
	int specularpow;



};

class OutlineMaterial :public Material {
public:
	OutlineMaterial(float outlineWidth, glm::vec3 Color);
	void Render(Shader* s);
	void KeyboardEvent(unsigned char key);

private:
	float outlineWidth;
	glm::vec3 outlineColor;
	float deltaFactor = 0.01f;


};

	class ToonMaterial : public OutlineMaterial {
	public:
		ToonMaterial(float outlineWidth, glm::vec3 outlineColor,
			glm::vec3 albedoColor, glm::vec3 specularColor,
			float albedoSteps, float specularSteps);
		/*Press o to increase number Of albedo steps by one or press p to decrease
		*Also, press k to increase specular ones or l to decrease them
		*/

		void Render(Shader* s);
		void KeyboardEvent(unsigned char key);
	private:
		glm::vec3 albedoColor;
		glm::vec3 specularColor;
		float albedoSteps;
		float specularSteps;
		int specular = 16;

	};

class PhongOutline : public PhongMaterial {
public:
	PhongOutline(Texture* a, Texture*s, Texture*n, Texture* e, glm::vec3 color, float width);

	PhongOutline(Texture* a, Texture*s, Texture*n, Texture* e, glm::vec3 kd, glm::vec3 ks, glm::vec3 ke, int N, glm::vec3 color, float width);
	PhongOutline(glm::vec3 kd, glm::vec3 ks, glm::vec3 ke, glm::vec3 color, float width);

	void Render(Shader* s);
	void KeyboardEvent(unsigned char key);
private:

	float lineWidth;
	glm::vec3 lineColor;

};

class NormalWireFrame : public Material {
public:
	NormalWireFrame(float normalWidth);

	void Render(Shader* s);
	void KeyboardEvent(unsigned char key);
private:
	float width;

};

class TrisTesselationLevels : public Material {
public:
	TrisTesselationLevels(glm::vec3 outer, float inner);

	void Render(Shader* s);
	void KeyboardEvent(unsigned char key);
private:
	glm::vec3 outer;
	float inner;
	int tessModifierIndex = -1;

};

class BounceComputeMaterial : public Material {
public:
	BounceComputeMaterial(Transform* Sphere, float cubeDims, float lifetime, float kd);

	void Render(Shader* s);
	void KeyboardEvent(unsigned char key);
private:
	float cubeDimensions;
	float kd;
	float lifetime;
	Transform* Sphere;

};

class AttractorsMovingComputeMaterial : public Material {
public:
	AttractorsMovingComputeMaterial(Transform* Sphere, std::vector<Transform*> attractors, float cubeDims, float lifetime, float kd);

	void Render(Shader* s);
	void KeyboardEvent(unsigned char key);
private:
	float cubeDimensions;
	float kd;
	float lifetime;
	float radius = 3.f;
	float attractionForce = 7.f;
	bool move = false;
	int numberOfActiveAttractors = 0;
	Transform* Sphere;
	std::vector<Transform*> Attractors;
	std::vector<glm::vec3> iniPos;
	GLuint attractorBuffer;

};
class ParticlesRenderMaterial : public Material {
public:
	ParticlesRenderMaterial(glm::vec4 cA, glm::vec4 cB, bool glow);

	void Render(Shader* s);
private:
	glm::vec4 colorA;
	glm::vec4 colorB;
	bool glow;



};

class PerlinMaterial : public Material {
private:
	float Octaves;
	float Amplitude = 2;
	float Frequency = 0.5;

	bool LOD = false;

	glm::vec2 FDMaxStep;

	Texture* terrainText;
	FBO* terrainTextures;
	bool blends;

public:
	PerlinMaterial(float Octaves, float  NoiseHeight, float NoiseScale, Texture* colorGradient);
	PerlinMaterial(float Octaves, float  NoiseHeight, float NoiseScale, Texture* colorGradient,FBO*terrainTextures,bool blends);
	void ResetState();
	void Render(Shader*s);
	void KeyboardEvent(unsigned char key);
	float getMaxHeight();
};


class PostProccessMaterial : public Material {
private:
	FBO* waterBuffer;
	FBO* terrainBuffer;
	Texture* Foam;
	Texture* Skybox;

public:
	PostProccessMaterial(FBO * water_fbo, FBO* terrain_fbo, Texture* Water_Foam, Texture* Skybox);
	void Render(Shader*s);
	void KeyboardEvent(unsigned char key);
	void Clear();
};


class GerstnerWavesMaterial : public Material {

private:
	WaveParameters initialWaveParams;
	std::vector<WaveParameters> Waves;

	int numWaves;
	int initialWaves;
	int activeWave;
	int initialRandom;
	bool parentWave = false;




public:
	GerstnerWavesMaterial(std::vector<WaveParameters> Waves, int numWaves);
	GerstnerWavesMaterial(WaveParameters InitialWave, int numWaves,float randomAngle);

	void Render(Shader*s);
	void KeyboardEvent(unsigned char key);
	void resetWaves();



};

class RenderToFBOMaterial : public Material {

private:
	FBO* fbo;
	bool clears;

public:
	RenderToFBOMaterial(FBO* fbo);
		RenderToFBOMaterial(FBO* fbo,bool clears);
	void Render(Shader*s);
	void KeyboardEvent(unsigned char key);
	void Resize(float width, float height);
	void ResetState();
	void Clear();

};


class SkyboxMaterial : public Material {

private:
	Texture* skybox;


public:
	SkyboxMaterial(Texture* skybox);
	void Render(Shader*s);
	void KeyboardEvent(unsigned char key);
	void Resize(float width, float height);
	void ResetState();
	void Clear();

};

class GridMaterial : public Material {

private:
	float tessellation;


public:
	GridMaterial(float maxTessellation);
	void Render(Shader*s);


};

class ReadFromFBOMaterial : public Material {
private:
	FBO* fbo;


public:
	ReadFromFBOMaterial(FBO* fbo);
	void Render(Shader*s);
	void KeyboardEvent(unsigned char key);
	void Clear();
};


class DOFMaterial : public Material {
private:

	float focalDistance = -25.0;
	float maxDistanceFactor = 1.0 / 20.0;
	float maskFactor = 1 / 65.f;
	float blurMask[25] = {
		1.0f*maskFactor, 2.0f*maskFactor, 3.0f*maskFactor,2.0f*maskFactor, 1.0f*maskFactor,
		2.0f*maskFactor, 3.0f*maskFactor, 4.0f*maskFactor,3.0f*maskFactor, 2.0f*maskFactor,
		3.0f*maskFactor, 4.0f*maskFactor, 5.0f*maskFactor,4.0f*maskFactor, 3.0f*maskFactor,
		2.0f*maskFactor, 3.0f*maskFactor, 4.0f*maskFactor,3.0f*maskFactor, 2.0f*maskFactor,
		1.0f*maskFactor, 2.0f*maskFactor, 3.0f*maskFactor,2.0f*maskFactor, 1.0f*maskFactor };



public:
	DOFMaterial();
	void Render(Shader*s);
	void KeyboardEvent(unsigned char key);
	void Clear();
};