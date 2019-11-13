#include "h\main.h"
#include "h\3ds.h"	
#include "h\Md2.h"
#include "h\tga.h"
#include <stdio.h>

#include "h\modelmd2.h"
#include "h\model3ds.h"
#include "h\character.h"

#define SCREEN_HEIGHT 768
#define SCREEN_WIDTH 1024
#define MAX_MP3 6

struct MapHeader{
GLfloat NumOfObjects, Width,Height,Fog,FloorTexture,StaticObj,AnimatedObj;
int Pos,AnimPos,StaticPos;
}Header;
												
struct MapObject{
GLfloat X,Y,R,Type,No,Health;
}Objects;

int PlayingMp3No=0,menuItem=-1,HeroPow;
FILE* file;

HDC			hDC=NULL;		
HGLRC		hRC=NULL;		
HWND		hWnd=NULL;		
HINSTANCE	hInstance;		
#define MAP_N 4
#define DETAIL 42.978f
char MapNames[MAP_N][50];

Character **Anim;
StaticCharacter **Stat;
float g_FogDensity = 0.0050f;								
bool  g_bFog = true,hit=false;	
int map_no=-1;

bool	keys[256],keyPressed[256];			
bool	active=TRUE,can_go=true;		
bool	fullscreen=TRUE;
bool map_loaded=true,menu=true;
GLuint	base3D,base2D;	

GLYPHMETRICSFLOAT gmf[256];	
UINT TextureArray[9];	
TextureImage textures[10];	

GLfloat fogColor[8][4]={ {1.0f,0.2f,0.2f,1.0f},
                         {0.0f,0.1f,0.8f,1.0f},
                         {0.0f,0.0f,0.0f,1.0f},
                         {0.6f,0.7f,0.67f,1.0f},
                         {0.9f,0.2f,0.0f,1.0f},
                         {0.9f,0.2f,0.0f,1.0f},
                         {0.9f,0.2f,0.0f,1.0f},
                         {0.9f,0.2f,0.0f,1.0f}};

float g_LightPosition[] = {3.0f,15.0f,3.0f, 1.0f};		
	
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	

char* FPS();
static float listenerPos[] = {0.0,0.0,0.0};
static float listenerVel[] = {0.0,0.0,0.0};
static float listenerOri[] = {0.0,0.0,-1.0, 0.0,1.0,0.0};

CCamera objCamera; 

ModelMd2 *hero;

FMOD_Sample **smp;
FMOD_Stream **str;

void DrawGrid();
void FollowCamera();
void CreateSkyBox();
int DrawGLScene(GLvoid);
void MapClose();
void DeleteModels();
bool MapOpen();
void MapSave();
void CheckHit();
void RectangleTexture(GLfloat X,GLfloat Y,GLfloat H,GLfloat W,UINT TextureID );
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	
void find_y(double m,int &x,int &y);
GLvoid BuildFont3D(GLvoid){
	HFONT	font;										
	base3D = glGenLists(256);								
	font = CreateFont(	-12,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_DONTCARE|DEFAULT_PITCH,"Arial");				
	SelectObject(hDC, font);							
	wglUseFontOutlines(	hDC,0,255,base3D,0.0f,0.2f,WGL_FONT_POLYGONS,gmf);							
}
GLvoid BuildFont2D(GLvoid)										// Build Our Font Display List
{
	base2D=glGenLists(95);										// Creating 95 Display Lists
	glBindTexture(GL_TEXTURE_2D, textures[9].texID);			// Bind Our Font Texture
	for (int loop=0; loop<95; loop++)							// Loop Through All 95 Lists
	{
		float cx=float(loop%16)/16.0f;							// X Position Of Current Character
		float cy=float(loop/16)/8.0f;							// Y Position Of Current Character


		glNewList(base2D+loop,GL_COMPILE);						// Start Building A List
			glBegin(GL_QUADS);									// Use A Quad For Each Character
				glTexCoord2f(cx,         1.0f-cy-0.120f); glVertex2i(0,16);	// Texture / Vertex Coord (Bottom Left)
				glTexCoord2f(cx+0.0625f, 1.0f-cy-0.120f); glVertex2i(16,16);	// Texutre / Vertex Coord (Bottom Right)
				glTexCoord2f(cx+0.0625f, 1.0f-cy);		  glVertex2i(16,0);// Texture / Vertex Coord (Top Right)
				glTexCoord2f(cx,         1.0f-cy);		  glVertex2i(0,0);	// Texture / Vertex Coord (Top Left)
			glEnd();											// Done Building Our Quad (Character)
			glTranslated(10,0,0);								// Move To The Right Of The Character
		glEndList();											// Done Building The Display List
	}															// Loop Until All 256 Are Built
}

GLvoid glPrint2D(GLint x, GLint y, const char *string, ...)		// Where The Printing Happens
{
	char		text[256];										// Holds Our String
	va_list		ap;												// Pointer To List Of Arguments

	if (string == NULL)											// If There's No Text
		return;													// Do Nothing

	va_start(ap, string);										// Parses The String For Variables
	    vsprintf(text, string, ap);								// And Converts Symbols To Actual Numbers
	va_end(ap);													// Results Are Stored In Text

	glBindTexture(GL_TEXTURE_2D, textures[9].texID);			// Select Our Font Texture
	glPushMatrix();												// Store The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
	glTranslated(x,y,0);										// Position The Text (0,0 - Bottom Left)
	glListBase(base2D-32);										// Choose The Font Set
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);			// Draws The Display List Text
	glPopMatrix();												// Restore The Old Projection Matrix
}
GLvoid KillFont3D(GLvoid){ glDeleteLists(base3D, 256);}

GLvoid glPrint3D(const char *fmt, ...)					
{
	float		length=0;								
	char		text[256];								
	va_list		ap;										

	if (fmt == NULL)return;											

	va_start(ap, fmt);									
    vsprintf(text, fmt, ap);						
	va_end(ap);											
	for (int i=0;i<strlen(text);i++)
		length+=gmf[text[i]].gmfCellIncX;			
	glTranslatef(-length/2,0.0f,0.0f);					
	glPushAttrib(GL_LIST_BIT);							
	glListBase(base3D);									
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	
	glPopAttrib();										
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height){
	if (height==0)										
		height=1;										
	glViewport(0,0,width,height);						
	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();									
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,10.0f,800.0f);
	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();									
}

void OrthoMode(int left, int top, int right, int bottom)
{
	glMatrixMode(GL_PROJECTION);						
	glPushMatrix();									
	glLoadIdentity();								
	glOrtho( left, right, bottom, top, 0, 1 );	
	glMatrixMode(GL_MODELVIEW);								
	glLoadIdentity();										
}
void PerspectiveMode()										// Set Up A Perspective View
{
	glMatrixMode( GL_PROJECTION );							
	glPopMatrix();											
	glMatrixMode( GL_MODELVIEW );							
}

void DrawHero(){
	hero->AnimateMD2Model();
}

void MapSave(){
int i=0;
if((file=fopen("map\\saved.map","w+"))==NULL)
return;

fseek(file,0,SEEK_SET);
fprintf(file,"%3.0f%3.0f%3.0f%4.0f%4.0f%1.0f%1.0f",Header.NumOfObjects,Header.AnimatedObj,Header.StaticObj,Header.Width,Header.Height,Header.Fog, Header.FloorTexture);
for(i=0; i<Header.AnimatedObj;i++){
    fprintf(file,"%4.0f%4.0f%1.0f%1.0f%2.0f%3.0f",Anim[i]->p.x+Header.Width/2, Anim[i]->p.z+Header.Height/2,Anim[i]->r,0.0,Anim[i]->ObjNo, Anim[i]->health);
}
for(i=0; i<Header.StaticObj;i++){
    fprintf(file,"%4.0f%4.0f%1.0f%1.0f%2.0f%3.0f",Stat[i]->p.x+Header.Width/2, Stat[i]->p.z+Header.Height/2,Stat[i]->r,1.0,Stat[i]->ObjNo, Stat[i]->health);
}
i=0;
fprintf(file,"%4.0f%4.0f%1.0f%1.0f%2.0f%3.0f",Stat[i]->p.x+Header.Width/2, Stat[i]->p.z+Header.Height/2,Stat[i]->r,2.0,Stat[i]->ObjNo, Stat[i]->health);
fclose(file);
}

bool MapOpen(bool type){
	char temp[6];
    map_loaded=false;
    if(type){
	if((file=fopen(MapNames[map_no],"r"))==NULL)
        return false;
    }
    else
     if((file=fopen("map\\saved.map","r"))==NULL)

    fseek(file,0,SEEK_SET);
    fgets(temp,4,file);
	Header.NumOfObjects=atof(temp);
    fgets(temp,4,file);
    Header.AnimatedObj=atof(temp);
	fgets(temp,4,file);
    Header.StaticObj=atof(temp);
    fgets(temp,5,file);
    Header.Width=atof(temp);
	fgets(temp,5,file);
	Header.Height=atof(temp);
    fgets(temp,2,file);
    Header.Fog=atof(temp);
    fgets(temp,2,file);
    Header.FloorTexture=atof(temp);
    Header.Pos=0;
    Header.AnimPos=0;
    Header.StaticPos=0;
    
    Anim=new Character*[(int)Header.AnimatedObj];
    Stat=new StaticCharacter*[(int)Header.StaticObj];
    return true;
}

void DeleteModels(){
    for(int i=0; i<Header.AnimPos;i++)
        Anim[i]->~Character();
       // delete t[i]; 
    delete [] Anim;
    delete [] Stat;
}
void MapClose(){
	fclose(file);
	map_loaded=true;
}
void ReadMapPos(){
	char snd_name[100],mdl_name[100],temp[6];

    fseek(file,(Header.Pos*15)+19,SEEK_SET);
	fgets(temp,5,file);
    Objects.X=atof(temp)-(Header.Width/2);
	fgets(temp,5,file);
    Objects.Y=atof(temp)-(Header.Height/2);
	fgets(temp,2,file);
	Objects.R=atof(temp);
    fgets(temp,2,file);
    Objects.Type=atof(temp);
    fgets(temp,3,file);
    Objects.No=atof(temp);
    fgets(temp,4,file);
    Objects.Health=atof(temp);
    if(Objects.Type==0){
    switch((int)Objects.No){
    case 0:	
        strcpy(snd_name,"sound//evil1.wav");	
		strcpy(mdl_name,"tris2");	
	break;
    case 1:
		strcpy(snd_name,"sound//evil2.wav");
		strcpy(mdl_name,"tris3");	
        break;
    case 2:
		strcpy(snd_name,"sound//evil3.wav");
		strcpy(mdl_name,"tris4");	
	break;
    case 3:
		strcpy(snd_name,"sound//evil4.wav");
		strcpy(mdl_name,"tris5");	
	break;
    case 4:
        strcpy(snd_name,"sound//evil5.wav");
		strcpy(mdl_name,"tris6");	
	break;
    default:
        strcpy(snd_name,"sound//evil1.wav");
		strcpy(mdl_name,"tris6");	
	break;
    }
    Anim[Header.AnimPos]=new Character(Objects.X,0.0f,Objects.Y,0.1f,Objects.R,Objects.No,Objects.Health,mdl_name,snd_name);
    }
    else if(Objects.Type==1){
        Stat[Header.StaticPos]=new StaticCharacter(Objects.X,0.0f,Objects.Y,Objects.R,Objects.No,Objects.Health);
    }

}

int InitGL(GLvoid)										
{
	initfMOD();
	str=new FMOD_Stream*[MAX_MP3];
	char mp3_names[60];

	for(int i=0; i<MAX_MP3; i++){
	sprintf(mp3_names,"sound//song%02d.mp3",i);
	str[i]=new FMOD_Stream(mp3_names,true);
	if(i!=0)
	str[i]->stop();
	//str[i]->loopSound();
	}
	str[PlayingMp3No]->setPause(false);
	Header.Pos=0;
    Header.AnimPos =0;
    Header.StaticPos=0;
	smp=new FMOD_Sample*[2];
	smp[0]=new FMOD_Sample("sound//wind.wav",-1.0,0.0,-1.0,0.0,0.0,0.0,false);
	smp[1]=new FMOD_Sample("sound//wind.wav",2.0,0.0,2.0,0.0,0.0,0.0,false);
	hero=new ModelMd2("tris.md2","tris.bmp");
	hero->AnimateMD2Model();
objCamera.actif=false;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);								
	glEnable(GL_COLOR_MATERIAL);	
    LoadTGAFile(&textures[0],"Crosshair.tga");
    LoadTGAFile(&textures[1],"sky.tga");
    LoadTGAFile(&textures[2],"new.tga");
    LoadTGAFile(&textures[3],"save.tga");
    LoadTGAFile(&textures[4],"load.tga");
    LoadTGAFile(&textures[5],"exit.tga");
    LoadTGAFile(&textures[6],"selected.tga");
    LoadTGAFile(&textures[9],"font.tga");
    

	for(int i=0; i<MAP_N;i++) 
		sprintf(MapNames[i],"map//map%02d.map",i);
	
	for(int i=0; i<TEXT_N_3ds;i++){
	g_Load3ds[i].Import3DS(&g_3DModel[i], MldNames3ds[i]);	
	get3ds(i);
	}
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f, 0.0f, 0.0f, 0.99f);				
	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);		
	glEnable(GL_FOG);
    glFogfv(GL_FOG_COLOR, fogColor[3]);
	glFogf(GL_FOG_DENSITY, g_FogDensity);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	BMP_Texture(TextureArray, "model//sun.bmp", 1);
    BMP_Texture(TextureArray, "model//loading.bmp", 4);
	BMP_Texture(TextureArray, "CrossHairMask.bmp", 2);
	BMP_Texture(TextureArray, "CrossHair.bmp", 3);

	objCamera.ang=0.0f;
	objCamera.Position_Camera(0.0f,12.5f,25.0f,	0.0f,7.8f,0.0f,	0.0f, 1.0f, 0.0f);
	BuildFont3D();
    BuildFont2D();
	glCullFace(GL_FRONT);								
	return TRUE;										
}

void FollowCamera(){
	Point p={objCamera.mView.x,objCamera.mView.y,objCamera.mView.z};
	g_LightPosition[0]=objCamera.mView.x;
	g_LightPosition[2]=objCamera.mView.z;
    for(int i=0; i<Header.AnimPos;i++)
	Anim[i]->target=p;
 }

void checkHeroMoveAnim(int i){
Point p;	
p.x=objCamera.mView.x;
p.y=objCamera.mView.y;
p.z=objCamera.mView.z;
if(can_go)
can_go=true;
if(Anim[i]->Distance(p)<=(Anim[i]->r-0.3))
can_go=false;
}
void checkHeroMoveStatic(int i){
Point p;	
p.x=objCamera.mView.x;
p.y=objCamera.mView.y;
p.z=objCamera.mView.z;
if(can_go)
can_go=true;
if(Stat[i]->Distance(p)<=(Stat[i]->r))
can_go=false;
}
int DrawGLScene(GLvoid){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	GLUquadricObj *pObj = gluNewQuadric();		
    GLfloat barLeft=100,barUp=700,barLenght=700,barW;
	POINT mousePos;	
	GetCursorPos(&mousePos);	

	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);	
	glEnable(GL_DEPTH_TEST);							
	glEnable(GL_FOG);
    glFogfv(GL_FOG_COLOR, fogColor[int(Header.Fog)]);
	glFogf(GL_FOG_DENSITY, g_FogDensity);
	gluQuadricTexture(pObj, true);	

	glLoadIdentity();								
	gluLookAt(objCamera.mPos.x,  objCamera.mPos.y,  objCamera.mPos.z,	
			  objCamera.mView.x, objCamera.mView.y, objCamera.mView.z,	
			  objCamera.mUp.x,   objCamera.mUp.y,   objCamera.mUp.z);


Point tmp;
tmp.x=objCamera.mView.x;
tmp.y=objCamera.mView.y;
tmp.z=objCamera.mView.z;
if(!map_loaded && map_no!=-1){
    if((Header.AnimPos+Header.StaticPos)<(int)(Header.StaticObj+Header.AnimatedObj)){
    ReadMapPos();
    Header.Pos++;
    if(Objects.Type==0)
        Header.AnimPos++;
    else if(Objects.Type==1)
        Header.StaticPos++;
    }
    else{
    MapClose();
    }
}
CheckHit();
menuItem=-1;
if(map_loaded && !menu){
FollowCamera();
for(int i=0; i<Header.AnimPos;i++){
    if((int)Anim[i]->health>0){
    checkHeroMoveAnim(i);
    Anim[i]->ToGo();
    Anim[i]->Draw();
    }else{
    if( Anim[i]->p.y>-3.0){
    Anim[i]->p.y-=0.01;
    Anim[i]->Draw();
    }
    Anim[i]->snd->setPause(true);
    }
}
for(int i=0; i<Header.StaticPos;i++){
checkHeroMoveStatic(i);
Stat[i]->Draw();
}
}
	glPushMatrix();
	glTranslatef(objCamera.mView.x,2.0f,objCamera.mView.z);	
	glScalef(0.08f,0.08f,0.08f);
	glRotatef(objCamera.ang-270,0.0f,1.0f,0.0f);
	DrawHero();
	glPopMatrix();
//glPushMatrix();	
//glTranslatef(objCamera.mView.x,5.0f,objCamera.mView.z);	
//glRotatef(objCamera.ang,0.0f,1.0f,0.0f);
//glScalef(0.02f,0.02f,0.02f);
//Draw3ds(10);
//glPopMatrix();
	glPushMatrix();
	glTranslatef(objCamera.mView.x,15.0f,objCamera.mView.z);
	glRotatef(objCamera.ang,0.0f,1.0f,0.0f);
	glRotatef(12,1.0f,0.0f,0.0f);
	glColor3d(1.0f,1.0f,0.0f);
    glPrint3D("%s - %d - %d",FPS(),Header.Pos,(int)(((Header.StaticPos+Header.AnimPos)*100)/(Header.StaticObj+Header.AnimatedObj)));
	glPopMatrix();

	glEnable(GL_BLEND);
	glPushMatrix();
	glBlendFunc(GL_ONE, GL_SRC_ALPHA);		
	glTranslatef(objCamera.mView.x-225,55.0f,objCamera.mView.z-225.0f);
	glBindTexture(GL_TEXTURE_2D, TextureArray[1]);
	gluSphere(pObj, 9.5f, 25, 25);	
	glPopMatrix();
	glDisable(GL_BLEND);

glPushMatrix();
	glTranslatef(objCamera.mView.x,0.0f,objCamera.mView.z);	
	glScalef(3.1f,3.1f,3.1f);
	CreateSkyBox();
	glPopMatrix();
	glPushMatrix();
	DrawGrid();
	glPopMatrix();

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);								
	glDisable(GL_COLOR_MATERIAL);	
	glDisable(GL_DEPTH_TEST);							
	glDisable(GL_FOG);

    if(menu || !map_loaded){
	objCamera.Rotate_Position(-CAMERASPEED/5);
	OrthoMode(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glColor4f(1, 1, 1, 1);

	glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_DST_COLOR,GL_ZERO);
	glEnable(GL_BLEND);							

	RectangleTexture(0,0,SCREEN_HEIGHT,SCREEN_WIDTH,TextureArray[2]);

	glBlendFunc(GL_ONE,GL_ONE);
	
    RectangleTexture(0,0,SCREEN_HEIGHT,SCREEN_WIDTH,TextureArray[3]);
	
	RectangleTexture(262,50,200,500,textures[1].texID);
    menuItem=(mousePos.y-250)/100;
    for(int i=2; i<6; i++)
        if(menuItem==i-2 && mousePos.x>362 && mousePos.x<1024-362 ){
            RectangleTexture(362,(100*i)+50,100,300,textures[6].texID);
            RectangleTexture(362,(100*i)+50,100,300,textures[i].texID);
        }
        else
            RectangleTexture(312,(100*i)+70,80,400,textures[i].texID);
    

    barW=(((Header.StaticPos+Header.AnimPos)*barLenght)/(Header.StaticObj+Header.AnimatedObj))+barLeft;
    if(!map_loaded){
    glPrint2D(100,700,"Loading %c %d",'%',(int)(((Header.StaticPos+Header.AnimPos)*100)/(Header.StaticObj+Header.AnimatedObj)));
    RectangleTexture(barLeft,barUp,20,barW, TextureArray[4]);
    }
    
    RectangleTexture(mousePos.x,mousePos.y,55,55,textures[0].texID);

    //glPrint2D(100,30,"%d - %d = %d",(mousePos.x),(mousePos.y),(mousePos.y-250)/100);
    glPrint2D(670,750,"Warrior ver 1.0 @ Written By Reho");
    if(Header.NumOfObjects>0)
    //glPrint2D(100,300,"%.f",Anim[0]->health);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
	PerspectiveMode();
	}

return TRUE;					
}

void RectangleTexture(GLfloat X,GLfloat Y,GLfloat H,GLfloat W ,UINT TextureID){
glBindTexture(GL_TEXTURE_2D,  TextureID);
	glBegin(GL_QUADS);
     glTexCoord2f(0.0f, 1.0f);  glVertex2f(X, Y);
     glTexCoord2f(0.0f, 0.0f);  glVertex2f(X, Y+H);
     glTexCoord2f(1.0f, 0.0f);  glVertex2f(X+W, Y+H);
     glTexCoord2f(1.0f, 1.0f);  glVertex2f(X+W, Y);
	glEnd();
}

GLvoid KillGLWindow(GLvoid){
	if (fullscreen){
		ChangeDisplaySettings(NULL,0);					
		ShowCursor(TRUE);								
	}

	if (hRC){
		if (!wglMakeCurrent(NULL,NULL)){
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC)){
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										
	}

	if (hDC && !ReleaseDC(hWnd,hDC)){
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										
	}

	if (hWnd && !DestroyWindow(hWnd)){
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										
	}

	if (!UnregisterClass("OpenGL",hInstance)){
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									
	}
	KillFont3D();

}

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			
	WNDCLASS	wc;						
	DWORD		dwExStyle;				
	DWORD		dwStyle;				
	RECT		WindowRect;				
	WindowRect.left=(long)0;			
	WindowRect.right=(long)width;		
	WindowRect.top=(long)0;				
	WindowRect.bottom=(long)height;		

	fullscreen=fullscreenflag;			

	hInstance			= GetModuleHandle(NULL);				
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	wc.lpfnWndProc		= (WNDPROC) WndProc;					
	wc.cbClsExtra		= 0;									
	wc.cbWndExtra		= 0;									
	wc.hInstance		= hInstance;							
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			
	wc.hbrBackground	= NULL;									
	wc.lpszMenuName		= NULL;									
	wc.lpszClassName	= "OpenGL";								

	if (!RegisterClass(&wc))									
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}
	
	if (fullscreen)												
	{
		DEVMODE dmScreenSettings;								
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		
		dmScreenSettings.dmPelsWidth	= width;				
		dmScreenSettings.dmPelsHeight	= height;				
		dmScreenSettings.dmBitsPerPel	= bits;					
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","OPENGL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;	
			}
			else{
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;					
			}
		}
	}

	if (fullscreen)												
	{
		dwExStyle=WS_EX_APPWINDOW;								
		dwStyle=WS_POPUP;										
		ShowCursor(FALSE);										
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			
		dwStyle=WS_OVERLAPPEDWINDOW;							
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		
	if (!(hWnd=CreateWindowEx(	dwExStyle,"OpenGL",title,dwStyle |WS_CLIPSIBLINGS |	WS_CLIPCHILDREN,0, 0,WindowRect.right-WindowRect.left,WindowRect.bottom-WindowRect.top,	NULL,								
								NULL,hInstance,NULL)))								
	{
		KillGLWindow();								
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	static	PIXELFORMATDESCRIPTOR pfd=				
	{
		sizeof(PIXELFORMATDESCRIPTOR),1,PFD_DRAW_TO_WINDOW |PFD_SUPPORT_OPENGL |PFD_DOUBLEBUFFER,							
		PFD_TYPE_RGBA,bits,0, 0, 0, 0, 0, 0,0,	0,0,0, 0, 0, 0,16,0,0,PFD_MAIN_PLANE,0,0, 0, 0										
	};
	
	if (!(hDC=GetDC(hWnd)))							
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(hRC=wglCreateContext(hDC)))				
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!wglMakeCurrent(hDC,hRC))					
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	ShowWindow(hWnd,SW_SHOW);						
	SetForegroundWindow(hWnd);						
	SetFocus(hWnd);									
	ReSizeGLScene(width, height);					

	if (!InitGL())									
	{
		KillGLWindow();								
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}
	return TRUE;									
}
void CheckHit(){
Point p;	

p.x=objCamera.mView.x;
p.y=objCamera.mView.y;
p.z=objCamera.mView.z;
if(hit){

for(int i=0; i<Header.AnimatedObj;i++){
    if(Anim[i]->health>0 && Anim[i]->Distance(p)<=(7.0) && HeroPow>0){
    Anim[i]->health-=34;
    if( Anim[i]->p.y>-3.0)
    Anim[i]->p.y-=0.01;
    HeroPow-=34;
    if(Anim[i]->health<=0)
        Anim[i]->aniPos=15;
    }
}
}

hit=false;

}
LRESULT CALLBACK WndProc(	HWND	hWnd,UINT	uMsg,WPARAM	wParam,LPARAM	lParam)			
{
	switch (uMsg)									
	{
		case WM_ACTIVATE:							
		{
			if (!HIWORD(wParam))					
				active=TRUE;						
			else
				active=FALSE;						
			return 0;								
		}

		case WM_LBUTTONDOWN:
            if(!menu){
		    hero->makeAnimationmd2(10);
            HeroPow=100;
            hit=true;
            }
        else{
            switch(menuItem){
        case 0:
            menu=!menu;
            objCamera.actif=!objCamera.actif;
            break;
            case 1:
                if(Header.NumOfObjects>0)
                MapSave();break;
            case 2:
                if(map_no!=-1 && map_no!=9)
                DeleteModels();
                if(map_no!=9){
                map_no=9;
	            if(!MapOpen(false))
                    exit(0);
	            }
                break;
            case 3:exit(0);
            default:break;
            }
        }

		break;

	case WM_RBUTTONDOWN:	
		if(!menu){
		hero->nextAnimation();
		if(g_ViewMode == GL_TRIANGLES) {				
			g_ViewMode = GL_LINE_STRIP;					
		} else {
			g_ViewMode = GL_TRIANGLES;					
		}
		}
		break;

		case WM_SYSCOMMAND:							
		{
			switch (wParam)							
			{
				case SC_SCREENSAVE:					
				case SC_MONITORPOWER:				
				return 0;							
			}
			break;									
		}

		case WM_CLOSE:								
		{
			PostQuitMessage(0);						
			return 0;								
		}
		case WM_KEYDOWN:

		  keys[wParam]=true;
		switch(wParam) 
		  {		  
		  		case VK_F12:
				PostQuitMessage(0);	
				break;
			}
		  break;
		case WM_KEYUP:								
		{
			hero->makeAnimationmd2(0);
			keys[wParam]=false;
			return 0;								
		}

		case WM_SIZE:								
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  
			return 0;		
		}
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,HINSTANCE	hPrevInstance,LPSTR	lpCmdLine,int nCmdShow)			
{
	MSG		msg;									
	BOOL	done=FALSE;								

	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							
	}
	if (!CreateGLWindow("Warrior (RehoGame)",1024,768,32,fullscreen))
	{
		return 0;									
	}

	while(!done)									
	{

		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	
		{
			if (msg.message==WM_QUIT)				
			{
				done=TRUE;							
			}
			else									
			{
				TranslateMessage(&msg);				
				DispatchMessage(&msg);				
			}
		}
		else										
		{
			Keyboard_Input();
			objCamera.Mouse_Move(1024, 768);
			if ((active && !DrawGLScene()) || keys[VK_F12])	
			{
				done=TRUE;							
			}
			else									
			{
				SwapBuffers(hDC);					
			}

			if (keys[VK_F1])						
			{
				keys[VK_F1]=FALSE;					
				KillGLWindow();						
				fullscreen=!fullscreen;				
				if (!CreateGLWindow("Warrior (RehoGame)",640,480,16,fullscreen))
				{
					return 0;						
				}
			}
		}
	}
	for(int i=0; i<TEXT_N_3ds;i++)
	remove3ds(i);
	KillGLWindow();									
	return (msg.wParam);						
}
void Keyboard_Input()
{
	if((GetKeyState('P') & 0x80) &&  keyPressed['P']==false)
	{	
	keyPressed['P']=true;
	menu=!menu;
	objCamera.actif=!objCamera.actif;
	}

    if((GetKeyState(VK_ESCAPE) & 0x80) &&  keyPressed[VK_ESCAPE]==false)
	{	
	keyPressed[VK_ESCAPE]=true;
	menu=!menu;
	objCamera.actif=!objCamera.actif;
	}
    if(!keys[VK_ESCAPE])
		keyPressed[VK_ESCAPE]=false;
	
    if(!keys['P'])
		keyPressed['P']=false;
	if((GetKeyState(VK_F8) & 0x80)  && keyPressed[VK_F8]==false){
		keyPressed[VK_F8]=true;
	str[PlayingMp3No++]->stop();
	if(PlayingMp3No>MAX_MP3-1)
			PlayingMp3No=0;
	str[PlayingMp3No]->play();
	}
	if(!keys[VK_F8])
		keyPressed[VK_F8]=false;

    if((GetKeyState(VK_F2) & 0x80)  && keyPressed[VK_F2]==false){
		keyPressed[VK_F2]=true;
	if(Header.NumOfObjects>0)
        MapSave();
	}
	if(!keys[VK_F2])
		keyPressed[VK_F2]=false;

   if((GetKeyState(VK_F3) & 0x80)  && keyPressed[VK_F3]==false){
		keyPressed[VK_F3]=true;
	if(map_no!=-1 && map_no!=9)
    DeleteModels();
    if(map_no!=9){
    map_no=9;
	if(!MapOpen(false))
        exit(0);
	}
	}
	if(!keys[VK_F3])
		keyPressed[VK_F3]=false;

	if((GetKeyState(VK_F7) & 0x80)  && keyPressed[VK_F7]==false){
		keyPressed[VK_F7]=true;
	str[PlayingMp3No--]->stop();
	if(PlayingMp3No<0)
			PlayingMp3No=MAX_MP3-1;
	str[PlayingMp3No]->play();
	}
	if(!keys[VK_F7])
		keyPressed[VK_F7]=false;


	if((GetKeyState(VK_F9) & 0x80)  && keyPressed[VK_F9]==false){
		keyPressed[VK_F9]=true;
		str[PlayingMp3No]->setPause(!str[PlayingMp3No]->getPause());
	}
	if(!keys[VK_F9])
		keyPressed[VK_F9]=false;

	if(!menu){
	if(((GetKeyState(VK_UP) & 0x80) || (GetKeyState('W') & 0x80)) && can_go)
	{
	objCamera.Move_Camera( CAMERASPEED);
	listenerPos[0]=objCamera.mView.x;
	listenerPos[2]=objCamera.mView.z;

	FSOUND_3D_Listener_SetAttributes(&listenerPos[0], &listenerVel[0],  listenerOri[0], listenerOri[1], listenerOri[2],listenerOri[3], listenerOri[4], listenerOri[5]);
	FSOUND_Update();
	hero->makeAnimationmd2(1);
	keyPressed['W']=true;
	}
	if(!keys['W'])
	keyPressed['W']=false;

	if((GetKeyState(VK_DOWN) & 0x80) || (GetKeyState('S') & 0x80))
	{	
	objCamera.Move_Camera( -CAMERASPEED);
	listenerPos[0]=objCamera.mView.x;
	listenerPos[2]=objCamera.mView.z;
	can_go=true;
	FSOUND_3D_Listener_SetAttributes(&listenerPos[0], &listenerVel[0],  listenerOri[0], listenerOri[1], listenerOri[2],listenerOri[3], listenerOri[4], listenerOri[5]);
	FSOUND_Update();
	hero->makeAnimationmd2(1);
	keyPressed['S']=true;
	}
	if(!keys['S'])
	keyPressed['S']=false;
	
	if(((GetKeyState(VK_LEFT) & 0x80) || (GetKeyState('A') & 0x80) ) )
	{			
	objCamera.Rotate_Position(-CAMERASPEED);
	keyPressed['A']=true;
	}
	if(!keys['A'])
	keyPressed['A']=false;

	if((GetKeyState(VK_RIGHT) & 0x80) || (GetKeyState('D') & 0x80))
	{				
	objCamera.Rotate_Position( CAMERASPEED);
	keyPressed['D']=true;
	}
	if(!keys['D'])
	keyPressed['D']=false;
	}
	if( (GetKeyState('1') & 0x80) || (GetKeyState('1') & 0x80))
	{	
    keyPressed['1']=true;
    if(map_no!=-1 && map_no!=0)
    DeleteModels();
    if(map_no!=0){
    map_no=0;
	if(!MapOpen(true))
        exit(0);
	}
    }

    if(!keys['1'])
	keyPressed['1']=false;
	if( (GetKeyState('2') & 0x80) )
	{	
   if(map_no!=-1 && map_no!=1)
      DeleteModels();
   if(map_no!=1){
    map_no=1;

    if(!MapOpen(true))
        exit(0);
	}
    }
	if( (GetKeyState('3') & 0x80))
	{	
    if(map_no!=-1 && map_no!=2)
      DeleteModels();
    if(map_no!=2){
    map_no=2;

	if(!MapOpen(true))
        exit(0);
	}
    }
	if( (GetKeyState('4') & 0x80))
	{
    if(map_no!=-1 && map_no!=3)
    DeleteModels();
    if(map_no!=3){	
    map_no=3;
    	if(!MapOpen(true))
        exit(0);
	}
    }

	if( (GetKeyState(' ') & 0x80) && !keyPressed['A'] && !keyPressed['W'] && !keyPressed['S'] && !keyPressed['D'])
	{		
	hero->makeAnimationmd2(6);
	}
	if( (GetKeyState(VK_ADD) & 0x80)){
		g_FogDensity += 0.0005f;					
		glFogf(GL_FOG_DENSITY, g_FogDensity);	
		if(g_FogDensity > 1) g_FogDensity = 1;	
	}
	if( (GetKeyState(VK_CONTROL) & 0x80)&& !keyPressed['A'] && !keyPressed['W'] && !keyPressed['S'] && !keyPressed['D']){
		hero->makeAnimationmd2(12);
	}

	if( (GetKeyState( VK_SUBTRACT) & 0x80)){
		g_FogDensity -= 0.0005f;					
		glFogf(GL_FOG_DENSITY, g_FogDensity);	
	if(g_FogDensity < 0) g_FogDensity = 0;	
	}
}

void CreateSkyBox(){
glRotatef(90,  1.0f, 0,0);						
Draw3ds(3);
}

void DrawGrid()
{																	
GLfloat t_x=DETAIL,start=-900,end=900;
int n=map_no%3;
	for(GLfloat i = start; i <= end; i += (2*t_x))
	for(GLfloat j = start; j <= end; j += (2*t_x))
	{
	glPushMatrix();
	glTranslatef(i,0,j);
	glRotatef(90,1,0,0);
    Draw3ds((n<0?0:n)+10);
	glPopMatrix();
	}
}

char* FPS()
{
	static float fps           = 0.0f;						
    static float previousTime  = 0.0f;	
	static char  strFPS[20]    = {0};
	
	float currentTime = (GetTickCount() * 0.001f);				
    ++fps;	
    if( currentTime - previousTime > 1.0f )
    {
	    previousTime = currentTime;
		sprintf(strFPS, "%d", int(fps));
		SetWindowText(hWnd, strFPS);
		
        fps = 0.0f;
    }
	return strFPS;
}

