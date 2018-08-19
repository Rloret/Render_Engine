#pragma once


/***
	This class was one of the first attepmts of shader variables and cpu variables to mathc, however it does not scale well, so 
	ad- hoc methods have been implemnted.
***/
	#define GLOBAL_MESHES_PATH "../Meshes/"




	#define GLOBAL_SHADER_ATR_VAR_INPOS   "inPos"
	#define GLOBAL_SHADER_ATR_VAR_INCOLOR   "inColor"
	#define GLOBAL_SHADER_ATR_VAR_INNORMAL   "inNormal"
	#define GLOBAL_SHADER_ATR_VAR_INTEXTURECOORDINATES   "inTexCoord"
	#define GLOBAL_SHADER_ATR_VAR_INTANGENT   "inTangent"

	#define GLOBAL_SHADER_UNIF_VAR_NORMALMAT   "normal"
	#define GLOBAL_SHADER_UNIF_VAR_MVMAT   "modelView"
	#define GLOBAL_SHADER_UNIF_VAR_VP "ViewProj"
	#define GLOBAL_SHADER_UNIF_VAR_MVPMAT   "modelViewProj"

	#define GLOBAL_SHADER_UNIF_VAR_TIME  "time"
	#define GLOBAL_SHADER_UNIF_VAR_OUTLINECOLOR  "outlineColor"
	#define GLOBAL_SHADER_UNIF_VAR_OUTLINEWIDTH  "outlineWidth"

	#define GLOBAL_SHADER_UNIF_VAR_TOONCOLOR "albedoColor"
	#define GLOBAL_SHADER_UNIF_VAR_TOONSPECULAR "specularColor"
	#define GLOBAL_SHADER_UNIF_VAR_TOONCOLORSTEP "albedoSteps"
	#define GLOBAL_SHADER_UNIF_VAR_TOONSPECULARSTEP "specularSteps"

	#define GLOBAL_SHADER_UNIF_VAR_MAXLIGHTS  "maxLights"
	#define GLOBAL_SHADER_UNIF_VAR_MINLIGHTS  "minLights"

	#define GLOBAL_SHADER_UNIF_VAR_COLOR_TEXTURE   "colorTex"
	#define GLOBAL_SHADER_UNIF_VAR_EMISSION_TEXTURE   "emiTex"
	#define GLOBAL_SHADER_UNIF_VAR_SPECULAR_TEXTURE   "specularTex"
	#define GLOBAL_SHADER_UNIF_VAR_NORMAL_TEXTURE   "normalTex"


	#define GLOBAL_SHADER_UNIF_VAR_COLOR_TEXTURE_UNIT   0
	#define GLOBAL_SHADER_UNIF_VAR_EMISSION_TEXTURE_UNIT   1
	#define GLOBAL_SHADER_UNIF_VAR_SPECULAR_TEXTURE_UNIT   2
	#define GLOBAL_SHADER_UNIF_VAR_NORMAL_TEXTURE_UNIT   3

	#define GLOBAL_SHADER_UNIF_VAR_TIME_LOCATION 7

	#define	GLOBAL_SHADER_LIGHTS_ARRAY_NAME "lights["
	#define GLOBAL_SHADER_UNIF_VAR_LIGHT_POSITION	"].lightPos"
	#define GLOBAL_SHADER_UNIF_VAR_LIGHT_DIRECTION "].direction"
	#define GLOBAL_SHADER_UNIF_VAR_LIGHT_iNTENSITY "].I"
	#define GLOBAL_SHADER_UNIF_VAR_LIGHT_ATTENUATION "].att"
	#define GLOBAL_SHADER_UNIF_VAR_LIGHT_ANGLE "].cosbeta"



	


