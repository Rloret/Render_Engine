#include "..\HeaderFiles\Material.h"
#include "..\HeaderFiles\GlobalVariables.h"
#include "..\HeaderFiles\Shader.h"
#include "..\HeaderFiles\Uniform.h"
#include "..\HeaderFiles\Transform.h"
#include "..\HeaderFiles\Engine.h"
#include "..\HeaderFiles\MeshData.h"
#include "..\HeaderFiles\FBO.h"
#include "..\HeaderFiles\Curve.h"
#include <iostream>
#include <string>



PhongMaterial::PhongMaterial(Texture * a, Texture * s, Texture * n, Texture * e) : Material()
{
	Albedo = a;
	Specular = s;
	Normal = n;
	Emission = e;
	this->kd = glm::vec3(0);
	this->ks = glm::vec3(0);
	this->ke = glm::vec3(0);
	this->N = 0;
	specularpow = 5;
}

PhongMaterial::PhongMaterial(Texture * a, Texture * s, Texture * n, Texture * e, glm::vec3 kd, glm::vec3 ks, glm::vec3 Ke, int N)
{
	Albedo = a;
	Specular = s;
	Normal = n;
	Emission = e;
	this->kd = kd;
	this->ks = ks;
	this->ke = ke;
	this->N = 1;
	specularpow = 5;
}

PhongMaterial::PhongMaterial(glm::vec3 kd, glm::vec3 ks, glm::vec3 ke)
{
	Albedo = nullptr;
	Specular = nullptr;
	Normal = nullptr;
	Emission = nullptr;
	this->kd = kd;
	this->ks = ks;
	this->ke = ke;
	this->N = 1;

	specularpow = 5;
}

void PhongMaterial::Render(Shader * s)
{
	Uniform* uNormalMat = s->GetUniform(GLOBAL_SHADER_UNIF_VAR_NORMALMAT);
	Uniform* uModelViewMat = s->GetUniform(GLOBAL_SHADER_UNIF_VAR_MVMAT);
	Uniform* uModelViewProjMat = s->GetUniform(GLOBAL_SHADER_UNIF_VAR_MVPMAT);

	Uniform* uColorTex = s->GetUniform(GLOBAL_SHADER_UNIF_VAR_COLOR_TEXTURE);
	Uniform* uEmiTex = s->GetUniform(GLOBAL_SHADER_UNIF_VAR_EMISSION_TEXTURE);
	Uniform* uSpecTex = s->GetUniform(GLOBAL_SHADER_UNIF_VAR_SPECULAR_TEXTURE);
	Uniform* uNormTex = s->GetUniform(GLOBAL_SHADER_UNIF_VAR_NORMAL_TEXTURE);

	Uniform * ukdmod = s->GetUniform("kdModifier");
	Uniform * uksmod = s->GetUniform("ksModifier");
	Uniform * ukemod = s->GetUniform("keModifier");
	Uniform * unmod = s->GetUniform("normalModifier");

	if (Albedo != nullptr && uColorTex != nullptr)
	{

		((uInt*)uColorTex)->Set(GLOBAL_SHADER_UNIF_VAR_COLOR_TEXTURE_UNIT);

		glActiveTexture(GL_TEXTURE0 + GLOBAL_SHADER_UNIF_VAR_COLOR_TEXTURE_UNIT);
		glBindTexture(GL_TEXTURE_2D, Albedo->GetId());
	}
	//we also upload this values even if they are not used, since otherwhise the GPU will take the ones already in memory
	//from other models. we do not want that do we?

	if (ukdmod) ((u3Float*)ukdmod)->Set(kd);

	if (Emission != nullptr && uEmiTex != nullptr) {
		((uInt*)uEmiTex)->Set(GLOBAL_SHADER_UNIF_VAR_EMISSION_TEXTURE_UNIT);
		glActiveTexture(GL_TEXTURE0 + GLOBAL_SHADER_UNIF_VAR_EMISSION_TEXTURE_UNIT);
		glBindTexture(GL_TEXTURE_2D, Emission->GetId());
	}
	if (ukemod) ((u3Float*)ukemod)->Set(ke);

	if (Specular != nullptr && uSpecTex != nullptr) {

		((uInt*)uSpecTex)->Set(GLOBAL_SHADER_UNIF_VAR_SPECULAR_TEXTURE_UNIT);
		glActiveTexture(GL_TEXTURE0 + GLOBAL_SHADER_UNIF_VAR_SPECULAR_TEXTURE_UNIT);
		glBindTexture(GL_TEXTURE_2D, Specular->GetId());

	}
	if (uksmod) ((u3Float*)uksmod)->Set(ks);

	if (Normal != nullptr && uNormTex != nullptr) {
		((uInt*)uNormTex)->Set(GLOBAL_SHADER_UNIF_VAR_NORMAL_TEXTURE_UNIT);
		glActiveTexture(GL_TEXTURE0 + GLOBAL_SHADER_UNIF_VAR_NORMAL_TEXTURE_UNIT);
		glBindTexture(GL_TEXTURE_2D, Normal->GetId());
	}
	if (unmod) ((uFloat*)unmod)->Set((float)N);


	uInt*  specularPow = (uInt*)s->GetUniform("n");
	if (specularPow != nullptr) specularPow->Set(specularpow);
}

void PhongMaterial::KeyboardEvent(unsigned char key)
{
	if (key == '9') {
		specularpow++;
	}
	else if ((key == '6') && specularpow>0) {
		specularpow--;

	}

}

OutlineMaterial::OutlineMaterial(float outlineWidth, glm::vec3 Color) :Material()
{
	this->outlineWidth = outlineWidth;
	this->outlineColor = Color;
}

void OutlineMaterial::Render(Shader * s)
{
	uFloat*  uoutlineWidth = (uFloat*)s->GetUniform(GLOBAL_SHADER_UNIF_VAR_OUTLINEWIDTH);
	if (uoutlineWidth != nullptr) uoutlineWidth->Set(outlineWidth);
	u3Float*  uoutlineColor = (u3Float*)s->GetUniform(GLOBAL_SHADER_UNIF_VAR_OUTLINECOLOR);
	if (uoutlineColor != nullptr)uoutlineColor->Set(outlineColor);
}

void OutlineMaterial::KeyboardEvent(unsigned char key)
{

	if (key == '-')outlineWidth -= deltaFactor;
	else if (key == '+')outlineWidth += deltaFactor;
	outlineWidth = outlineWidth > 1.0f ? 1.0f //higherbound
		: outlineWidth < 0.0f ? //is lower?
		0.0f : // lowerbound
		outlineWidth;//stay as it is
}

ToonMaterial::ToonMaterial(float outlineWidth, glm::vec3 outlineColor, glm::vec3 albedoColor, glm::vec3 specularColor, float albedoSteps, float specularSteps) : OutlineMaterial(outlineWidth, outlineColor)
{
	this->albedoColor = albedoColor;
	this->specularColor = specularColor;
	this->albedoSteps = albedoSteps;
	this->specularSteps = specularSteps;
	this->specular = 5;

}

void ToonMaterial::Render(Shader * s)
{
	OutlineMaterial::Render(s);

	uFloat*  ualSteps = (uFloat*)s->GetUniform(GLOBAL_SHADER_UNIF_VAR_TOONCOLORSTEP);
	if (ualSteps != nullptr)ualSteps->Set(albedoSteps);
	uFloat*  uspecSteps = (uFloat*)s->GetUniform(GLOBAL_SHADER_UNIF_VAR_TOONSPECULARSTEP);
	if (uspecSteps != nullptr)uspecSteps->Set(specularSteps);

	u3Float*  ual = (u3Float*)s->GetUniform(GLOBAL_SHADER_UNIF_VAR_TOONCOLOR);
	if (ual != nullptr)ual->Set(albedoColor);
	u3Float*  uspec = (u3Float*)s->GetUniform(GLOBAL_SHADER_UNIF_VAR_TOONSPECULAR);
	if (uspec != nullptr)uspec->Set(specularColor);

	uInt*  uspecular = (uInt*)s->GetUniform("n");
	if (uspecular != nullptr) uspecular->Set(specular);
}

void ToonMaterial::KeyboardEvent(unsigned char key)
{
	OutlineMaterial::KeyboardEvent(key);
	if (key == '8') {
		albedoSteps++;
	}
	else if ((key == '5') && albedoSteps>1) {
		albedoSteps--;
	}

	if (key == '7') {
		specularSteps++;
	}
	else if ((key == '4') && specularSteps>1) {
		specularSteps--;

	}

	if (key == '9') {
		specular++;

	}
	else if ((key == '6') && specular>0) {
		specular--;

	}


}

PhongOutline::PhongOutline(Texture * a, Texture * s, Texture * n, Texture * e, glm::vec3 color, float width) :PhongMaterial(a, s, n, e)
{
	lineWidth = width;
	lineColor = color;
}

PhongOutline::PhongOutline(Texture * a, Texture * s, Texture * n, Texture * e,
	glm::vec3 kd, glm::vec3 ks, glm::vec3 ke, int N,
	glm::vec3 color, float width) :PhongMaterial(a, s, n, e, kd, ks, ke, N)
{
	lineWidth = width;
	lineColor = color;
}

PhongOutline::PhongOutline(glm::vec3 kd, glm::vec3 ks, glm::vec3 ke, glm::vec3 color, float width) :
	PhongMaterial(kd, ks, ke)
{
	lineWidth = width;
	lineColor = color;
}

void PhongOutline::Render(Shader * s)
{
	PhongMaterial::Render(s);
	Uniform* outlineWidth = s->GetUniform("outlineWidth");
	if (outlineWidth)((uFloat*)outlineWidth)->Set(this->lineWidth);
	Uniform* outlineColor = s->GetUniform("outlineColor");
	if (outlineColor)((u3Float*)outlineColor)->Set(this->lineColor);

}

void PhongOutline::KeyboardEvent(unsigned char key)
{
	PhongMaterial::KeyboardEvent(key);
	if (key == '-') lineWidth -= 0.01f;
	else if (key == '+')lineWidth += 0.01f;
	lineWidth = lineWidth > 1.0f ? 1.0f //higherbound
		: lineWidth < 0.0f ? //is lower?
		0.0f : // lowerbound
		lineWidth;//stay as it is
}

NormalWireFrame::NormalWireFrame(float normalWidth)
{
	this->width = normalWidth;
}

void NormalWireFrame::Render(Shader * s)
{
	Uniform * uwidth = s->GetUniform("normalLength");
	if (uwidth)((uFloat*)uwidth)->Set(this->width);
}

void NormalWireFrame::KeyboardEvent(unsigned char key)
{
	if (key == '-') this->width -= 0.1f;
	else if (key == '+')this->width += 0.1f;
}

TrisTesselationLevels::TrisTesselationLevels(glm::vec3 outer, float inner)
{
	this->outer = outer;
	this->inner = inner;
}

void TrisTesselationLevels::Render(Shader * s)
{
	Uniform* uinner = s->GetUniform("TessLevelInner");
	if (uinner != nullptr) ((uFloat*)uinner)->Set(this->inner);
	Uniform* uouter = s->GetUniform("TessLevelOuter");
	if (uouter != nullptr) ((u3Float*)uouter)->Set(this->outer);
}

void TrisTesselationLevels::KeyboardEvent(unsigned char key)
{

	if (key == 'Q') {
		tessModifierIndex = 0;
	}
	else	if (key == 'W') {
		tessModifierIndex = 1;
	}
	else	if (key == 'E') {
		tessModifierIndex = 2;
	}
	else	if (key == 'R') {
		tessModifierIndex = 3;
	}

	if (tessModifierIndex != -1) {
		if (key == '.') {
			if (tessModifierIndex == 3) {
				inner++;
			}
			else {
				outer[tessModifierIndex]++;
			}
		}
		else if (key == ',') {
			if (tessModifierIndex == 3) {
				inner--;
				inner = inner < 1 ? 1 : inner;
			}
			else {
				outer[tessModifierIndex]--;
				outer[tessModifierIndex] = outer[tessModifierIndex]<1 ? outer[tessModifierIndex] : 0;
			}
		}
	}
}


BounceComputeMaterial::BounceComputeMaterial(Transform * Sphere, float cubeDims, float lifetime, float kd)
{
	this->cubeDimensions = cubeDims;
	this->kd = kd;
	this->Sphere = Sphere;
	this->lifetime = lifetime;
}

void BounceComputeMaterial::Render(Shader * s)
{
	Uniform* cubeDims = s->GetUniform("cubeDimensions");
	if (cubeDims != nullptr) ((uFloat*)cubeDims)->Set(this->cubeDimensions);
	Uniform* DT = s->GetUniform("DT");
	if (DT != nullptr) ((uFloat*)DT)->Set(-0.001 *Engine::DeltaTime);
	Uniform* kd = s->GetUniform("kd");
	if (kd != nullptr) ((uFloat*)kd)->Set(this->kd);
	Uniform* sphereDefinition = s->GetUniform("sphereDefinition");
	Uniform * u = s->GetUniform("LifeTime");
	if (u)((uFloat*)u)->Set(lifetime);
	if (sphereDefinition != nullptr) {
		glm::vec4 sphereDef = glm::vec4();
		glm::vec3 spos = this->Sphere->GetPosition();
		sphereDef.x = spos.x;
		sphereDef.y = spos.y;
		sphereDef.z = spos.z;
		sphereDef.w = this->Sphere->GetScale().x;
		((u4Float*)sphereDefinition)->Set(sphereDef);
	}

}

void BounceComputeMaterial::KeyboardEvent(unsigned char key)
{

	float deltaMov = 0.1;
	if (key == 'W') {
		this->Sphere->Translate(glm::vec3(0, 0, -1)* deltaMov);
	}
	else if (key == 'S') {
		this->Sphere->Translate(glm::vec3(0, 0, 1)* deltaMov);
	}
	else if (key == 'A') {
		this->Sphere->Translate(glm::vec3(-1, 0, 0)* deltaMov);
	}
	else if (key == 'D') {
		this->Sphere->Translate(glm::vec3(1, 0, 0)* deltaMov);
	}
	else if (key == 'Q') {
		this->Sphere->Translate(glm::vec3(0, -1, 0)* deltaMov);
	}
	else if (key == 'E') {
		this->Sphere->Translate(glm::vec3(0, 1, 0)* deltaMov);
	}
	else if (key == '+') {
		this->Sphere->Scale(glm::vec3(-1)* deltaMov);
	}
	else if (key == '-') {
		this->Sphere->Scale(glm::vec3(1)* deltaMov);
	}
	else if (key == '9') {

		cubeDimensions -= 1;
	}
	else if (key == '6') {
		cubeDimensions += 1;
	}

	else if (key == '8') {

		kd -= 0.1;
		kd = kd < 0 ? 0 : kd;
	}
	else if (key == '5') {
		kd += 0.1;
		kd = kd > 1 ? 1 : kd;

	}
}




AttractorsMovingComputeMaterial::AttractorsMovingComputeMaterial(Transform * Sphere, std::vector<Transform*> attractors, float cubeDims, float lifetime, float kd)
{
	this->cubeDimensions = cubeDims;
	this->kd = kd;
	this->Sphere = Sphere;
	this->lifetime = lifetime;

	Attractors = attractors;
	this->move = false;

	glGenBuffers(1, &attractorBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, attractorBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * attractors.size(), NULL, GL_DYNAMIC_COPY);
	glm::vec4* attractorsMapped = reinterpret_cast<glm::vec4*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0,
		attractors.size() * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

	float r;

	for (int i = 0; i < attractors.size(); i++)
	{
		float time = Engine::LastTime / 1000;
		iniPos.push_back(attractors[i]->GetPosition());
		glm::vec3 pos = radius*glm::vec3(sinf(time * (i + 1)*2.5f), cosf(time* (i + 4)),
			-cosf(time* (i + 1)) *2.5);
		attractors[i]->Rotate(glm::vec3(90, 0, 0));
		attractorsMapped[i] = glm::vec4(pos.x, pos.y, pos.z, rand() % 100 / 100.0f + 0.0001);

	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

}

void AttractorsMovingComputeMaterial::Render(Shader * s)
{

	Uniform* cubeDims = s->GetUniform("cubeDimensions");
	if (cubeDims != nullptr) ((uFloat*)cubeDims)->Set(cubeDimensions);
	Uniform* DT = s->GetUniform("DT");
	if (DT != nullptr) ((uFloat*)DT)->Set(-0.001 *Engine::DeltaTime);
	Uniform* kd = s->GetUniform("kd");
	if (kd != nullptr) ((uFloat*)kd)->Set(this->kd);
	Uniform* kG = s->GetUniform("kG");
	if (kG != nullptr) ((uFloat*)kG)->Set(this->attractionForce);
	Uniform* n = s->GetUniform("nAttractors");
	if (n != nullptr) ((uInt*)n)->Set(numberOfActiveAttractors);
	Uniform* sphereDefinition = s->GetUniform("sphereDefinition");
	Uniform * u = s->GetUniform("LifeTime");
	if (u)((uFloat*)u)->Set(lifetime);
	if (sphereDefinition != nullptr) {
		glm::vec4 sphereDef = glm::vec4();
		glm::vec3 spos = this->Sphere->GetPosition();
		sphereDef.x = spos.x;
		sphereDef.y = spos.y;
		sphereDef.z = spos.z;
		sphereDef.w = this->Sphere->GetScale().x;
		((u4Float*)sphereDefinition)->Set(sphereDef);
	}

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, attractorBuffer);
	glm::vec4* attractorsMapped = reinterpret_cast<glm::vec4*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0,
		Attractors.size() * sizeof(glm::vec4), GL_MAP_WRITE_BIT));

	float r;

	for (int i = 0; i < numberOfActiveAttractors; i++)
	{
		Attractors[i]->SetScale(glm::vec3(1));
		Attractors[i]->Rotate(glm::vec3(0, -rand() % 100 * Engine::DeltaTime*0.001, 0));
		glm::vec3 pos;
		if (move) {
			float time = Engine::LastTime / 1000;
			pos = radius*glm::vec3(sinf(time * (i + 1)*2.5f), cosf(time* (i + 4)),
				-cosf(time* (i + 1)) *2.5);
		}
		else
		{
			pos = iniPos[i];

		}
		Attractors[i]->SetTranslate(pos);
		attractorsMapped[i] = glm::vec4(pos.x, pos.y, pos.z, attractorsMapped[i].w);

	}
	for (int i = numberOfActiveAttractors; i < Attractors.size(); i++)
	{
		Attractors[i]->SetScale(glm::vec3(0));
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);


}

void AttractorsMovingComputeMaterial::KeyboardEvent(unsigned char key)
{

	float deltaMov = 0.1;


	if (key == '9') {

		cubeDimensions -= 1;
	}
	else if (key == '6') {
		cubeDimensions += 1;
	}
	else if (key == 'm') {
		std::cout << attractionForce << std::endl;
		attractionForce -= 1e-2;
		attractionForce = attractionForce < 0.01f ? 0.01f : attractionForce;

	}
	else if (key == 'n') {
		attractionForce += 1;

	}

	else if (key == '8') {

		kd -= 0.1;
		kd = kd < 0 ? 0 : kd;
	}
	else if (key == '5') {
		kd += 0.1;
		kd = kd > 1 ? 1 : kd;

	}

	else if (key == '7') {

		numberOfActiveAttractors -= 1;
		numberOfActiveAttractors = numberOfActiveAttractors < 1 ? 1 : numberOfActiveAttractors;
	}
	else if (key == '4') {
		numberOfActiveAttractors += 1;
		numberOfActiveAttractors = numberOfActiveAttractors >Attractors.size() - 1 ? Attractors.size() : numberOfActiveAttractors;

	}

	else if (key == ',') {

		radius -= 1;
		radius = radius < 1 ? 1 : radius;
	}
	else if (key == '.') {
		radius += 1;

	}
	else if (key == 'b') {
		this->move = !this->move;
	}
}

ParticlesRenderMaterial::ParticlesRenderMaterial(glm::vec4 cA, glm::vec4 cB, bool glow)
{
	this->colorA = cA;
	this->colorB = cB;
	this->glow = glow;
}

void ParticlesRenderMaterial::Render(Shader * s)
{
	Uniform* color1 = s->GetUniform("colorA");
	if (color1)((u4Float*)color1)->Set(colorA);

	Uniform* color2 = s->GetUniform("colorB");
	if (color2)((u4Float*)color2)->Set(colorB);


	if (glow) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
	}
	else {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}


}

PerlinMaterial::PerlinMaterial(float Octaves, float  NoiseHeight, float NoiseScale, Texture* terrainTexture)
{
	this->Octaves = Octaves;
	Amplitude = NoiseHeight;
	Frequency = NoiseScale;
	Curve::maxHeight = Amplitude*1.3f;
	this->terrainText = terrainTexture;
	terrainTextures = nullptr;
	blends = false;

}

PerlinMaterial::PerlinMaterial(float Octaves, float NoiseHeight, float NoiseScale, Texture * colorGradient, FBO * terrainTextures,bool blends)
{
	this->Octaves = Octaves;
	Amplitude = NoiseHeight;
	Frequency = NoiseScale;
	this->terrainText = colorGradient;
	this->terrainTextures = terrainTextures;
	this->blends = blends;
}

void PerlinMaterial::ResetState()
{
	glDisable(GL_BLEND);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PerlinMaterial::Render(Shader * s)
{
	if (blends) {
		glEnable(GL_BLEND);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_CONSTANT_ALPHA, GL_CONSTANT_ALPHA);
	}

	Uniform * uHeight = s->GetUniform("octaves");
	if (uHeight)((uFloat*)uHeight)->Set(Octaves);
	Uniform * uAmp = s->GetUniform("Amplitude");
	if (uAmp)((uFloat*)uAmp)->Set(Amplitude);

	Uniform * uFreq = s->GetUniform("Frequency");
	if (uFreq)((uFloat*)uFreq)->Set(Frequency);


	Uniform * uLOD = s->GetUniform("DynamicLOD");
	if (uLOD)((uInt*)uLOD)->Set((int)LOD);

	float time = Engine::LastTime / 1000;
	Uniform* uTime = s->GetUniform("time");
	if (uTime)((uFloat*)uTime)->Set(time);
	Uniform* uNormalStep = s->GetUniform("normalStep");
	if (uNormalStep)((u2Float*)uNormalStep)->Set(FDMaxStep);

	Uniform* uterrtex = s->GetUniform("terrainTexture");
	if (uterrtex) {
		((uInt*)uterrtex)->Set(0);

		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_1D, terrainText->GetId());
	}

	if (terrainTextures) {

		Uniform* uHeights = s->GetUniform("PerlinHeight");
		if (uHeights)
		{
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, terrainTextures->colorBuffer->GetId());
			((uInt*)uHeights)->Set(0);
		}

		Uniform* uNormals = s->GetUniform("PerlinNormal");
		if (uNormals)
		{
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, terrainTextures->normalBuffer->GetId());
			((uInt*)uNormals)->Set(1);
		}

	}


}

void PerlinMaterial::KeyboardEvent(unsigned char key)
{
	if (key == '6') {

		Octaves -= 1;
		Octaves = Octaves < 1 ? 1 : Octaves;
	}
	else if (key == '9') {
		Octaves += 1;
		Octaves = Octaves> 10 ? 10 : Octaves;
	}

	else if (key == '5') {

		Amplitude -= 0.1;
	}
	else if (key == '8') {
		Amplitude += 0.1;
		Curve::maxHeight = Amplitude*1.3f;

	}
	else if (key == '4') {
		Curve::maxHeight = Amplitude*1.3f;
		Frequency -= 0.01;

	}
	else if (key == '7') {
		Frequency += 0.01;

	}

	else if (key == 'b') {
		LOD = !LOD;
	}



}

float PerlinMaterial::getMaxHeight()
{
	return Amplitude;
}

PostProccessMaterial::PostProccessMaterial(FBO * water_fbo,FBO* terrain_fbo,Texture* Water_Foam,Texture* Skybox)
{
	Foam = Water_Foam;
	waterBuffer = water_fbo;
	terrainBuffer = terrain_fbo;
	this->Skybox = Skybox;
}

void PostProccessMaterial::Render(Shader * s)
{

	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	Uniform* uColor = s->GetUniform("Water_Foam");
	if (uColor && Foam)
	{
		glActiveTexture(GL_TEXTURE0+8);
		glBindTexture(GL_TEXTURE_2D, Foam->GetId());
		((uInt*)uColor)->Set(8);
	}

	if (waterBuffer) {
		
		Uniform* uUV = s->GetUniform("Water_UV");
		if (uUV)
		{
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, waterBuffer->uvBuffer->GetId());
			((uInt*)uUV)->Set(0);
		}

		Uniform* uVert = s->GetUniform("Water_Vertex");
		if (uVert)
		{
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, waterBuffer->vertexBuffer->GetId());
			((uInt*)uVert)->Set(1);
		}

		Uniform* uNorm = s->GetUniform("Water_Normal");
		if (uNorm)
		{
			glActiveTexture(GL_TEXTURE0 + 2);
			glBindTexture(GL_TEXTURE_2D, waterBuffer->normalBuffer->GetId());
			((uInt*)uNorm)->Set(2);
		}

		Uniform* uDepth = s->GetUniform("Water_Depth");
		if (uDepth)
		{
			glActiveTexture(GL_TEXTURE0 + 3);
			glBindTexture(GL_TEXTURE_2D, waterBuffer->depthBuffTexId);
			((uInt*)uDepth)->Set(3);
		}
	}
	if (terrainBuffer) {
		Uniform* uColor = s->GetUniform("Terrain_Color");
		if (uColor)
		{
			glActiveTexture(GL_TEXTURE0+4);
			glBindTexture(GL_TEXTURE_2D, terrainBuffer->colorBuffer->GetId());
			((uInt*)uColor)->Set(4);
		}

		Uniform* uVert = s->GetUniform("Terrain_Vertex");
		if (uVert)
		{
			glActiveTexture(GL_TEXTURE0 + 5);
			glBindTexture(GL_TEXTURE_2D, terrainBuffer->vertexBuffer->GetId());
			((uInt*)uVert)->Set(5);
		}

		Uniform* uNorm = s->GetUniform("Terrain_Normal");
		if (uNorm)
		{
			glActiveTexture(GL_TEXTURE0 + 6);
			glBindTexture(GL_TEXTURE_2D, terrainBuffer->normalBuffer->GetId());
			((uInt*)uNorm)->Set(6);
		}

		Uniform* uDepth = s->GetUniform("Terrain_Depth");
		if (uDepth)
		{
			glActiveTexture(GL_TEXTURE0 + 7);
			glBindTexture(GL_TEXTURE_2D, terrainBuffer->depthBuffTexId);
			((uInt*)uDepth)->Set(7);
		}
	}

	Uniform * u = s->GetUniform("skybox");
	if (u && Skybox) {

		glActiveTexture(GL_TEXTURE0+9);
		glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox->GetId());
		((uInt*)u)->Set(9);

	}

	float time = Engine::LastTime / 1000;
	Uniform* uTime = s->GetUniform("time");
	if (uTime)((uFloat*)uTime)->Set(time);



}

void PostProccessMaterial::KeyboardEvent(unsigned char key)
{
}

void PostProccessMaterial::Clear()
{
	if (waterBuffer) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, waterBuffer->GetId());
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	if (terrainBuffer) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, terrainBuffer->GetId());
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
}


GerstnerWavesMaterial::GerstnerWavesMaterial(std::vector<WaveParameters> Waves,int numWaves)
{
	this->Waves = Waves;

	this->numWaves = numWaves;

}

GerstnerWavesMaterial::GerstnerWavesMaterial(WaveParameters InitialWave, int numWaves,float randomAngle)
{
	this->initialWaveParams = InitialWave;
	this->Waves = std::vector<WaveParameters>();
	Waves.reserve(numWaves);
	this->numWaves = numWaves;
	this->initialWaves = numWaves;
	this->activeWave = 0;
	initialRandom = randomAngle;

	float L = InitialWave.L;
	float A = InitialWave.A;
	float p = 1.45;
#define rand ((double)rand() / (RAND_MAX))
	for (size_t i = 0; i < numWaves; i++)
	{
		WaveParameters pi = InitialWave;
		//float l= 0.6341798 + (21.76619 - 0.6341798) / (1 + powf((i / 1.639793), 5.566377));
		float l = L / (exp(i/L*1.2));
		float a = (p / (i + p))*A;
		pi.A = a;
		pi.L = l;

		pi.angle += ((rand * 2) - 1) * randomAngle * (rand>0.5?1:-1);
		
		Waves.push_back(pi);

	}
}



void GerstnerWavesMaterial::Render(Shader * s)
{

	for (size_t i = 0; i < numWaves; i++)
	{

		std::string u = "Waves[" + std::to_string(i) + "].A";
		Uniform* uA = s->GetUniform(u);
		if (uA) ((uFloat*)uA)->Set(Waves[i].A);
		u = "Waves[" + std::to_string(i) + "].L";
		Uniform* uLmod = s->GetUniform(u);
		if (uLmod) ((uFloat*)uLmod)->Set(Waves[i].L);

		u = "Waves[" + std::to_string(i) + "].angle";
		Uniform* uan= s->GetUniform(u);
		if (uan) ((uFloat*)uan)->Set(Waves[i].angle);
		
		u = "Waves[" + std::to_string(i) + "].Q";
		Uniform* uq = s->GetUniform(u);
		if (uq) ((uFloat*)uq)->Set(Waves[i].Q);

	}



	Uniform* nWaves = s->GetUniform("numWaves");
	if (nWaves) ((uFloat*)nWaves)->Set(numWaves);


	float time = Engine::LastTime / 1000;
	Uniform* uTime = s->GetUniform("time");
	if (uTime)((uFloat*)uTime)->Set(time);
}

void GerstnerWavesMaterial::KeyboardEvent(unsigned char key)
{
	if (key == '+') {
		activeWave += 1;

		if (activeWave>=numWaves)
		{
			activeWave = numWaves - 1;
		}
		std::cout << "wave number: " << activeWave << std::endl;

	}
	else if (key == '-') {
		activeWave -= 1;

		if (activeWave <0)
		{
			activeWave = 0;
		}
		std::cout << "wave number: " << activeWave << std::endl;

	}


	else if (key == 'H') {
		if (!parentWave)Waves[activeWave].A += 0.1;
		else {
			initialWaveParams.A += 0.1;
			resetWaves();
		}
	}

	else if (key == 'h') {
		if (!parentWave)Waves[activeWave].A -= 0.1;
		else {
			initialWaveParams.A -= 0.1;
			resetWaves();
		}
		
	}

	else if (key == 'L') {
		
		if (!parentWave)Waves[activeWave].L += 0.1;
		else {
			initialWaveParams.L += 0.1;
			resetWaves();
		}
	}

	else if (key == 'l') {
		if (!parentWave)Waves[activeWave].L -= 0.1;
		else {
			initialWaveParams.L -= 0.1;
			resetWaves();
		}
	}

	else if (key == 'Q') {
		if (!parentWave) {
			Waves[activeWave].Q += 0.1;
			if (Waves[activeWave].Q > 1)Waves[activeWave].Q = 1;
		}
		else {
			initialWaveParams.Q += 0.1;
			if (initialWaveParams.Q > 1)initialWaveParams.Q = 1;
			resetWaves();
		}
	}

	else if (key == 'q') {
		if (!parentWave) {
			Waves[activeWave].Q -= 0.1;
			if (Waves[activeWave].Q <0)Waves[activeWave].Q = 0;
		}
		else {
			initialWaveParams.Q -= 0.1;
			if (initialWaveParams.Q <0)initialWaveParams.Q = 0;
			resetWaves();
		}
	}

	else if (key == 'M') {
		numWaves += 1;
		numWaves = numWaves > initialWaves ? initialWaves : numWaves;
		std::cout << numWaves;
	}
	else if (key == 'm') {
		numWaves -= 1;
		numWaves = numWaves < 1 ? 1 : numWaves;
	}
	else if (key == 'r') {
		resetWaves();
	}

	else if (key == '*') {
		parentWave = !parentWave;
		std::cout << "parent wave :" << (parentWave ? " active" : "not active") << std::endl;
	}
}

void GerstnerWavesMaterial::resetWaves()
{
	WaveParameters InitialWave = initialWaveParams;
	float L = InitialWave.L;
	float A = InitialWave.A;
	float p = 1.45;
#define rand ((double)rand() / (RAND_MAX))
	for (size_t i = 0; i < numWaves; i++)
	{
		WaveParameters pi = Waves[i];
		//float l= 0.6341798 + (21.76619 - 0.6341798) / (1 + powf((i / 1.639793), 5.566377));
		float l = L / (exp(i / L*1.2));
		float a = (p / (i + p))*A;
		pi.A = a;
		pi.L = l;
		pi.Q = initialWaveParams.Q;
		//pi.angle += ((rand * 2) - 1) * initialRandom * (rand > 0.5 ? 1 : -1);

		Waves[i] = pi;
	}
}






RenderToFBOMaterial::RenderToFBOMaterial(FBO * fbo)
{
	this->fbo = fbo;
	clears = true;
}

RenderToFBOMaterial::RenderToFBOMaterial(FBO * fbo, bool clears)
{
	this->fbo = fbo;
	this->clears = clears;
}

void RenderToFBOMaterial::Render(Shader * s)
{
	if (fbo) {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo->GetId());
		
	}
}

void RenderToFBOMaterial::KeyboardEvent(unsigned char key)
{

}

void RenderToFBOMaterial::Resize(float width, float height)
{
	fbo->Resize(width, height);
}

void RenderToFBOMaterial::ResetState()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_CULL_FACE);
}

void RenderToFBOMaterial::Clear()
{
	if (fbo) {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo->GetId());
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}
}

SkyboxMaterial::SkyboxMaterial(Texture * skybox)
{
	this->skybox = skybox;
}

void SkyboxMaterial::Render(Shader * s)
{
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	Uniform * u =s->GetUniform("skybox");
	if (u) {

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP , skybox->GetId());
		((uInt*)u)->Set(0);

	}

}

void SkyboxMaterial::KeyboardEvent(unsigned char key)
{
}

void SkyboxMaterial::Resize(float width, float height)
{
}

void SkyboxMaterial::Clear()
{
	
}

void SkyboxMaterial::ResetState()
{
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
}

GridMaterial::GridMaterial(float maxTessellation)
{
	this->tessellation = maxTessellation;
}

void GridMaterial::Render(Shader * s)
{
	Uniform* u = s->GetUniform("maxT");
	if (u) ((uFloat*)u)->Set(tessellation);
}

ReadFromFBOMaterial::ReadFromFBOMaterial(FBO * fbo)
{
	this->fbo = fbo;
}

void ReadFromFBOMaterial::Render(Shader * s)
{
	
	if (fbo) {
		Uniform* uColor = s->GetUniform("Terrain_Color");
		if (uColor)
		{
			glActiveTexture(GL_TEXTURE0 + 4);
			glBindTexture(GL_TEXTURE_2D, fbo->colorBuffer->GetId());
			((uInt*)uColor)->Set(4);
		}

		Uniform* uVert = s->GetUniform("Terrain_Vertex");
		if (uVert)
		{
			glActiveTexture(GL_TEXTURE0 + 5);
			glBindTexture(GL_TEXTURE_2D, fbo->vertexBuffer->GetId());
			((uInt*)uVert)->Set(5);
		}

		Uniform* uNorm = s->GetUniform("Terrain_Normal");
		if (uNorm)
		{
			glActiveTexture(GL_TEXTURE0 + 6);
			glBindTexture(GL_TEXTURE_2D, fbo->normalBuffer->GetId());
			((uInt*)uNorm)->Set(6);
		}

		Uniform* uDepth = s->GetUniform("Terrain_Depth");
		if (uDepth)
		{
			glActiveTexture(GL_TEXTURE0 + 7);
			glBindTexture(GL_TEXTURE_2D, fbo->depthBuffTexId);
			((uInt*)uDepth)->Set(7);
		}
	}
}

void ReadFromFBOMaterial::KeyboardEvent(unsigned char key)
{
}

void ReadFromFBOMaterial::Clear()
{

	if (fbo) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->GetId());
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

}

DOFMaterial::DOFMaterial()
{

	focalDistance = -25.0;
	maxDistanceFactor = 1.0 / 5.0;
	maskFactor = 1 / 65.f;
	
}

void DOFMaterial::Render(Shader * s)
{
	/*Uniform* uMask = s->GetUniform("mask");
	if(uMask) ((u1Floatv*)uMask)->Set(25, &blurMask[0]);*/

	Uniform* ufocalD= s->GetUniform("focalDistance");
	if (ufocalD) ((uFloat*)ufocalD)->Set(focalDistance);

	Uniform* umaxDistanceFactor = s->GetUniform("maxDistanceFactor");
	if (umaxDistanceFactor) ((uFloat*)umaxDistanceFactor)->Set(this->maxDistanceFactor);

}

void DOFMaterial::KeyboardEvent(unsigned char key)
{
}

void DOFMaterial::Clear()
{
}
