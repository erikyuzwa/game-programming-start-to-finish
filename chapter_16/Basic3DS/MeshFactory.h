
#ifndef __MESHFACTORY_H_
#define __MESHFACTORY_H_

#include "PeonMain.h"

// Primary Chunk, at the beginning of each file
#define PRIMARY       0x4D4D

// Main Chunks
#define OBJECTINFO    0x3D3D				 
#define VERSION       0x0002				
#define EDITKEYFRAME  0xB000				

// Sub defines of OBJECTINFO
#define MATERIAL	  0xAFFF				
#define OBJECT		  0x4000				

// Sub defines of MATERIAL
#define MATNAME       0xA000				
#define MATDIFFUSE    0xA020				
#define MATMAP        0xA200				
#define MATMAPFILE    0xA300				

#define OBJECT_MESH   0x4100				

// Sub defines of OBJECT_MESH
#define OBJECT_VERTICES     0x4110			
#define OBJECT_FACES		0x4120			
#define OBJECT_MATERIAL		0x4130			
#define OBJECT_UV			0x4140	

		
namespace peon
{


//////////////////////////////////////
//The sFace Struct
//////////////////////////////////////
struct sFace
{
	int vertIndex[3];							
	int coordIndex[3];							
};

//////////////////////////////////////
//The sMaterialInfo Struct
//////////////////////////////////////
struct sMaterialInfo
{
	char  strName[255];							
	char  strFile[255];							
	BYTE  color[3];								
	int   texureId;								
	float uTile;								
	float vTile;								
	float uOffset;								
	float vOffset;									
};

//////////////////////////////////////
//The s3DObject Struct
//////////////////////////////////////
struct s3DObject 
{
	int  numOfVerts;			
	int  numOfFaces;			
	int  numTexVertex;			
	int  materialID;			
	bool bHasTexture;			
	char strName[255];			
	UINT      *pIndices;		
	Vector3  *pVerts;			
	Vector3  *pNormals;		
	Vector2  *pTexVerts;		
	sFace *pFaces;				
};

//////////////////////////////////////
//The s3DModel Struct
//////////////////////////////////////
struct s3DModel 
{
	int numOfObjects;							
	int numOfMaterials;							
	vector<sMaterialInfo> pMaterials;			
	vector<s3DObject> pObject;					
};





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Here is our structure for our 3DS indicies (since .3DS stores 4 unsigned shorts)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct sIndices {							

	unsigned short a, b, c, bVisible;		
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This holds the chunk info
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct sChunk
{
	unsigned short int ID;					
	unsigned int length;					
	unsigned int bytesRead;					
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This class handles all of the loading code
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MeshFactory
{
public:
	MeshFactory();
	
	~MeshFactory();								

	bool Import3DS(s3DModel *pModel, char *strFileName);

private:
	int GetString(char *);

	void ReadChunk(sChunk *);

	void ProcessNextChunk(s3DModel *pModel, sChunk *);

	void ProcessNextObjectChunk(s3DModel *pModel, s3DObject *pObject, sChunk *);

	void ProcessNextMaterialChunk(s3DModel *pModel, sChunk *);

	void ReadColorChunk(sMaterialInfo *pMaterial, sChunk *pChunk);

	void ReadVertices(s3DObject *pObject, sChunk *);

	void ReadVertexIndices(s3DObject *pObject, sChunk *);

	void ReadUVCoordinates(s3DObject *pObject, sChunk *);

	void ReadObjectMaterial(s3DModel *pModel, s3DObject *pObject, sChunk *pPreviousChunk);
	
	void ComputeNormals(s3DModel *pModel);

	void CleanUp();
	
	FILE *m_FilePointer;
	
	sChunk *m_CurrentChunk;
	sChunk *m_TempChunk;
};

}

#endif


