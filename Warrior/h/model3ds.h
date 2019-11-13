#ifndef _MODEL3DS_H
#define _MODEL3DS_H
#define TEXT_N_3ds 13
char MldNames3ds[TEXT_N_3ds][50]={"tree.3ds","box.3ds",
	"model01.3ds",	"sky.3ds",		"model02.3ds"
	,"model03.3ds",	"model04.3DS",
	"model05.3ds",	"model06.3ds","health.3ds","floor.3ds","floor1.3ds","floor2.3ds"};
UINT g_Texture3ds[TEXT_N_3ds][MAX_TEXTURES] = {0};

CLoad3DS g_Load3ds[TEXT_N_3ds];										
t3DModel g_3DModel[TEXT_N_3ds];	

void Draw3ds(int n){
for(int i = 0; i < g_3DModel[n].numOfObjects; i++)
	{
		if(g_3DModel[n].pObject.size() <= 0) break;

		t3DObject *pObject = &g_3DModel[n].pObject[i];
		if(pObject->bHasTexture) {
			glEnable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
			glBindTexture(GL_TEXTURE_2D, g_Texture3ds[n][pObject->materialID]);
		} else {
			glDisable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
		}
		glBegin(g_ViewMode);
			for(int j = 0; j < pObject->numOfFaces; j++){
				for(int whichVertex = 0; whichVertex < 3; whichVertex++){
					int index = pObject->pFaces[j].vertIndex[whichVertex];
					glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);
					if(pObject->bHasTexture) {
						if(pObject->pTexVerts) {
							glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
						}
					} else {
						if(g_3DModel[n].pMaterials.size() && pObject->materialID >= 0) 
						{
							BYTE *pColor = g_3DModel[n].pMaterials[pObject->materialID].color;
							glColor3ub(pColor[0], pColor[1], pColor[2]);
						}
					}
					glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
				}
			}
	}
	glEnd();
}


void remove3ds(int n){
	for(int i = 0; i < g_3DModel[n].numOfObjects; i++)
	{
		delete [] g_3DModel[n].pObject[i].pFaces;
		delete [] g_3DModel[n].pObject[i].pNormals;
		delete [] g_3DModel[n].pObject[i].pVerts;
		delete [] g_3DModel[n].pObject[i].pTexVerts;
	}
}
void get3ds(int n){
	for(int i = 0; i < g_3DModel[n].numOfMaterials; i++){
		if(strlen(g_3DModel[n].pMaterials[i].strFile) > 0)
				CreateTexture(g_Texture3ds[n], g_3DModel[n].pMaterials[i].strFile, i);			
		g_3DModel[n].pMaterials[i].texureId = i;
	}
}

//class Model3ds{
//public:
//Model3ds(char* fname);
//~Model3ds();
//void get3ds();
//void Draw3ds();
//
//CLoad3DS g_Load3ds;
//t3DModel g_3DModel3ds;
//
//UINT g_Texture3ds[MAX_TEXTURES];
//};
//
//
//Model3ds::~Model3ds(){
//	for(int i = 0; i < g_3DModel3ds.numOfObjects; i++){
//		delete [] g_3DModel3ds.pObject[i].pFaces;
//		delete [] g_3DModel3ds.pObject[i].pNormals;
//		delete [] g_3DModel3ds.pObject[i].pVerts;
//		delete [] g_3DModel3ds.pObject[i].pTexVerts;
//	}
//}
//Model3ds::Model3ds(char* fname){
//	g_Load3ds.Import3DS(&g_3DModel3ds,fname);		
//	get3ds();
//}
//
//void Model3ds::get3ds(){
//	
//	for(int i = 0; i < g_3DModel3ds.numOfMaterials; i++){
//		if(strlen(g_3DModel3ds.pMaterials[i].strFile) > 0)
//			CreateTexture(g_Texture3ds, g_3DModel3ds.pMaterials[i].strFile, i);			
//		g_3DModel3ds.pMaterials[i].texureId = i;
//	}
//}
//
//void Model3ds::Draw3ds(){
//
//for(int i = 0; i < g_3DModel3ds.numOfObjects; i++)
//	{
//		if(g_3DModel3ds.pObject.size() <= 0) break;
//
//		t3DObject *pObject = &g_3DModel3ds.pObject[i];
//		if(pObject->bHasTexture) {
//			glEnable(GL_TEXTURE_2D);
//			glColor3ub(255, 255, 255);
//			glBindTexture(GL_TEXTURE_2D, g_Texture3ds[pObject->materialID]);
//		} else {
//			glDisable(GL_TEXTURE_2D);
//			glColor3ub(255, 255, 255);
//		}
//		glBegin(g_ViewMode);
//			for(int j = 0; j < pObject->numOfFaces; j++){
//				for(int whichVertex = 0; whichVertex < 3; whichVertex++){
//					int index = pObject->pFaces[j].vertIndex[whichVertex];
//					glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);
//					if(pObject->bHasTexture) {
//						if(pObject->pTexVerts) {
//							glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
//						}
//					} else {
//						if(g_3DModel3ds.pMaterials.size() && pObject->materialID >= 0) 
//						{
//							BYTE *pColor = g_3DModel3ds.pMaterials[pObject->materialID].color;
//							glColor3ub(pColor[0], pColor[1], pColor[2]);
//						}
//					}
//					glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
//				}
//			}
//	}
//	glEnd();
//}

#endif