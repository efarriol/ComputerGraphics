#pragma once
#include "Vertex.h"
#include <vector>
#include "GameObject.h"
#include "CollisionAABB.h"
#include "ObjectLoader.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include <random> 

#define BLUE_CUBE 0
#define RED_CUBE 1
#define WHITE_CUBE 2
#define CAR_ASE 3
#define ROAD 4
#define GRASS 5
#define STREETLIGHT_ASE 6
#define FENCE_ASE 7
#define MENU_TITLE 8
#define HOUSE 9
#define ROOF 10
#define LOSE_TITLE 11
#define NUMBASICOBJECTS 12

//This class stores and manipulates all the objects loaded from the text fileddd
class Geometry
{
	std::vector <Vertex *> _verticesData;
	std::vector <int> _numVertices;
	std::vector <GameObject> _listOfObjects;
	std::vector <AABB> _listOfAABB;
	glm::vec3 center;
	float max, min, mid;
	int _numBasicObjects;
	float maxX; //very small value
	float maxY;
	float maxZ;
	float minX;  //very big value
	float minY;
	float minZ;


public:
	Geometry();
	~Geometry();
	void loadGameElements(char fileName[100]);
	Vertex * getData(int objectID);
	int getNumVertices(int objectID);
	int getNumGameElements();
	GameObject & getGameElement(int objectID);
	void loadCube(int cube, glm::vec4 color);
	void loadPlane(int plane, glm::vec4 color);
	AABB & getAABB(int objectID);
	void loadBasic3DObjects();
	glm::vec3 computeFaceNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
	ObjectLoader _objectLoader;
	TextureManager _texture;
	MaterialManager _material;
	bool firstRender;
};

