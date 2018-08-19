#include "..\HeaderFiles\Camera.h"
#include "..\HeaderFiles\Curve.h"
#include "..\HeaderFiles\Engine.h"

Camera::Camera()
{

	rotation = glm::vec3(0, 0, 0);
	COP = glm::vec3(0, 0, 0);
	angularSpeed = 45.0f * 3.1415 / 180;
	speed = 100.0f;

	near_plane = 0.4f;
	far_plane = 200.0f;
	fovy = 60.0f;
	

	projectionMatrix = glm::perspective(glm::radians(fovy), 1.0f, near_plane, far_plane);

	viewMatrix = glm::mat4(1.0f);
	freeMovement = true;
	UpdateCamera();
}

Camera::~Camera()
{

}

void Camera::UpdateCamera()
{

	glm::mat4 xRot(1.0f);
	glm::mat4 yRot(1.0f);
	glm::mat4 zRot(1.0f);

	glm::mat4 rotMatrix(1.0f);
	glm::mat4 tMatrix(1.0f);


	xRot = glm::rotate(xRot, rotation.x, glm::vec3(1, 0, 0));
	yRot = glm::rotate(yRot, rotation.y, glm::vec3(0, 1, 0));
	zRot = glm::rotate(zRot, rotation.z, glm::vec3(0, 0, 1));
	

	
	rotMatrix = xRot*yRot*zRot;
	tMatrix = glm::translate(tMatrix, -COP);

	viewMatrix = rotMatrix* tMatrix;
	//we apply the inverse of the translation
	if (!freeMovement ) {
		float time = Engine::LastTime / 1000.0f - motionStartTime;

		//the reason behind the delta computation of points
		//is that for some reason the point p appears to be the same as the COP in the first instant.
		//So we are predicting another one with an approximation
		glm::vec3 p =  motion->interPolate(time);
		glm::vec3 p2 = motion->interPolate(time+0.1);
	
		if (lookAtRoute) {
			viewMatrix = glm::lookAt(COP, COP + glm::normalize(p2-p), glm::vec3(0, 1, 0));
		}

		COP.y = p.y;
		COP.x = p.x;
		COP.z = p.z;
	}

	
	


}

glm::mat4 Camera::get_View()
{
	return viewMatrix;
}

glm::mat4 Camera::get_Projection()
{
	return projectionMatrix;
}

glm::vec2 Camera::get_windowSize()
{
	return currentWindowSize;
}

glm::vec3 Camera::get_Position()
{
	return COP;
}

glm::mat4 Camera::get_ViewInverse()
{
	return glm::inverse(viewMatrix);
}

float Camera::get_NearPlane()
{
	return near_plane;
}

float Camera::get_FarPlane()
{
	return far_plane;
}

void Camera::ResizeEvent(int width, int height)
{
	currentWindowSize = glm::vec2(width, height);
	float AR = width*1.0f / height*1.0f;
	lastAR = AR;
	float FOVY = fovy *3.14159f / 180.0f;

	projectionMatrix = glm::mat4(0.0f);


	float t = tan(FOVY / 2)*near_plane;
	float b = -t;
	float r = AR *t;
	float l = -r;

	projectionMatrix[0].x = (2 * near_plane) / (r - l);
	projectionMatrix[1].y = 2 * near_plane / (t - b);
	projectionMatrix[2].z = -(far_plane + near_plane) / (far_plane - near_plane);
	projectionMatrix[3].z = -2.0f*far_plane*near_plane / (far_plane - near_plane);
	projectionMatrix[2].w = -1.0f;


}

void Camera::MouseMotionEvent(int x, int y)
{
	glm::vec2 currentMousePixelPos(x, y);
	glm::vec2 differenceInMousPos = currentMousePixelPos - mousePixelPos;
	differenceInMousPos *= 0.01745329252f;//degree to radians
	if (!lookAtRoute) {								  //std::cout << differenceInMousPos.x << " , " << differenceInMousPos.y<<std::endl;
		rotation.x += 10 * differenceInMousPos.y;
		rotation.y += 10 * differenceInMousPos.x;
		mousePixelPos = currentMousePixelPos;
		UpdateCamera();
	}
}

void Camera::MouseEvent(int x, int y)
{
	mousePixelPos = glm::vec2(x, y);
}

void Camera::setCOP(glm::vec3 cop)
{
	this->COP = cop;
	UpdateCamera();
}


void Camera::KeyBoardEvent(unsigned char key, int x, int y,float deltaTime) {

	forward = glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]);
	forward =glm::normalize(forward);
	right = glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
	float t ;
	float b ;
	float r ;
	float l ;
	float FOVY;
	switch (key)
	{
	case 'w':
		if (freeMovement)COP += -forward * speed * deltaTime;
		break;

	case 's':
		if (freeMovement)COP -= -forward * speed * deltaTime;
		break;
	case 'a':
		if (freeMovement)COP -= right * speed * deltaTime;
		break;

	case 'd':
		if(freeMovement)COP += right * speed * deltaTime;
		break;
	case 'q':
		rotation.y -= angularSpeed *deltaTime;


		break;

	case 'e':
		rotation.y += angularSpeed *deltaTime;
		break;

	case 'Z':
		fovy += 0.5;
		fovy=fovy >180 ? 180 : fovy;
		 FOVY = fovy *3.14159f / 180.0f;

		projectionMatrix = glm::mat4(0.0f);


		 t = tan(FOVY / 2)*near_plane;
		 b = -t;
		 r = lastAR *t;
		 l = -r;

		projectionMatrix[0].x = (2 * near_plane) / (r - l);
		projectionMatrix[1].y = 2 * near_plane / (t - b);
		projectionMatrix[2].z = -(far_plane + near_plane) / (far_plane - near_plane);
		projectionMatrix[3].z = -2.0f*far_plane*near_plane / (far_plane - near_plane);
		projectionMatrix[2].w = -1.0f;

		break;
	case 'z':
		fovy -= 0.5;
		fovy = fovy <0 ? 0.5 : fovy;


		 FOVY = fovy *3.14159f / 180.0f;

		projectionMatrix = glm::mat4(0.0f);


		 t = tan(FOVY / 2)*near_plane;
		 b = -t;
		 r = lastAR *t;
		 l = -r;

		projectionMatrix[0].x = (2 * near_plane) / (r - l);
		projectionMatrix[1].y = 2 * near_plane / (t - b);
		projectionMatrix[2].z = -(far_plane + near_plane) / (far_plane - near_plane);
		projectionMatrix[3].z = -2.0f*far_plane*near_plane / (far_plane - near_plane);
		projectionMatrix[2].w = -1.0f;

		break;
	case 'f':
		freeMovement = !freeMovement;
		if (!freeMovement) {
			if (motion) {
				motion->setInitialPoint(glm::vec2(COP.x,COP.y));
				motionStartTime = Engine::LastTime/1000.0f;
			}

		}
		break;
	case 'F':
		lookAtRoute = !lookAtRoute;
		std::cout << "Looking: "<< lookAtRoute << std::endl;
		break;
	case 'B':
		Curve::hasDebug = !Curve::hasDebug;
		break;

	default:
		break;

	}
	UpdateCamera();
}

void Camera::addCurve(Curve * c)
{
	this->motion = c;
}
