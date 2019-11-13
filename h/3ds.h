#ifndef _3DS_H
#define _3DS_H

//>------ Primary Chunk, at the beginning of each file
#define PRIMARY       0x4D4D

//>------ Main Chunks
#define OBJECTINFO    0x3D3D				
#define VERSION       0x0002				
#define EDITKEYFRAME  0xB000				

//>------ sub defines of OBJECTINFO
#define MATERIAL	  0xAFFF				
#define OBJECT		  0x4000				

//>------ sub defines of MATERIAL
#define MATNAME       0xA000				
#define MATDIFFUSE    0xA020				
#define MATMAP        0xA200				
#define MATMAPFILE    0xA300				

#define OBJECT_MESH   0x4100				

//>------ sub defines of OBJECT_MESH
#define OBJECT_VERTICES     0x4110			
#define OBJECT_FACES		0x4120			
#define OBJECT_MATERIAL		0x4130			
#define OBJECT_UV			0x4140			

struct tIndices {							

	unsigned short a, b, c, bVisible;		
};

struct tChunk
{
	unsigned short int ID;					
	unsigned int length;					
	unsigned int bytesRead;					
};

class CLoad3DS
{
public:
	CLoad3DS();								
	bool Import3DS(t3DModel *pModel, char *strFileName);

private:
	int GetString(char *);
	void ReadChunk(tChunk *);
	void ProcessNextChunk(t3DModel *pModel, tChunk *);
	void ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *);
	void ProcessNextMaterialChunk(t3DModel *pModel, tChunk *);
	void ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk);
	void ReadVertices(t3DObject *pObject, tChunk *);
	void ReadVertexIndices(t3DObject *pObject, tChunk *);
	void ReadUVCoordinates(t3DObject *pObject, tChunk *);
	void ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk);
	void ComputeNormals(t3DModel *pModel);
	void CleanUp();
	
	FILE *m_FilePointer;
};


#endif

