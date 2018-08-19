#ifndef __PLANEFILE__
#define __PLANEFILE__

//Describimos un plano en coordenadas normalizadas
//*************************************************

//Número de vértices
 int planeNVertex = 4; // 4 vértices 
 int planeNFaces = 2; // 4 vértices 

//Posicíon de los vertices
 float planeVertexPos[] = { 
	//Cara z = 1
	-1.0f,	-1.0f,	 0.0f, 
	 1.0f,	-1.0f,	 0.0f, 
	-1.0f,	 1.0f,	 0.0f, 
	 1.0f,	 1.0f,	 0.0f, 
 };

unsigned int planeFacesOrder[] = {0,1,2,2,1,3};

#endif