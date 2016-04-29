#pragma once
#include <glm/glm.hpp>			//OpenGL Mathematics 
struct AABB {
	glm::vec3 _centre;
	glm::vec3 _extent;
	float _multiplier;
};
