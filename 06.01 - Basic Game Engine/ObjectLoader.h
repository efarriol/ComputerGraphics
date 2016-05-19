#pragma once
#include "ErrorManagement.h"
#include "Vertex.h"
#include "GameObject.h"
#include "parserAse.h"


class ObjectLoader
{
public:
	ObjectLoader();
	~ObjectLoader();
	void loadAse(char * fileName, std::vector <int> & numVertices, std::vector <Vertex *> & verticesData);
	void computeNormals(File *Object);
};

