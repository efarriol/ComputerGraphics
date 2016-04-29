#pragma once
#include "CollisionAABB.h"
#include <glm/glm.hpp>	
class ComputeCollision
{

public:
	ComputeCollision();
	~ComputeCollision();
	bool computeCollisionAABB(AABB _AABBone, AABB _AABBtwo);

};

