#include "ComputeCollision.h"
#include <iostream>
ComputeCollision::ComputeCollision(){

}

ComputeCollision::~ComputeCollision()
{
}

bool ComputeCollision::computeCollisionAABB(AABB _AABBone, AABB _AABBtwo)
{
	if (glm::abs(_AABBone._centre.x - _AABBtwo._centre.x) > glm::abs(_AABBone._extent.x - _AABBtwo._extent.x) ||
		glm::abs(_AABBone._centre.y - _AABBtwo._centre.y) > glm::abs(_AABBone._extent.y - _AABBtwo._extent.y) ||
		glm::abs(_AABBone._centre.z - _AABBtwo._centre.z) > glm::abs(_AABBone._extent.z - _AABBtwo._extent.z))
	return false;
	else return true;
}
