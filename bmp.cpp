#include "h\bmp.h"
void BMP_Texture(UINT textureArray[], LPSTR strFileName, int ID)
{
	if(!strFileName)   return;
	
	AUX_RGBImageRec *pBitMap = auxDIBImageLoad(strFileName);
	
	if(pBitMap == NULL)	exit(0);

	glGenTextures(1, &textureArray[ID]);
	glBindTexture(GL_TEXTURE_2D, textureArray[ID]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitMap->sizeX, pBitMap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitMap->data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	if (pBitMap)										
	{
		if (pBitMap->data)								
		{
			free(pBitMap->data);						
		}
		free(pBitMap);									
	}
}
