#include "h\main.h"
#include "h\3ds.h"
#include <assert.h>

int gBuffer[50000] = {0};					
CLoad3DS::CLoad3DS()
{
	m_FilePointer = NULL;
}
bool CLoad3DS::Import3DS(t3DModel *pModel, char *strFileName)
{
	char strMessage[255] = {0};
	tChunk currentChunk = {0};

	m_FilePointer = fopen(strFileName, "rb");

	if(!m_FilePointer) 
	{
		sprintf(strMessage, "Unable to find the file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}
	ReadChunk(&currentChunk);

	if (currentChunk.ID != PRIMARY)
	{
		sprintf(strMessage, "Unable to load PRIMARY chuck from file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}
	ProcessNextChunk(pModel, &currentChunk);

	ComputeNormals(pModel);

	CleanUp();

	return true;
}
void CLoad3DS::CleanUp()
{
	if (m_FilePointer) {
		fclose(m_FilePointer);					
		m_FilePointer = NULL;
	}
}
void CLoad3DS::ProcessNextChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
	t3DObject newObject = {0};					
	tMaterialInfo newTexture = {0};				

	tChunk currentChunk = {0};					
	tChunk tempChunk = {0};						
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		ReadChunk(&currentChunk);

		switch (currentChunk.ID)
		{
		case VERSION:							
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);

			if ((currentChunk.length - currentChunk.bytesRead == 4) && (gBuffer[0] > 0x03)) {
				MessageBox(NULL, "This 3DS file is over version 3 so it may load incorrectly", "Warning", MB_OK);
			}
			break;

		case OBJECTINFO:						
			{	
			ReadChunk(&tempChunk);

			tempChunk.bytesRead += fread(gBuffer, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer);

			currentChunk.bytesRead += tempChunk.bytesRead;

			ProcessNextChunk(pModel, &currentChunk);
			break;
		}
		case MATERIAL:							
			pModel->numOfMaterials++;
			pModel->pMaterials.push_back(newTexture);
			ProcessNextMaterialChunk(pModel, &currentChunk);
			break;

		case OBJECT:							
			pModel->numOfObjects++;
			pModel->pObject.push_back(newObject);
			memset(&(pModel->pObject[pModel->numOfObjects - 1]), 0, sizeof(t3DObject));
			currentChunk.bytesRead += GetString(pModel->pObject[pModel->numOfObjects - 1].strName);
			ProcessNextObjectChunk(pModel, &(pModel->pObject[pModel->numOfObjects - 1]), &currentChunk);
			break;
		case EDITKEYFRAME:
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		default: 
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		}
		pPreviousChunk->bytesRead += currentChunk.bytesRead;
	}
}
void CLoad3DS::ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
	tChunk currentChunk = {0};

	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		ReadChunk(&currentChunk);
		switch (currentChunk.ID)
		{
		case OBJECT_MESH:					
			ProcessNextObjectChunk(pModel, pObject, &currentChunk);
			break;

		case OBJECT_VERTICES:				
			ReadVertices(pObject, &currentChunk);
			break;

		case OBJECT_FACES:					
			ReadVertexIndices(pObject, &currentChunk);
			break;

		case OBJECT_MATERIAL:				
			ReadObjectMaterial(pModel, pObject, &currentChunk);			
			break;

		case OBJECT_UV:						
			ReadUVCoordinates(pObject, &currentChunk);
			break;

		default:  
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		}
		pPreviousChunk->bytesRead += currentChunk.bytesRead;
	}
}
void CLoad3DS::ProcessNextMaterialChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
	tChunk currentChunk = {0};
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		ReadChunk(&currentChunk);
		switch (currentChunk.ID)
		{
		case MATNAME:							
			currentChunk.bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strName, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;

		case MATDIFFUSE:						
			ReadColorChunk(&(pModel->pMaterials[pModel->numOfMaterials - 1]), &currentChunk);
			break;
		
		case MATMAP:							
			ProcessNextMaterialChunk(pModel, &currentChunk);
			break;

		case MATMAPFILE:						
			currentChunk.bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strFile, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		
		default:  
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		}
		pPreviousChunk->bytesRead += currentChunk.bytesRead;
	}
}
void CLoad3DS::ReadChunk(tChunk *pChunk)
{
	pChunk->bytesRead = fread(&pChunk->ID, 1, 2, m_FilePointer);
	pChunk->bytesRead += fread(&pChunk->length, 1, 4, m_FilePointer);
}
int CLoad3DS::GetString(char *pBuffer)
{
	int index = 0;
	fread(pBuffer, 1, 1, m_FilePointer);
	while (*(pBuffer + index++) != 0) {
		fread(pBuffer + index, 1, 1, m_FilePointer);
	}
	return strlen(pBuffer) + 1;
}
void CLoad3DS::ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk)
{
	tChunk tempChunk = {0};
	ReadChunk(&tempChunk);
	tempChunk.bytesRead += fread(pMaterial->color, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer);
	pChunk->bytesRead += tempChunk.bytesRead;
}

void CLoad3DS::ReadVertexIndices(t3DObject *pObject, tChunk *pPreviousChunk)
{
	unsigned short index = 0;					

	pPreviousChunk->bytesRead += fread(&pObject->numOfFaces, 1, 2, m_FilePointer);

	pObject->pFaces = new tFace [pObject->numOfFaces];
	memset(pObject->pFaces, 0, sizeof(tFace) * pObject->numOfFaces);

	for(int i = 0; i < pObject->numOfFaces; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			pPreviousChunk->bytesRead += fread(&index, 1, sizeof(index), m_FilePointer);

			if(j < 3)
			{
				pObject->pFaces[i].vertIndex[j] = index;
			}
		}
	}
}


void CLoad3DS::ReadUVCoordinates(t3DObject *pObject, tChunk *pPreviousChunk)
{
	pPreviousChunk->bytesRead += fread(&pObject->numTexVertex, 1, 2, m_FilePointer);
	pObject->pTexVerts = new CVector2 [pObject->numTexVertex];
	pPreviousChunk->bytesRead += fread(pObject->pTexVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}


void CLoad3DS::ReadVertices(t3DObject *pObject, tChunk *pPreviousChunk)
{
	pPreviousChunk->bytesRead += fread(&(pObject->numOfVerts), 1, 2, m_FilePointer);

	pObject->pVerts = new CVector3 [pObject->numOfVerts];
	memset(pObject->pVerts, 0, sizeof(CVector3) * pObject->numOfVerts);

	pPreviousChunk->bytesRead += fread(pObject->pVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);

	for(int i = 0; i < pObject->numOfVerts; i++)
	{
		float fTempY = pObject->pVerts[i].y;

		pObject->pVerts[i].y = pObject->pVerts[i].z;

		pObject->pVerts[i].z = -fTempY;
	}
}

void CLoad3DS::ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
	char strMaterial[255] = {0};			// This is used to hold the objects material name

	pPreviousChunk->bytesRead += GetString(strMaterial);

	for(int i = 0; i < pModel->numOfMaterials; i++)
	{
	
		if(strcmp(strMaterial, pModel->pMaterials[i].strName) == 0)
		{
			pObject->materialID = i;
			if(strlen(pModel->pMaterials[i].strFile) > 0) {
			pObject->bHasTexture = true;
			}	
			break;
		}
		else
		{
			pObject->materialID = -1;
		}
	}

	pPreviousChunk->bytesRead += fread(gBuffer, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}			

#define Mag(Normal) (sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))

CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2)
{
	CVector3 vVector;						

	vVector.x = vPoint1.x - vPoint2.x;		
	vVector.y = vPoint1.y - vPoint2.y;		
	vVector.z = vPoint1.z - vPoint2.z;		

	return vVector;							
}

CVector3 AddVector(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vResult;							
	
	vResult.x = vVector2.x + vVector1.x;		
	vResult.y = vVector2.y + vVector1.y;		
	vResult.z = vVector2.z + vVector1.z;		

	return vResult;								
}

CVector3 DivideVectorByScaler(CVector3 vVector1, float Scaler)
{
	CVector3 vResult;							
	
	vResult.x = vVector1.x / Scaler;			
	vResult.y = vVector1.y / Scaler;			
	vResult.z = vVector1.z / Scaler;			

	return vResult;								
}


CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vCross;							
	vCross.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vCross.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vCross.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));
	return vCross;								
}

CVector3 Normalize(CVector3 vNormal)
{
	double Magnitude;							

	Magnitude = Mag(vNormal);					

	vNormal.x /= (float)Magnitude;				
	vNormal.y /= (float)Magnitude;				
	vNormal.z /= (float)Magnitude;				

	return vNormal;								
}

void CLoad3DS::ComputeNormals(t3DModel *pModel)
{
	CVector3 vVector1, vVector2, vNormal, vPoly[3];

	if(pModel->numOfObjects <= 0)
		return;
	for(int index = 0; index < pModel->numOfObjects; index++)
	{
		t3DObject *pObject = &(pModel->pObject[index]);

		CVector3 *pNormals		= new CVector3 [pObject->numOfFaces];
		CVector3 *pTempNormals	= new CVector3 [pObject->numOfFaces];
		pObject->pNormals		= new CVector3 [pObject->numOfVerts];

		for(int i=0; i < pObject->numOfFaces; i++)
		{												
			vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
			vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
			vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];
			vVector1 = Vector(vPoly[0], vPoly[2]);		
			vVector2 = Vector(vPoly[2], vPoly[1]);		

			vNormal  = Cross(vVector1, vVector2);		
			pTempNormals[i] = vNormal;					
			vNormal  = Normalize(vNormal);				

			pNormals[i] = vNormal;						
		}
		CVector3 vSum = {0.0, 0.0, 0.0};
		CVector3 vZero = vSum;
		int shared=0;

		for (int i = 0; i < pObject->numOfVerts; i++)			
		{
			for (int j = 0; j < pObject->numOfFaces; j++)	
			{												
				if (pObject->pFaces[j].vertIndex[0] == i || 
					pObject->pFaces[j].vertIndex[1] == i || 
					pObject->pFaces[j].vertIndex[2] == i)
				{
					vSum = AddVector(vSum, pTempNormals[j]);
					shared++;								
				}
			}      
			
			pObject->pNormals[i] = DivideVectorByScaler(vSum, float(-shared));

			pObject->pNormals[i] = Normalize(pObject->pNormals[i]);	

			vSum = vZero;									
			shared = 0;										
		}
		delete [] pTempNormals;
		delete [] pNormals;
	}
}
