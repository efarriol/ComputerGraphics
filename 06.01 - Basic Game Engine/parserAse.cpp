#include "parserAse.h"

bool ReadMeshFaceListNode( FILE *f , File *fl )
{
	char	 acLine[1000];
	int		 iId , iCA , iCB , iCC;
	Face3D	*face;

	fgets( acLine , 1000 , f );
	while( !feof( f ) )
	{
		if( strstr( acLine , "MESH_FACE" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1 , SEEK_CUR );
			fscanf( f , "\t\t\t*MESH_FACE\t%d:\tA:\t%d\tB:\t%d\tC:\t%d" , &iId , &iCA , &iCB , &iCC );
			face = new Face3D;
			if( face != NULL )
			{
				face->s_uiVertexA = iCA;
				face->s_uiVertexB = iCB;
				face->s_uiVertexC = iCC;
				fl->s_MeshList.s_vMesh.at( ( int )fl->s_MeshList.s_vMesh.size() - 1 )->s_vFace.push_back( face );
			}
			fgets( acLine , 1000 , f );
		}

		else if( strstr( acLine , "}" ) )
		{
			return true;
		}
		fgets( acLine , 1000 , f );
	}
	return false;
}

bool ReadMeshVertexListNode( FILE *f , File *fl )
{
	char	 acLine[1000];
	int		 iId;
	float	 fCX , fCY , fCZ;
	Point3D *point;

	fgets( acLine , 1000 , f );
	while( !feof( f ) )
	{
		if( strstr( acLine , "MESH_VERTEX" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1 , SEEK_CUR );
			fscanf( f , "\t\t\t*MESH_VERTEX\t%d\t%f\t%f\t%f\n" , &iId , &fCX , &fCY , &fCZ );
			point = new Point3D;
			if( point != NULL )
			{
				point->s_fXCoordinate = fCX;
				point->s_fYCoordinate = fCY;
				point->s_fZCoordinate = fCZ;
				fl->s_MeshList.s_vMesh.at( ( int )fl->s_MeshList.s_vMesh.size() - 1 )->s_vVertex.push_back( point );
			}
		}

		else if( strstr( acLine , "}" ) )
		{
			return true;
		}
		fgets( acLine , 1000 , f );
	}
	return false;
}

bool ReadMeshTVertexListNode( FILE *f , File *fl )
{
	char	 acLine[1000];
	int		 iId;
	float	 fCX , fCY , fCZ;
	Point3D *point;

	fgets( acLine , 1000 , f );
	while( !feof( f ) )
	{
		if( strstr( acLine , "MESH_TVERT" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1 , SEEK_CUR );
			fscanf( f , "\t\t\t*MESH_TVERT\t%d\t%f\t%f\t%f\n" , &iId , &fCX , &fCY , &fCZ );
			point = new Point3D;
			if( point != NULL )
			{
				point->s_fXCoordinate = fCX;
				point->s_fYCoordinate = fCY;
				point->s_fZCoordinate = fCZ;
				fl->s_MeshList.s_vMesh.at( ( int )fl->s_MeshList.s_vMesh.size() - 1 )->s_vTVertex.push_back( point );
			}
		}

		else if( strstr( acLine , "}" ) )
		{
			return true;
		}
		fgets( acLine , 1000 , f );
	}
	return false;
}

bool ReadMeshTFaceListNode( FILE *f , File *fl )
{
	char	acLine[1000];
	int		iId , iCA , iCB , iCC;
	Face3D *face;

	fgets( acLine , 1000 , f );
	while( !feof( f ) )
	{
		if( strstr( acLine , "MESH_TFACE" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1 , SEEK_CUR );
			fscanf( f , "\t\t\t*MESH_TFACE\t%d\t%d\t%d\t%d\n" , &iId , &iCA , &iCB , &iCC );
			face = new Face3D;
			if( face != NULL )
			{
				face->s_uiVertexA = iCA;
				face->s_uiVertexB = iCB;
				face->s_uiVertexC = iCC;
				fl->s_MeshList.s_vMesh.at( ( int )fl->s_MeshList.s_vMesh.size() - 1 )->s_vTFace.push_back( face );
			}
		}

		else if( strstr( acLine , "}" ) )
		{
			return true;
		}
		fgets( acLine , 1000 , f );
	}
	return false;
}

bool ReadMeshNormalsNode( FILE *f , File *fl )
{
	char	acLine[1000];
	int		iId ;
	float	iCA , iCB , iCC;
	Face3D *face;
	float	 fCX , fCY , fCZ;
	Point3D *point;

	fgets( acLine , 1000 , f );
	while( !feof( f ) )
	{
		if( strstr( acLine , "MESH_FACENORMAL" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1 , SEEK_CUR );
			fscanf( f , "\t\t\t*MESH_FACENORMAL %d\t%f\t%f\t%f\n" , &iId , &iCA , &iCB , &iCC );
			face = new Face3D;
			if( face != NULL )
			{
				face->s_uiVertexA = (unsigned int) iCA;
				face->s_uiVertexB = (unsigned int) iCB;
				face->s_uiVertexC = (unsigned int) iCC;
//				fl->s_MeshList.s_vMesh.at( ( int )fl->s_MeshList.s_vMesh.size() - 1 )->s_vNFace.push_back( face );
			}
		}

		else if( strstr( acLine , "MESH_VERTEXNORMAL" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1 , SEEK_CUR );
			fscanf( f , "\t\t\t\t*MESH_VERTEXNORMAL %d\t%f\t%f\t%f\n" , &iId , &fCX , &fCY , &fCZ );
			point = new Point3D;
			if( point != NULL )
			{
				point->s_fXCoordinate = fCX;
				point->s_fYCoordinate = fCY;
				point->s_fZCoordinate = fCZ;
				fl->s_MeshList.s_vMesh.at( ( int )fl->s_MeshList.s_vMesh.size() - 1 )->s_vNVertex.push_back( point );
			}
		}

		else if( strstr( acLine , "}" ) )
		{
			return true;
		}
		fgets( acLine , 1000 , f );
	}
	return false;
}

bool ReadMeshNode( FILE *f , File *fl )
{
	char	 acLine[1000];
	int		 NumVtx , NumFc , NumTVtx , NumTFc;

	fgets( acLine , 1000 , f );
	while( !feof( f ) )
	{
		if( strstr( acLine , "TIMEVALUE" ) )
		{
		}

		else if( strstr( acLine , "MESH_NUMVERTEX" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1, SEEK_CUR );
			fscanf( f , "\t\t*MESH_NUMVERTEX\t%d\n" , &NumVtx );
		}

		else if( strstr( acLine , "MESH_NUMFACES" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1 , SEEK_CUR );
			fscanf( f , "\t\t*MESH_NUMFACES\t%d\n" , &NumFc );
		}

		else if( strstr( acLine , "MESH_VERTEX_LIST" ) )
		{
			if( !ReadMeshVertexListNode( f , fl ) ) return false;
		}

		else if( strstr( acLine , "MESH_FACE_LIST" ) )
		{
			if( !ReadMeshFaceListNode( f , fl ) ) return false;
		}

		else if( strstr( acLine , "MESH_NUMTVERTEX" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1 , SEEK_CUR );
			fscanf( f , "\t\t*MESH_NUMTVERTEX %d\n" , &NumTVtx );
			fl->s_MeshList.s_vMesh.at( ( int )fl->s_MeshList.s_vMesh.size() - 1 )->s_bHasTexture = true;
		}

		else if( strstr( acLine , "MESH_TVERTLIST" ) )
		{
			fl->s_MeshList.s_vMesh.at( ( int )fl->s_MeshList.s_vMesh.size() - 1 )->s_bHasTexture = true;
			if( !ReadMeshTVertexListNode( f , fl ) ) return false;
		}

		else if( strstr( acLine , "MESH_NUMTVFACES" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1 , SEEK_CUR );
			fscanf( f , "\t\t*MESH_NUMTVFACES %d\n" , &NumTFc );
			fl->s_MeshList.s_vMesh.at( ( int )fl->s_MeshList.s_vMesh.size() - 1 )->s_bHasTexture = true;
		}

		else if( strstr( acLine , "MESH_TFACELIST" ) )
		{
			fl->s_MeshList.s_vMesh.at( ( int )fl->s_MeshList.s_vMesh.size() - 1 )->s_bHasTexture = true;
			if( !ReadMeshTFaceListNode( f , fl ) ) return false;
		}

		else if( strstr( acLine , "MESH_NORMALS" ) )
		{
			if( !ReadMeshNormalsNode( f , fl ) ) return false;
		}

		else if( strstr( acLine , "}" ) )
		{
			return true;
		}
		fgets( acLine , 1000 , f );
	}
	return false;
}

bool ReadSceneNode( FILE *f , File *fl )
{
	char	acLine[1000];

	fgets( acLine , 1000 , f );
	while( !feof( f ) )
	{
		if( strstr( acLine , "SCENE_FILENAME" ) )
		{
		}

		else if( strstr( acLine , "SCENE_FIRSTFRAME" ) )
		{
		}

		else if( strstr( acLine , "SCENE_LASTFRAME" ) )
		{
		}

		else if( strstr( acLine , "SCENE_FRAMESPEED" ) )
		{
		}

		else if( strstr( acLine , "SCENE_TICKSPERFRAME" ) )
		{
		}

		else if( strstr( acLine , "SCENE_BACKGROUND_STATIC" ) )
		{
		}

		else if( strstr( acLine , "SCENE_AMBIENT_STATIC" ) )
		{
		}

		else if( strstr( acLine , "}" ) )
		{
			return true;
		}
		fgets( acLine , 1000 , f );
	}
	return false;
}

bool ReadMapDiffuseNode( FILE *f , File *fl )
{
	char	 acLine[1000] , cAux;
	std::string		sMaterialPath;

	fgets( acLine , 1000 , f );
	while( !feof( f ) )
	{
		if( strstr( acLine , "MAP_NAME" ) )
		{
		}

		else if( strstr( acLine , "MAP_CLASS" ) )
		{
		}

		else if( strstr( acLine , "MAP_SUBNO" ) )
		{
		}

		else if( strstr( acLine , "MAP_AMOUNT" ) )
		{
		}

		else if( strstr( acLine , "BITMAP_FILTER" ) )
		{
		}

		else if( strstr( acLine , "BITMAP" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1 , SEEK_CUR );
			fscanf( f , "\t\t\t*BITMAP\t\"" );
			fscanf( f , "%c" , &cAux );
			while( cAux != '\"' )
			{
				sMaterialPath.push_back( cAux );
				fscanf( f , "%c" , &cAux );
			}
			fgets( acLine , 1000 , f );
			fl->s_MatList.s_vMaterial.at( ( int )fl->s_MatList.s_vMaterial.size() - 1 )->s_sPath.assign( sMaterialPath );
		}

		else if( strstr( acLine , "MAP_TYPE" ) )
		{
		}

		else if( strstr( acLine , "UVW_U_OFFSET" ) )
		{
		}

		else if( strstr( acLine , "UVW_V_OFFSET" ) )
		{
		}

		else if( strstr( acLine , "UVW_U_TILING" ) )
		{
		}

		else if( strstr( acLine , "UVW_V_TILING" ) )
		{
		}

		else if( strstr( acLine , "UVW_ANGLE" ) )
		{
		}

		else if( strstr( acLine , "UVW_BLUR" ) )
		{
		}

		else if( strstr( acLine , "UVW_BLUR_OFFSET" ) )
		{
		}

		else if( strstr( acLine , "UVW_NOUSE_AMT" ) )
		{
		}

		else if( strstr( acLine , "UVW_NOISE_SIZE" ) )
		{
		}

		else if( strstr( acLine , "UVW_NOISE_LEVEL" ) )
		{
		}

		else if( strstr( acLine , "UVW_NOISE_PHASE" ) )
		{
		}

		else if( strstr( acLine , "}" ) )
		{
			return true;
		}
		fgets( acLine , 1000 , f );
	}
	return false;
}

bool ReadMaterialNode( FILE *f , File *fl )
{
	char		 acLine[1000] , cAux;
	std::string	 sMaterialName;
	Material	*mat;
	float		 fAuxX , fAuxY , fAuxZ;

	mat = new Material;
	fl->s_MatList.s_vMaterial.push_back( mat );

	fgets( acLine , 1000 , f );
	while( !feof( f ) )
	{
		if( strstr( acLine , "MATERIAL_NAME" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1 , SEEK_CUR );
			fscanf( f , "\t\t*MATERIAL_NAME\t\"" );
			fscanf( f , "%c" , &cAux );
			while( cAux != '\"' )
			{
				sMaterialName.push_back( cAux );
				fscanf( f , "%c" , &cAux );
			}
			fgets( acLine , 1000 , f );
			fl->s_MatList.s_vMaterial.at( ( int )fl->s_MatList.s_vMaterial.size() - 1 )->s_sName.assign( sMaterialName );
		}

		else if( strstr( acLine , "MATERIAL_CLASS" ) )
		{
		}

		else if( strstr( acLine , "*MATERIAL_AMBIENT" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1 , SEEK_CUR );
			fscanf( f , "\t\t*MATERIAL_AMBIENT\t%f\t%f\t%f\n" , &fAuxX , &fAuxY , &fAuxZ );
			fl->s_MatList.s_vMaterial.at( ( int )fl->s_MatList.s_vMaterial.size() - 1 )->s_Ambient.s_fRed	= fAuxX;
			fl->s_MatList.s_vMaterial.at( ( int )fl->s_MatList.s_vMaterial.size() - 1 )->s_Ambient.s_fGreen	= fAuxY;
			fl->s_MatList.s_vMaterial.at( ( int )fl->s_MatList.s_vMaterial.size() - 1 )->s_Ambient.s_fBlue	= fAuxZ;
		}

		else if( strstr( acLine , "*MATERIAL_DIFFUSE" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1 , SEEK_CUR );
			fscanf( f , "\t\t*MATERIAL_DIFFUSE\t%f\t%f\t%f\n" , &fAuxX , &fAuxY , &fAuxZ );
			fl->s_MatList.s_vMaterial.at( ( int )fl->s_MatList.s_vMaterial.size() - 1 )->s_Diffuse.s_fRed	= fAuxX;
			fl->s_MatList.s_vMaterial.at( ( int )fl->s_MatList.s_vMaterial.size() - 1 )->s_Diffuse.s_fGreen	= fAuxY;
			fl->s_MatList.s_vMaterial.at( ( int )fl->s_MatList.s_vMaterial.size() - 1 )->s_Diffuse.s_fBlue	= fAuxZ;
		}

		else if( strstr( acLine , "*MATERIAL_SPECULAR" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1 , SEEK_CUR );
			fscanf( f , "\t\t*MATERIAL_SPECULAR\t%f\t%f\t%f\n" , &fAuxX , &fAuxY , &fAuxZ );
			fl->s_MatList.s_vMaterial.at( ( int )fl->s_MatList.s_vMaterial.size() - 1 )->s_Specular.s_fRed	= fAuxX;
			fl->s_MatList.s_vMaterial.at( ( int )fl->s_MatList.s_vMaterial.size() - 1 )->s_Specular.s_fGreen= fAuxY;
			fl->s_MatList.s_vMaterial.at( ( int )fl->s_MatList.s_vMaterial.size() - 1 )->s_Specular.s_fBlue	= fAuxZ;
		}

		else if( strstr( acLine , "*MATERIAL_SHINE" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1 , SEEK_CUR );
			fscanf( f , "\t\t*MATERIAL_SHINE\t%f\n" , &fAuxX );
			fl->s_MatList.s_vMaterial.at( ( int )fl->s_MatList.s_vMaterial.size() - 1 )->s_fShineComponent = fAuxX;
		}

		else if( strstr( acLine , "*MATERIAL_SHINESTRENGTH" ) )
		{
		}

		else if( strstr( acLine , "*MATERIAL_TRANSPARENCY" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1 , SEEK_CUR );
			fscanf( f , "\t\t*MATERIAL_TRANSPARENCY\t%f\n" , &fAuxX );
			fl->s_MatList.s_vMaterial.at( ( int )fl->s_MatList.s_vMaterial.size() - 1 )->s_fTransparencyComponent = fAuxX;
		}

		else if( strstr( acLine , "*MATERIAL_WIRESIZE" ) )
		{
		}

		else if( strstr( acLine , "*MATERIAL_SHADING" ) )
		{
		}

		else if( strstr( acLine , "*MATERIAL_XP_FALLOFF" ) )
		{
		}

		else if( strstr( acLine , "*MATERIAL_SELFILLUM" ) )
		{
		}

		else if( strstr( acLine , "*MATERIAL_FALLOFF" ) )
		{
		}

		else if( strstr( acLine , "*MATERIAL_XP_TYPE" ) )
		{
		}

		else if( strstr( acLine , "*MAP_DIFFUSE" ) )
		{
			if( ! ReadMapDiffuseNode( f , fl ) ) return false;
		}

		else if( strstr( acLine , "}" ) )
		{
			return true;
		}
		fgets( acLine , 1000 , f );
	}
	return false;
}

bool ReadMaterialListNode( FILE *f , File *fl )
{
	char	 acLine[1000];

	fgets( acLine , 1000 , f );
	while( !feof( f ) )
	{
		if( strstr( acLine , "MATERIAL_COUNT" ) )
		{
		}

		else if( strstr( acLine , "MATERIAL" ) )
		{
			if( !ReadMaterialNode( f , fl ) ) return false;
		}

		else if( strstr( acLine , "}" ) )
		{
			return true;
		}
		fgets( acLine , 1000 , f );
	}
	return false;
}

bool ReadNodeTMNode( FILE *f , File *fl )
{
	char	 acLine[1000];

	fgets( acLine , 1000 , f );
	while( !feof( f ) )
	{
		if( strstr( acLine , "NODE_NAME" ) )
		{
		}

		else if( strstr( acLine , "INHERIT_POS" ) )
		{
		}

		else if( strstr( acLine , "INHERIT_ROT" ) )
		{
		}

		else if( strstr( acLine , "INHERIT_SCL" ) )
		{
		}

		else if( strstr( acLine , "TM_ROW0" ) )
		{
		}

		else if( strstr( acLine , "TM_ROW1" ) )
		{
		}

		else if( strstr( acLine , "TM_ROW2" ) )
		{
		}

		else if( strstr( acLine , "TM_ROW3" ) )
		{
		}

		else if( strstr( acLine , "TM_POS" ) )
		{
		}

		else if( strstr( acLine , "TM_ROTAXIS" ) )
		{
		}

		else if( strstr( acLine , "TM_ROTANGLE" ) )
		{
		}

		else if( strstr( acLine , "TM_SCALE" ) )
		{
		}

		else if( strstr( acLine , "TM_SCALEAXIS" ) )
		{
		}

		else if( strstr( acLine , "TM_SCALEAXISANG" ) )
		{
		}

		else if( strstr( acLine , "}" ) )
		{
			return true;
		}
		fgets( acLine , 1000 , f );
	}
	return false;
}

bool ReadGeomObjectNode( FILE *f , File *fl )
{
	char	 acLine[1000] , cAux;
	std::string sName;
	int		 NumMat;
	float	 fAuxR , fAuxG , fAuxB;

	fgets( acLine , 1000 , f );
	while( !feof( f ) )
	{
		if( strstr( acLine , "NODE_NAME" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1 , SEEK_CUR );
			fscanf( f , "\t*NODE_NAME\t\"" );
			fscanf( f , "%c" , &cAux );
			while( cAux != '\"' )
			{
				sName.push_back( cAux );
				fscanf( f , "%c" , &cAux );
			}
			fl->s_MeshList.s_vMesh.at( ( int )fl->s_MeshList.s_vMesh.size() - 1 )->s_sName.assign( sName );
			fgets( acLine , 1000 , f );
		}

		else if( strstr( acLine , "NODE_TM" ) )
		{
			if( !ReadNodeTMNode( f , fl ) ) return false;
		}

		else if( strstr( acLine , "MESH" ) )
		{
			if( !ReadMeshNode( f , fl ) ) return false;
		}

		else if( strstr( acLine , "PROP_MOTIONBLUR" ) )
		{
		}

		else if( strstr( acLine , "PROP_CASTSHADOW" ) )
		{
		}

		else if( strstr( acLine , "PROP_RECVSHADOW" ) )
		{
		}

		else if( strstr( acLine , "MATERIAL_REF" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1 , SEEK_CUR );
			fscanf( f , "\t*MATERIAL_REF %d\n" , &NumMat );
			fl->s_MeshList.s_vMesh.at( ( int )fl->s_MeshList.s_vMesh.size() - 1 )->s_iMaterialId = NumMat;
			fl->s_MeshList.s_vMesh.at( ( int )fl->s_MeshList.s_vMesh.size() - 1 )->s_bHasMaterial = true;
		}

		else if( strstr( acLine , "WIREFRAME_COLOR" ) )
		{
			fseek( f , -( int )strlen( acLine ) - 1 , SEEK_CUR );
			fscanf( f , "\t*WIREFRAME_COLOR %f %f %f\n" , &fAuxR , &fAuxG , &fAuxB );
			fl->s_MeshList.s_vMesh.at( ( int )fl->s_MeshList.s_vMesh.size() - 1 )->s_Wireframe.s_fRed	= fAuxR;
			fl->s_MeshList.s_vMesh.at( ( int )fl->s_MeshList.s_vMesh.size() - 1 )->s_Wireframe.s_fGreen	= fAuxG;
			fl->s_MeshList.s_vMesh.at( ( int )fl->s_MeshList.s_vMesh.size() - 1 )->s_Wireframe.s_fBlue	= fAuxB;
		}

		else if( strstr( acLine , "}" ) )
		{
			return true;
		}
		fgets( acLine , 1000 , f );
	}
	return false;
}

int getNumMeshes(File & fObject){
	return fObject.s_MeshList.s_vMesh.size();
}

int getNumFaces( File & fObject, int m ){
	Mesh* mesh = fObject.s_MeshList.s_vMesh[m];
	return mesh->s_vFace.size();
}

int getNumVertex(File & fObject, int m) {
	Mesh* mesh = fObject.s_MeshList.s_vMesh[m];
	return mesh->s_vVertex.size();
}

Point3D getVertex( File & fObject , int m , int f , int v ){
	Point3D vertex;
	Mesh* mesh = fObject.s_MeshList.s_vMesh[m];
	Face3D* face = mesh->s_vFace[f];

	switch(v){
		case 1:
			vertex.s_fXCoordinate = mesh->s_vVertex[face->s_uiVertexA]->s_fXCoordinate;
			vertex.s_fYCoordinate = mesh->s_vVertex[face->s_uiVertexA]->s_fYCoordinate;
			vertex.s_fZCoordinate = mesh->s_vVertex[face->s_uiVertexA]->s_fZCoordinate;			
		break;
		case 2:
			vertex.s_fXCoordinate = mesh->s_vVertex[face->s_uiVertexB]->s_fXCoordinate;
			vertex.s_fYCoordinate = mesh->s_vVertex[face->s_uiVertexB]->s_fYCoordinate;
			vertex.s_fZCoordinate = mesh->s_vVertex[face->s_uiVertexB]->s_fZCoordinate;		
		break;
		case 3:
			vertex.s_fXCoordinate = mesh->s_vVertex[face->s_uiVertexC]->s_fXCoordinate;
			vertex.s_fYCoordinate = mesh->s_vVertex[face->s_uiVertexC]->s_fYCoordinate;
			vertex.s_fZCoordinate = mesh->s_vVertex[face->s_uiVertexC]->s_fZCoordinate;		
		break;
	}
	return vertex;
}

Color getColor( File & fObject , int m ){
	Color color;
	Mesh* mesh = fObject.s_MeshList.s_vMesh[m];
	color.s_fRed = fObject.s_MatList.s_vMaterial[mesh->s_iMaterialId]->s_Diffuse.s_fRed;
	color.s_fGreen = fObject.s_MatList.s_vMaterial[mesh->s_iMaterialId]->s_Diffuse.s_fGreen;
	color.s_fBlue = fObject.s_MatList.s_vMaterial[mesh->s_iMaterialId]->s_Diffuse.s_fBlue;
	return color;
}

int getNumVerticesModel(File & fObject) {
	int numVertex = 0;
	int iNumMeshes = getNumMeshes(fObject); 										  
	for (int iCntMeshes = 0; iCntMeshes < iNumMeshes; iCntMeshes++)	{
		int iNumFaces = getNumFaces(fObject, iCntMeshes);
		numVertex = numVertex + (iNumFaces * 3);
	}
	return numVertex;
}


bool LoadFile( char *pFileName , File *fl )
{
	FILE	*f;
	char	 acLine[1000];
	Mesh	*mesh;
	bool	 bLoaded = true;

	f	= fopen( pFileName , "r" );
	if( f != NULL )
	{
		fgets( acLine , 1000 , f );
		while( !feof( f ) )
		{
			if( strstr( acLine , "3DSMAX_ASCIIEXPORT" ) )
			{
			}

			else if( strstr( acLine , "COMMENT" ) )
			{
			}

			else if( strstr( acLine , "SCENE" ) )
			{
				if( !ReadSceneNode( f , fl ) ) bLoaded = false;
			}

			else if( strstr( acLine , "MATERIAL_LIST" ) )
			{
				if( !ReadMaterialListNode( f , fl ) ) bLoaded = false;
			}

			else if( strstr( acLine , "GEOMOBJECT" ) )
			{
				mesh = new Mesh;
				if( mesh != NULL )
				{
					mesh->s_bHasTexture = false;
					mesh->s_bHasMaterial = false;
					fl->s_MeshList.s_vMesh.push_back( mesh );
					if( !ReadGeomObjectNode( f , fl ) ) bLoaded = false;
				}
				else
				{
					bLoaded = false;
				}
			}
			fgets( acLine , 1000 , f );
		}
		fclose( f );
	}
	else
	{
		bLoaded	= false;
	}
	return bLoaded;
}


void addNormal(File & fObject, int m, float x, float y, float z) {
	Mesh* mesh = fObject.s_MeshList.s_vMesh[m];
	Point3D* normal;
	normal = new Point3D();
	normal->s_fXCoordinate = x;
	normal->s_fYCoordinate = y;
	normal->s_fZCoordinate = z;
	mesh->s_vNFace.push_back(normal);
}


void addVertexNormal(File & fObject, int m, Point3D *normal)
{
	Mesh* mesh = fObject.s_MeshList.s_vMesh[m];
	mesh->s_vNVertex.push_back(normal);
}

void updateVertexNormal(File & fObject, int m, int fv, Point3D *normal, int method)
{
	Mesh* mesh = fObject.s_MeshList.s_vMesh[m];
	if (method == 0) {

		Face3D* face = mesh->s_vFace[fv];

		mesh->s_vNVertex[face->s_uiVertexA]->s_fXCoordinate += normal->s_fXCoordinate;
		mesh->s_vNVertex[face->s_uiVertexA]->s_fYCoordinate += normal->s_fYCoordinate;
		mesh->s_vNVertex[face->s_uiVertexA]->s_fZCoordinate += normal->s_fZCoordinate;

		mesh->s_vNVertex[face->s_uiVertexB]->s_fXCoordinate += normal->s_fXCoordinate;
		mesh->s_vNVertex[face->s_uiVertexB]->s_fYCoordinate += normal->s_fYCoordinate;
		mesh->s_vNVertex[face->s_uiVertexB]->s_fZCoordinate += normal->s_fZCoordinate;

		mesh->s_vNVertex[face->s_uiVertexC]->s_fXCoordinate += normal->s_fXCoordinate;
		mesh->s_vNVertex[face->s_uiVertexC]->s_fYCoordinate += normal->s_fYCoordinate;
		mesh->s_vNVertex[face->s_uiVertexC]->s_fZCoordinate += normal->s_fZCoordinate;
	}
	else {
		mesh->s_vNVertex[fv]->s_fXCoordinate = normal->s_fXCoordinate;
		mesh->s_vNVertex[fv]->s_fYCoordinate = normal->s_fYCoordinate;
		mesh->s_vNVertex[fv]->s_fZCoordinate = normal->s_fZCoordinate;
	}
}

Point3D* getVertexNormal(File & fObject, int m, int v) {
	Point3D* normal = new Point3D();;
	Mesh* mesh = fObject.s_MeshList.s_vMesh[m];
	normal->s_fXCoordinate = mesh->s_vNVertex[v]->s_fXCoordinate;
	normal->s_fYCoordinate = mesh->s_vNVertex[v]->s_fYCoordinate;
	normal->s_fZCoordinate = mesh->s_vNVertex[v]->s_fZCoordinate;
	return normal;
}

Point3D getNormal(File & fObject, int m, int f) {
	Point3D normal;
	Mesh* mesh = fObject.s_MeshList.s_vMesh[m];

	normal.s_fXCoordinate = mesh->s_vNFace[f]->s_fXCoordinate;
	normal.s_fYCoordinate = mesh->s_vNFace[f]->s_fYCoordinate;
	normal.s_fZCoordinate = mesh->s_vNFace[f]->s_fZCoordinate;
	return normal;
}


Color getDifuse(File & fObject, int m) {
	Color color;
	Mesh* mesh = fObject.s_MeshList.s_vMesh[m];
	color.s_fRed = fObject.s_MatList.s_vMaterial[mesh->s_iMaterialId]->s_Diffuse.s_fRed;
	color.s_fGreen = fObject.s_MatList.s_vMaterial[mesh->s_iMaterialId]->s_Diffuse.s_fGreen;
	color.s_fBlue = fObject.s_MatList.s_vMaterial[mesh->s_iMaterialId]->s_Diffuse.s_fBlue;
	return color;
}

Color getAmbient(File & fObject, int m) {
	Color color;
	Mesh* mesh = fObject.s_MeshList.s_vMesh[m];
	color.s_fRed = fObject.s_MatList.s_vMaterial[mesh->s_iMaterialId]->s_Ambient.s_fRed;
	color.s_fGreen = fObject.s_MatList.s_vMaterial[mesh->s_iMaterialId]->s_Ambient.s_fGreen;
	color.s_fBlue = fObject.s_MatList.s_vMaterial[mesh->s_iMaterialId]->s_Ambient.s_fBlue;
	return color;
}

Color getSpecular(File & fObject, int m) {
	Color color;
	Mesh* mesh = fObject.s_MeshList.s_vMesh[m];
	color.s_fRed = fObject.s_MatList.s_vMaterial[mesh->s_iMaterialId]->s_Specular.s_fRed;
	color.s_fGreen = fObject.s_MatList.s_vMaterial[mesh->s_iMaterialId]->s_Specular.s_fGreen;
	color.s_fBlue = fObject.s_MatList.s_vMaterial[mesh->s_iMaterialId]->s_Specular.s_fBlue;
	return color;
}

float getShine(File & fObject, int m) {
	Mesh* mesh = fObject.s_MeshList.s_vMesh[m];
	float shine = fObject.s_MatList.s_vMaterial[mesh->s_iMaterialId]->s_fShineComponent*128.0f;
	return shine;
}

