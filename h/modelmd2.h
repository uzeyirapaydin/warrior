#ifndef _MODELMD2_H
#define _MODELMD2_H
int   g_ViewMode	  = GL_TRIANGLES;	
void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID){
	AUX_RGBImageRec *pBitmap = NULL;
	if(!strFileName)									
		return;
	pBitmap = auxDIBImageLoad(strFileName);				
	if(pBitmap == NULL)									
		exit(0);
	glGenTextures(1, &textureArray[textureID]);
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	if (pBitmap){
		if (pBitmap->data){
			free(pBitmap->data);						 
		}
		free(pBitmap);									
	}
}
class ModelMd2{
public:
ModelMd2(char* fname,char* tname );
~ModelMd2();
float ReturnCurrentTime(int nextFrame);
void AnimateMD2Model();
void nextAnimation();
void makeAnimationmd2(int ani);
void getmd2();

CLoadMD2 g_LoadMd2;										
t3DModelMd2 g_3DModelMd2;	
UINT g_Texturemd2[MAX_TEXTURES];
float elapsedTime;
float lastTime;
};

ModelMd2::ModelMd2(char* fname,char* tname ){
	g_LoadMd2.ImportMD2(&g_3DModelMd2,fname, tname);		
	getmd2();
	elapsedTime   = 0.0f;
	lastTime	  = 0.0f;
}

float ModelMd2::ReturnCurrentTime(int nextFrame)
{
	float time = (float)GetTickCount();
	elapsedTime = time - lastTime;
	float t = elapsedTime / (1000.0f / kAnimationSpeed);
	if (elapsedTime >= (1000.0f / kAnimationSpeed) )
	{
		g_3DModelMd2.currentFrame = nextFrame;
		lastTime = time;
	}
	return t;
}
void ModelMd2::nextAnimation(){
makeAnimationmd2((g_3DModelMd2.currentAnim + 1) % (g_3DModelMd2.numOfAnimations));
}
void ModelMd2::AnimateMD2Model(){ 

	glBindTexture(GL_TEXTURE_2D, g_Texturemd2[0]);
	if(g_3DModelMd2.pObject.size() <= 0) return;
	tAnimationInfo *pAnim = &(g_3DModelMd2.pAnimations[g_3DModelMd2.currentAnim]);
	int nextFrame = (g_3DModelMd2.currentFrame + 1) % pAnim->endFrame;
	if(nextFrame == 0) 
		nextFrame =  pAnim->startFrame;
	t3DObject *pFrame =		 &g_3DModelMd2.pObject[g_3DModelMd2.currentFrame];

	t3DObject *pNextFrame =  &g_3DModelMd2.pObject[nextFrame];

	t3DObject *pFirstFrame = &g_3DModelMd2.pObject[0];
	float t = ReturnCurrentTime(nextFrame);
	glBegin(g_ViewMode);
		for(int j = 0; j < pFrame->numOfFaces; j++)
		{
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				int vertIndex = pFirstFrame->pFaces[j].vertIndex[whichVertex];

				int texIndex  = pFirstFrame->pFaces[j].coordIndex[whichVertex];
		
				if(pFirstFrame->pTexVerts) 
				{
					glTexCoord2f(pFirstFrame->pTexVerts[ texIndex ].x, 
				 pFirstFrame->pTexVerts[ texIndex ].y);
				}

				CVector3 vPoint1 = pFrame->pVerts[ vertIndex ];
				CVector3 vPoint2 = pNextFrame->pVerts[ vertIndex ];
				glVertex3f(vPoint1.x + t * (vPoint2.x - vPoint1.x), // Find the interpolated X
						   vPoint1.y + t * (vPoint2.y - vPoint1.y), // Find the interpolated Y
						   vPoint1.z + t * (vPoint2.z - vPoint1.z));// Find the interpolated Z
			}
		}
	glEnd();	
}

ModelMd2::~ModelMd2(){
for(int i = 0; i < g_3DModelMd2.numOfObjects; i++)
	{
		if(g_3DModelMd2.pObject[i].pFaces)		
			delete [] g_3DModelMd2.pObject[i].pFaces;
		if(g_3DModelMd2.pObject[i].pNormals)	
			delete [] g_3DModelMd2.pObject[i].pNormals;
		if(g_3DModelMd2.pObject[i].pVerts)		
			delete [] g_3DModelMd2.pObject[i].pVerts;
		if(g_3DModelMd2.pObject[i].pTexVerts)	
			delete [] g_3DModelMd2.pObject[i].pTexVerts;
	}
}

void ModelMd2::makeAnimationmd2(int ani){
	ani=ani%(g_3DModelMd2.numOfAnimations);
if(g_3DModelMd2.currentAnim != ani){
	g_3DModelMd2.currentAnim = ani;
	g_3DModelMd2.currentFrame = g_3DModelMd2.pAnimations[g_3DModelMd2.currentAnim].startFrame;
}
}
void ModelMd2::getmd2(){
for(int i = 0; i < g_3DModelMd2.numOfMaterials; i++)
	{
		if(strlen(g_3DModelMd2.pMaterials[i].strFile) > 0)
		{
			CreateTexture(g_Texturemd2, g_3DModelMd2.pMaterials[i].strFile, i);			
		}
		g_3DModelMd2.pMaterials[i].texureId = i;
	}
}
#endif