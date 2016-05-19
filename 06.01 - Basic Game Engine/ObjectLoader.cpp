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
	Color color;

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

	//Analyze the different meshes in order to get information about positions, color and also compute the UV values
	for (int iCntMeshes = 0; iCntMeshes < iNumMeshes; iCntMeshes++) {
		//A mesh is composed by different faces. All the faces share the same color. The color goes from 0 to 1
		int iNumFaces = getNumFaces(model, iCntMeshes);
	
		color = getColor(model, iCntMeshes);
		cRGBA.r = (GLubyte)color.s_fRed * 255;
		cRGBA.g = (GLubyte)color.s_fGreen * 255;
		cRGBA.b = (GLubyte)color.s_fBlue * 255;
		cRGBA.a = 255;
	
		UV normals[3];
		normals[0].u = 0;
		normals[0].v = 0;
		normals[1].u = 1;
		normals[1].v = 0;
		normals[2].u = 0;
		normals[2].v = 1;

		//A face is composed by different triangles
		for (int iCntFaces = 0; iCntFaces < iNumFaces; iCntFaces++) {
			for (int v = 1;v <= 3;v++) {
				Point3D vertex = getVertex(model, iCntMeshes, iCntFaces, v);
				verticesData[newObjectID][currentVertex].setPosition(vertex.s_fXCoordinate, vertex.s_fYCoordinate, vertex.s_fZCoordinate);
				verticesData[newObjectID][currentVertex].setColor(cRGBA.r, cRGBA.g, cRGBA.b, cRGBA.a);
				verticesData[newObjectID][currentVertex].setUV(normals[v - 1].u, normals[v - 1].v);				
				currentVertex++;
			}
		}
	}
	
	//Compute normals
	Point3D* normal = new Point3D();
	computeNormals(&model);
	for (int iCntMeshes = 0; iCntMeshes < iNumMeshes; iCntMeshes++) {
		int iNumVertexs = getNumVertex(model, iCntMeshes);
		for (int iCntVertexs = 0; iCntVertexs < iNumVertexs; iCntVertexs++) {
			normal = getVertexNormal(model, iCntMeshes, iCntVertexs);
			verticesData[newObjectID][iCntVertexs].setNormalVector(normal->s_fXCoordinate, normal->s_fYCoordinate, normal->s_fZCoordinate);
		}
	}
	delete normal;
}


/*
* Compute the normmal
* NOTE: This method is not using the glm methods because glm didn't exist when it was written!!!
*/
void ObjectLoader::computeNormals(File *Object) {
	Point3D vecBA;
	Point3D vecBC;
	Point3D* normal = new Point3D();
	normal->s_fXCoordinate = 0;
	normal->s_fYCoordinate = 0;
	normal->s_fZCoordinate = 0;

	int iNumMeshes = getNumMeshes(*Object);

	//For each vertex, set normals to  (0,0,0)	
	for (int iCntMeshes = 0; iCntMeshes < iNumMeshes; iCntMeshes++) 	{		
		int iNumVertexs = getNumVertex(*Object, iCntMeshes);
		for (int iCntVertexs = 0; iCntVertexs < iNumVertexs; iCntVertexs++) 	{
			addVertexNormal(*Object, iCntMeshes, normal);
		}
	}

	for (int iCntMeshes = 0; iCntMeshes < iNumMeshes; iCntMeshes++) 	{

		int	iNumFaces = getNumFaces(*Object, iCntMeshes);
		for (int iCntFaces = 0; iCntFaces < iNumFaces; iCntFaces++) 		{
			// Get three vertices from the current triangle
			Point3D  pointA = getVertex(*Object, iCntMeshes, iCntFaces, 1);
			Point3D  pointB = getVertex(*Object, iCntMeshes, iCntFaces, 2);
			Point3D  pointC = getVertex(*Object, iCntMeshes, iCntFaces, 3);

			// Compute BA and BC vectors
			vecBA.s_fXCoordinate = pointA.s_fXCoordinate - pointB.s_fXCoordinate;
			vecBA.s_fYCoordinate = pointA.s_fYCoordinate - pointB.s_fYCoordinate;
			vecBA.s_fZCoordinate = pointA.s_fZCoordinate - pointB.s_fZCoordinate;
			vecBC.s_fXCoordinate = pointC.s_fXCoordinate - pointB.s_fXCoordinate;
			vecBC.s_fYCoordinate = pointC.s_fYCoordinate - pointB.s_fYCoordinate;
			vecBC.s_fZCoordinate = pointC.s_fZCoordinate - pointB.s_fZCoordinate;

			// Compute BC x BA
			normal = new Point3D();
			normal->s_fXCoordinate = vecBC.s_fYCoordinate * vecBA.s_fZCoordinate - vecBA.s_fYCoordinate * vecBC.s_fZCoordinate;
			normal->s_fYCoordinate = vecBA.s_fXCoordinate * vecBC.s_fZCoordinate - vecBC.s_fXCoordinate * vecBA.s_fZCoordinate;
			normal->s_fZCoordinate = vecBC.s_fXCoordinate * vecBA.s_fYCoordinate - vecBA.s_fXCoordinate * vecBC.s_fYCoordinate;

			// Normal is normalized
			float fModul = sqrt((normal->s_fXCoordinate * normal->s_fXCoordinate) + (normal->s_fYCoordinate *normal->s_fYCoordinate) + (normal->s_fZCoordinate *normal->s_fZCoordinate));
			normal->s_fXCoordinate = normal->s_fXCoordinate / fModul;
			normal->s_fYCoordinate = normal->s_fYCoordinate / fModul;
			normal->s_fZCoordinate = normal->s_fZCoordinate / fModul;

			addNormal(*Object, iCntMeshes, normal->s_fXCoordinate, normal->s_fYCoordinate, normal->s_fZCoordinate);

			// Current normal value is equal to previous value + new value
			updateVertexNormal(*Object, iCntMeshes, iCntFaces, normal, 0);
		}
	}

		//Normalization of normals
	for (int iCntMeshes = 0; iCntMeshes < iNumMeshes; iCntMeshes++)
	{
		int iNumVertexs = getNumVertex(*Object, iCntMeshes);
		for (int iCntVertexs = 0; iCntVertexs < iNumVertexs; iCntVertexs++)
		{
			normal = getVertexNormal(*Object, iCntMeshes, iCntVertexs);
			// Calculo la norma
			float fNorma = sqrt(normal->s_fXCoordinate * normal->s_fXCoordinate + normal->s_fYCoordinate * normal->s_fYCoordinate + normal->s_fZCoordinate * normal->s_fZCoordinate);

			normal->s_fXCoordinate /= fNorma;
			normal->s_fYCoordinate /= fNorma;
			normal->s_fZCoordinate /= fNorma;
			updateVertexNormal(*Object, iCntMeshes, iCntVertexs, normal, 1);
		}
	}

}
