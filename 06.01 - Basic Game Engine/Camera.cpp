#include "Camera.h"

Camera::Camera(){
	_near = 0.1f;
	_far = 30.0f;
	_projectionWidth = 10.0f;
	_projectionHeight = 10.0f;
	_FOV = 45.0f;
	_cameraPos = glm::vec3(10.0f, 0.0f, 0.0f);
	_cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
	_cameraUP = glm::vec3(0.0f, 0.0f, 1.0f);
	_aspectRatio = (WIDTH / HEIGHT);
}

Camera::Camera(float screenWidth, float screenHeight, int screenType){
	_near = 0.1f;
	_far = 30.0;
	_projectionWidth = 7.0f;
	_projectionHeight = 7.0f;
	_FOV = 75.0f;
	_cameraPos = glm::vec3(0.0f, 3.0f, 3.0f);
	_cameraFront = glm::vec3(0.0f, 1.0f,0.0f);
	_cameraUP = glm::vec3(0.0f, 0.0f, 0.0f);
	_aspectRatio = (screenWidth / screenHeight);
	_screenType = screenType;
}



Camera::~Camera(){
}

glm::mat4 Camera::projectionMatrix() {
	if (_screenType == PERSP_CAM || _screenType == AUTO_CAM) {
		_projectionMatrix = glm::perspective(_FOV, _aspectRatio, _near, _far);
	}
	else if (_screenType == ORTHO_CAM) {
		_projectionMatrix = glm::ortho(-_projectionWidth / 2, _projectionWidth / 2, -_projectionHeight / 2, _projectionHeight / 2, _near, _far);
	}
	return _projectionMatrix;
}

glm::mat4 Camera::viewMatrix() {
	glm::vec3 cameraDirection = glm::normalize(_cameraPos - _cameraFront);
	glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	_cameraUP = glm::cross(cameraDirection, cameraRight);
	_viewMatrix = glm::lookAt(_cameraPos, _cameraFront, _cameraUP);
	return _viewMatrix;
}

glm::vec3 Camera::getPosition(){
	return _cameraPos;
}

glm::vec3 Camera::getFront()
{
	return _cameraFront;
}

void Camera::setPosition(glm::vec3 newPosition){
	_cameraPos = newPosition;
}

void Camera::setFront(glm::vec3 newFront)
{
	_cameraFront = newFront;
}

