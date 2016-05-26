#pragma once
#include <glm/glm.hpp>			//OpenGL Mathematics 
#include <string>
#include "MaterialManager.h"
struct GameObject
{
	int _maxCars;
	int _objectType;
	glm::vec4 _color;
	glm::vec3 _translate;
	GLuint _textureID;
	bool _textureRepetion;
	bool _texturedObject;
	float _angle;
	glm::vec3 _rotation;
	glm::vec3 _scale;
	std::string _textureFile;
	int _collisionType;
	std::string _materialType;
	bool _lightEnable;
};