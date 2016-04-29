#include "ObjectLoader.h"



ObjectLoader::ObjectLoader()
{
}


ObjectLoader::~ObjectLoader()
{
}

/*
* Load a 3D object and store it in the position X
*/
void ObjectLoader::loadAse(char * fileName, std::vector <int> & numVertices, std::vector <Vertex *> & verticesData) {
	//Read the .ASE file 
	File model;
	LoadFile(fileName, &model);
	int currentVertex = 0;
	ColorRGBA8 cRGBA;


	int newObjectID = numVertices.size();

		//Allocate memory for one additional element
	numVertices.resize(newObjectID + 1);
	verticesData.resize(newObjectID + 1);

		//Allocate memory for storing the vertices
	numVertices[newObjectID] = getNumVerticesModel(model);
	verticesData[newObjectID] = new Vertex[numVertices[newObjectID]];
	if (verticesData[newObjectID] == 0)
		ErrorManagement::errorRunTime("System was not able to allocate memory for the ASE model");

	//Get the vertices from the model and store them into the Vertex *
	int iNumMeshes = getNumMeshes(model);

	//Analyze the different meshes
	for (int iCntMeshes = 0; iCntMeshes < iNumMeshes; iCntMeshes++) {
		//A mesh is composed by different faces. All the faces share the same color. The color goes from 0 to 1
		int iNumFaces = getNumFaces(model, iCntMeshes);
		Color color = getColor(model, iCntMeshes);
		cRGBA.r = (GLubyte) color.s_fRed * 255;
		cRGBA.g = (GLubyte)color.s_fGreen * 255;
		cRGBA.b = (GLubyte)color.s_fBlue * 255;
		cRGBA.a = 255;

		//A face is composed by different triangles
		for (int iCntFaces = 0; iCntFaces < iNumFaces; iCntFaces++) {
			for (int v = 1;v <= 3;v++) {
				Point3D vertex = getVertex(model, iCntMeshes, iCntFaces, v);
				verticesData[newObjectID][currentVertex].setPosition(vertex.s_fXCoordinate, vertex.s_fYCoordinate, vertex.s_fZCoordinate);
				verticesData[newObjectID][currentVertex].setColor(cRGBA.r, cRGBA.g, cRGBA.b, cRGBA.a);
				currentVertex++;
			}
		}
	}
}
