#ifndef _TGA_H
#define _TGA_H


#include "main.h"


#define TGA_RGB		2
#define TGA_A		3
#define TGA_RLE		10


typedef struct													// Create A Structure
{
	GLubyte	*imageData;											// Image Data (Up To 32 Bits)
	GLuint	bpp;												// Image Color Depth In Bits Per Pixel.
	GLuint	width;												// Image Width
	GLuint	height;												// Image Height
	GLuint	texID;												// Texture ID Used To Select A Texture
} TextureImage;	

struct tImageTGA
{
	int channels;
	int size_x;	
	int size_y;				
	unsigned char *data;
};


bool LoadTGAFile(TextureImage *texture, char *filename);
tImageTGA *Load_TGA(const char *filename);
void TGA_Texture(UINT textureArray[], LPSTR strFileName, int textureID);



#endif



// www.morrowland.com
// apron@morrowland.com