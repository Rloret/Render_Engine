#include "..\HeaderFiles\modelobject.h"


#include "..\HeaderFiles\GlobalVariables.h"
#include "..\HeaderFiles\auxiliar.h"
#include "..\HeaderFiles\Uniform.h"
#include "..\HeaderFiles\Material.h"
#include "..\HeaderFiles\Transform.h"
#include "..\HeaderFiles\MeshData.h"
#include "..\HeaderFiles\Texture.h"
#include <gl/glew.h>




//If the mdoel does not have some of this argumentas pass 0 or nullptr.
ModelObject::ModelObject(MeshData * data, Transform* t,std::vector<Material*>&materials)
{
	this->mData = data;
	this->materials = materials;
	this->transform = t;
	vao = 0;
	this->patches = -1;
	registeredVBOs = std::vector<GLuint>();

}    


ModelObject::ModelObject(MeshData * data,Transform* t, std::vector<Material*>& materials, int numberOfPatches)
{
	this->mData = data;
	this->materials = materials;
	this->transform = t;
	vao = 0;
	this->patches = numberOfPatches;
	registeredVBOs = std::vector<GLuint>();
}


ModelObject::~ModelObject()
{
	for (int i = 0; i < registeredVBOs.size(); i++)
	{
		glDeleteBuffers(1, &registeredVBOs[i]);
	}

}



void ModelObject::LoadDataToGPU(Shader* s)
{


	glGenVertexArrays(1, &vao); 

	glBindVertexArray(vao); 
	
	int inColor =s->GetAttribute(GLOBAL_SHADER_ATR_VAR_INCOLOR);
	int inPos = s->GetAttribute( GLOBAL_SHADER_ATR_VAR_INPOS);
	int inTangent = s->GetAttribute( GLOBAL_SHADER_ATR_VAR_INTANGENT);
	int inNormal = s->GetAttribute( GLOBAL_SHADER_ATR_VAR_INNORMAL);
	int inTexCoord = s->GetAttribute( GLOBAL_SHADER_ATR_VAR_INTEXTURECOORDINATES);

	
	GLuint posVBO, colorVBO, normalVBO, tCoordVBO, tangentVBO, facesVBO;

	if (inPos != -1)
	{
		
		glGenBuffers(1, &posVBO);
		registeredVBOs.push_back(posVBO);
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		glBufferData(GL_ARRAY_BUFFER, mData->GetCoordinatesSize() * sizeof(float) * 3, &mData->GetCoordinates()[0] , GL_STATIC_DRAW);

		glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inPos); 
	}
	if (inColor != -1)
	{

		glGenBuffers(1, &colorVBO);
		registeredVBOs.push_back(colorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, mData->GetCoordinatesSize() * sizeof(float) * 3, &mData->GetColors()[0], GL_STATIC_DRAW);
		glVertexAttribPointer(inColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inColor);
	}
	if (inNormal != -1)
	{
		glGenBuffers(1, &normalVBO);
		registeredVBOs.push_back(normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, mData->GetCoordinatesSize() * sizeof(float) * 3, &mData->GetNormals()[0], GL_STATIC_DRAW);
		glVertexAttribPointer(inNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inNormal);
	}
	if (inTexCoord != -1)
	{
		glGenBuffers(1, &tCoordVBO);
		registeredVBOs.push_back(tCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, tCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, mData->GetCoordinatesSize() * sizeof(float) * 2,&mData->GetTCoords()[0], GL_STATIC_DRAW);
		glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTexCoord);
	}
	if (inTangent != -1)
	{
		glGenBuffers(1, &tangentVBO);
		registeredVBOs.push_back(tangentVBO);
		glBindBuffer(GL_ARRAY_BUFFER, tangentVBO);
		glBufferData(GL_ARRAY_BUFFER, mData->GetCoordinatesSize() * sizeof(float) * 3,&mData->GetTangents()[0], GL_STATIC_DRAW);
		glVertexAttribPointer(inTangent, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTangent);
	}

	glGenBuffers(1, &facesVBO);
	registeredVBOs.push_back(facesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, facesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mData->GetFacesSize() * sizeof(unsigned int) * 3, &mData->GetFaces()[0], GL_STATIC_DRAW);
	
	glBindVertexArray(0);
	//initTextures();


}




void ModelObject::Render(glm::mat4& view, glm::mat4& proj,Shader* s)
{

	for (Material* m : materials) {
		m->Render(s);
	}

	Uniform* uNormalMat = s->GetUniform( GLOBAL_SHADER_UNIF_VAR_NORMALMAT);
	Uniform* uModelViewMat = s->GetUniform( GLOBAL_SHADER_UNIF_VAR_MVMAT);
	Uniform* uModelViewProjMat = s->GetUniform( GLOBAL_SHADER_UNIF_VAR_MVPMAT);
	Uniform* uModel = s->GetUniform("Model");

	
	glBindVertexArray(vao);

	glm::mat4 modelView = glm::mat4(1.0f);
	glm::mat4 model = this->transform->GetModel();
	modelView = view  *model;
	glm::mat4 modelViewProj = proj * view * model;
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));

	if (uModelViewMat != nullptr)
		((uMatrix4Float*)uModelViewMat)->Set(modelView, 0);
	if (uModelViewProjMat != nullptr) {
		 ((uMatrix4Float*)uModelViewProjMat)->Set(modelViewProj, 0);
	}
	if (uNormalMat != nullptr)
		((uMatrix4Float*)uNormalMat)->Set(normal, 0);
	if(uModel)((uMatrix4Float*)uModel)->Set(model, 0);

	
	if (patches == -1) {
		glDrawElements(GL_TRIANGLES, mData->GetFacesSize() * 3, GL_UNSIGNED_INT, (void*)0);
	}
	else {
		glPatchParameteri(GL_PATCH_VERTICES, patches);
		glDrawElements(GL_PATCHES, mData->GetFacesSize() * 3, GL_UNSIGNED_INT, (void*)0);
	}
	glBindVertexArray(0);

	for (Material* m : materials) {
		m->ResetState();
	}


}


