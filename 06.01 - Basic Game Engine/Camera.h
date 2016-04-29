#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Constants.h"
class Camera
{
public:
	Camera();
	Camera(float screenWidth, float screenHeight, int type);
	~Camera();
	glm::mat4 projectionMatrix();
	glm::mat4 viewMatrix();
	glm::vec3 getPosition();
	void setPosition(glm::vec3 newPosition);
private:
	float _aspectRatio;
	glm::mat4 _projectionMatrix;
	glm::mat4 _viewMatrix;
	float _FOV;
	float _far;
	float _near;
	float _projectionWidth;
	float _projectionHeight;
	glm::vec3 _cameraPos;
	glm::vec3 _cameraFront;
	glm::vec3 _cameraUP;
	int _screenType;

};

