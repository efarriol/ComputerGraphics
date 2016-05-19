#define _CRT_SECURE_NO_WARNINGS
/**
 * Aquest fitxer conté les funcionalitats per llegir un fitxer en format ASE
 *
 * El fitxer ASE conté la geometria (mitjançant triangles) i els materials (del tipus Phong)
 *   d'un objecte 3D. Aquest pot estar composat de varies malles (sMesh)
 *
 * Per llegir un fitxer s'ha de  pantalla s'ha de cridar a la funció LoadFile( char *pFileName , File *fl )
 *  a la qual li passem la ruta del fitxer ase "pFileName" i retorna un punter a una estructura File "fl".
 * A partir d'aquest moment, hem d'utilitzar aquest punter tipus File per accedir a parts d'aquest fitxer.
 *
 */

#ifndef __PARSERASE_H__
#define __PARSERASE_H__

	#include <stdio.h>
	#include <stdlib.h>
	#include <vector>
	#include <string>

	typedef struct sPoint3D Point3D;
	
	
	struct sPoint3D
	{
		float	s_fXCoordinate;
		float	s_fYCoordinate;
		float	s_fZCoordinate;
	};

	typedef struct sFace3D Face3D;
	struct sFace3D
	{
		unsigned int	s_uiVertexA;
		unsigned int	s_uiVertexB;
		unsigned int	s_uiVertexC;
	};

	typedef struct sColor Color;
	struct sColor
	{
		float	s_fRed;
		float	s_fGreen;
		float	s_fBlue;
	};

	typedef struct sMesh Mesh;
	struct sMesh
	{
		std::vector < Point3D * >	s_vVertex;
		std::vector < Point3D * >	s_vTVertex;
		std::vector < Point3D * >	s_vNVertex;
		std::vector < Face3D * >	s_vFace;
		std::vector < Face3D * >	s_vTFace;
		std::vector < Point3D * >	s_vNFace;
		std::string					s_sName;
		int							s_iMaterialId;
		bool						s_bHasTexture;
		bool						s_bHasMaterial;
		Color						s_Wireframe;
	};

	typedef struct sMaterial Material;
	struct sMaterial
	{
		Color	s_Ambient;
		Color	s_Diffuse;
		Color	s_Specular;
		float	s_fShineComponent;
		float	s_fTransparencyComponent;
		std::string	s_sName;
		std::string	s_sPath;
	};

	typedef struct sMaterialList MaterialList;
	struct sMaterialList
	{
		std::vector < Material * >	s_vMaterial;
	};

	typedef struct sMeshList MeshList;
	struct sMeshList
	{
		std::vector < Mesh * >	s_vMesh;
	};

	typedef struct sFile File;
	struct sFile
	{
		MaterialList	s_MatList;
		MeshList		s_MeshList;
	};

		//Methods for getting/setting information about normals
	void    addNormal(File & fObject, int m, float x, float y, float z);
	void	addVertexNormal(File & fObject, int m, Point3D *normal);
	void	updateVertexNormal(File & fObject, int m, int fv, Point3D *normal, int method);
	Point3D* getVertexNormal(File & fObject, int m, int v);
	Point3D getNormal(File & fObject, int m, int f);
	
		//Methods for getting/setting information about material properties
	Color	getDifuse(File & fObject, int m);
	Color	getAmbient(File & fObject, int m);
	Color	getSpecular(File & fObject, int m);
	float	getShine(File & fObject, int m);

	Color	getColor( File & fObject , int m );
	int		getNumMeshes(File & fObject);
	int		getNumFaces( File & fObject, int m );
	int     getNumVertex(File & fObject, int m);
	Point3D getVertex( File & fObject , int m , int f , int v );
	int		getNumVerticesModel(File & fObject);
	
	bool	LoadFile( char *pFileName , File *fl );
	bool	ReadMeshNormalsNode( FILE *f , File *fl );
	bool	ReadMeshNode( FILE *f , File *fl );
	bool	ReadMeshVertexListNode( FILE *f , File *fl );
	bool	ReadMeshFaceListNode( FILE *f , File *fl );
	bool	ReadMeshTVertexListNode( FILE *f , File *fl );
	bool	ReadMeshTFaceListNode( FILE *f , File *fl );
	bool	ReadSceneNode( FILE *f , File *fl );
	bool	ReadMaterialListNode( FILE *f , File *fl );
	bool	ReadMaterialNode( FILE *f , File *fl );
	bool	ReadMapDiffuseNode( FILE *f , File *fl );
	bool	ReadGeomObjectNode( FILE *f , File *fl );
	bool	ReadNodeTMNode( FILE *f , File *fl );

#endif
