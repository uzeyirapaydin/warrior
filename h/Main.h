#ifndef _MAIN_H
#define _MAIN_H


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment( lib, "winmm.lib" )	
#pragma comment(lib,  "fmodvc.lib")

#include "fmod.h"
#include "fmod_errors.h"

#include <iostream>
#include <string>
using namespace std;
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>			
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <vector>

#include <gl\gl.h>										
#include <gl\glu.h>										
#include "glaux.h"
#include <crtdbg.h>

#include <math.h>
	
#include "bmp.h"


#define PI 3.141592653589
#define CAMERASPEED	0.02f				

#define MAX_TEXTURES 5					

class CVector3 
{
public:
	float x, y, z;
};


class CVector2 
{
public:
	float x, y;
};

struct tFace
{
	int vertIndex[3];			
	int coordIndex[3];			
};

struct tMaterialInfo
{
	char  strName[255];			
	char  strFile[255];			
	BYTE  color[3];				
	int   texureId;				
	float uTile;				
	float vTile;				
	float uOffset;			    
	float vOffset;				
} ;


struct t3DObject 
{
	int  numOfVerts;			
	int  numOfFaces;			
	int  numTexVertex;			
	int  materialID;			
	bool bHasTexture;			
	char strName[255];			
	CVector3  *pVerts;			
	CVector3  *pNormals;		
	CVector2  *pTexVerts;		
	tFace *pFaces;				
};


struct t3DModel 
{
	int numOfObjects;					
	int numOfMaterials;					
	vector<tMaterialInfo> pMaterials;	
	vector<t3DObject> pObject;			
};
struct tAnimationInfo
{
	char strName[255];			// This stores the name of the animation (Jump, Pain, etc..)
	int startFrame;				// This stores the first frame number for this animation
	int endFrame;				// This stores the last frame number for this animation
};


struct t3DModelMd2 
{
	int numOfObjects;					// The number of objects in the model
	int numOfMaterials;					// The number of materials for the model
	int numOfAnimations;				// The number of animations in this model (NEW)
	int currentAnim;					// The current index into pAnimations list (NEW)
	int currentFrame;					// The current frame of the current animation (NEW)
	vector<tAnimationInfo> pAnimations; // The list of animations (NEW)
	vector<tMaterialInfo> pMaterials;	// The list of material information (Textures and colors)
	vector<t3DObject> pObject;			// The object list for our model
};
typedef struct tVector3					
{			
	tVector3() {}	// constructor
	tVector3 (float new_x, float new_y, float new_z) 
	{x = new_x; y = new_y; z = new_z;}

	tVector3 operator+(tVector3 vVector) {return tVector3(vVector.x+x, vVector.y+y, vVector.z+z);}

	tVector3 operator-(tVector3 vVector) {return tVector3(x-vVector.x, y-vVector.y, z-vVector.z);}

	tVector3 operator*(float number)	 {return tVector3(x*number, y*number, z*number);}

	tVector3 operator/(float number)	 {return tVector3(x/number, y/number, z/number);}
	
	float x, y, z;						
}tVector3;

class CCamera 
{
	public:
		tVector3 mPos;							
		tVector3 mView;							
		tVector3 mUp;		
		GLfloat ang;
		CCamera();
		void Rotate_Position(float speed);

		void Move_Camera(float speed);
		void Mouse_Move(int wndWidth, int wndHeight);
		void Rotate_View(float speed);
		void Position_Camera(float pos_x,  float pos_y,  float pos_z,
							 float view_x, float view_y, float view_z,
							 float up_x,   float up_y,   float up_z);
		bool actif,mouse_moved;
};

extern	HDC			hDC;				// Device context
extern	HGLRC		hRC;				// Permanent rendering context
extern	HWND		hWnd;				// Holds our window handle
extern	HINSTANCE	hInstance;			// Holds the instance of the application

void Keyboard_Input();
void Draw_Grid();

#endif
