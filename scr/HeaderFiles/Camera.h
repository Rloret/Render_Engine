#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include "GL\glew.h"

#include <iostream>

class Curve;

/*
	Camera class it controlls the movements typical movements of the camera, modifying  zoom, rotations, path following...
*/
class Camera {
private:

	
	float angularSpeed = 45.0f * 3.1415f / 180;
	float speed = 30.0f;
	float fovy ;
	float motionStartTime;

	float near_plane;
	float far_plane;
	Curve* motion;

	glm::vec2 mousePixelPos;
	glm::vec2 currentWindowSize;
	glm::vec3 rotation;
	glm::vec3 COP ;
	glm::vec3 forward, right;

	float lastAR;

	glm::mat4 viewMatrix;

	glm::mat4 projectionMatrix;

	
public:

	Camera();
	~Camera();

	bool freeMovement = false;
	bool lookAtRoute = false;


	void UpdateCamera();

	glm::mat4 get_View();
	glm::mat4 get_Projection();
	glm::vec2 get_windowSize();
	glm::vec3 get_Position();
	glm::mat4 get_ViewInverse();
	float get_NearPlane();
	float get_FarPlane();
	void ResizeEvent(int width, int height);
	void MouseMotionEvent(int x, int y);
	void MouseEvent(int x, int y);
	void setCOP(glm::vec3 cop);
	void Camera::KeyBoardEvent(unsigned char key, int x, int y, float deltaTime);
	void addCurve(Curve* c);
	
};