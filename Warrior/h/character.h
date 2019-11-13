#ifndef _CHARACTER_H
#define _CHARACTER_H
#include "h\3dsound.h"

struct Point{
	GLfloat x,y,z;
};


class Character{
public :
Character(GLfloat x,GLfloat y,GLfloat z,GLfloat speed,GLfloat r,GLfloat ObjNo,GLfloat health,char mdlName[],char sndName[]);
~Character();	
	void Draw();
	bool ToGo();
	bool Coolision(Character);
	bool Coolision(Point);
	GLfloat Calculate_m(Point );
	GLfloat Distance(Point );
	GLfloat Calculate_angle(Point);
	Point p,target;
	GLfloat m,angle,dx,dz,speed,top,r,ObjNo,health;
	int aniPos;
	ModelMd2 *mdl;
	FMOD_Sample* snd;
};

Character::~Character(){
    delete mdl;
    delete snd;
}
Character::Character(GLfloat x,GLfloat y,GLfloat z,GLfloat speed,GLfloat r,GLfloat ObjNo,GLfloat health,char mdlName[],char sndName[]){
char temp[50];	
    p.x=x;
	p.y=y;
	p.z=z;
	angle=180.0f;
	m=0.0f;
	this->speed=speed;
	this->r=r;
	this->ObjNo=ObjNo;
	aniPos=0;
    this->health=health;
	strcpy(temp,mdlName);
	mdl=new ModelMd2(strcat(mdlName,".md2"),strcat(temp,".bmp"));
	snd=new FMOD_Sample(sndName,p.x,p.y,p.z,0.0,0.0,0.0,false);
   
}
bool Character::Coolision(Character ch){
  	return  sqrt( pow((this->p.x)-(ch.p.x),2)  +pow((this->p.z)-(ch.p.z),2))<=(r+ch.r);
}
bool Character::Coolision(Point p){
	return  Distance(p)<=(r/2);
}

GLfloat Character::Distance(Point p){
return sqrt( pow((this->p.x)-(p.x),2)  +pow((this->p.z)-(p.z),2));
}

bool Character::ToGo(){
Calculate_angle(target);
aniPos=8;
if(!Coolision(target)){
this->p.x+=(dx/top)*speed;
this->p.z+=(dz/top)*speed;
aniPos=1;
return true;
}
return false;
}

GLfloat Character::Calculate_m(Point p){
dz=(p.z-this->p.z);
dx=(p.x-this->p.x);
top=abs(dx)+abs(dz);
m=dx/dz;
return m;
}

GLfloat Character::Calculate_angle(Point p){
angle=(GLfloat)(180/PI)*atan(Calculate_m(p));
	if(p.x<this->p.x && p.z > this->p.z)
		angle+=180;
	else if(p.x < this->p.x && p.z < this->p.z)
		angle-=0;
	else if(p.x>this->p.x && p.z < this->p.z)
		angle+=360;
	else 
		angle+=180;
return angle;
}


void Character::Draw(){	
	glPushMatrix();
	glTranslatef(p.x,2.0f+p.y,p.z);
	glRotatef(angle+90,0,1,0);
	glScalef(0.08f,0.08f,0.08f);
	mdl->makeAnimationmd2(aniPos);
	mdl->AnimateMD2Model();
	glPopMatrix();
    
	snd->sourcePos[0]=p.x;
	snd->sourcePos[2]=p.z;
	FSOUND_3D_SetAttributes(snd->channel,snd->sourcePos,snd->sourceVel);
    FSOUND_Update();

}

class StaticCharacter{
public :
StaticCharacter(GLfloat x,GLfloat y,GLfloat z,GLfloat r,GLfloat ObjNo,GLfloat health);
void Draw();
bool Coolision(StaticCharacter);
bool Coolision(Point);
GLfloat Distance(Point p);
Point p;
GLfloat r,ObjNo,health;
int aniPos;
};

StaticCharacter::StaticCharacter(GLfloat x,GLfloat y,GLfloat z,GLfloat r,GLfloat ObjNo,GLfloat health){
char temp[50];	

p.x=x;
	p.y=y;
	p.z=z;
	this->r=r;
	this->ObjNo=ObjNo;
    this->health=health;
}
bool StaticCharacter::Coolision(StaticCharacter ch){
  	return  sqrt( pow((this->p.x)-(ch.p.x),2)  +pow((this->p.z)-(ch.p.z),2))<=(r+ch.r);
}
bool StaticCharacter::Coolision(Point p){
	return  Distance(p)<=(1.0);
}

GLfloat StaticCharacter::Distance(Point p){
return sqrt( pow((this->p.x)-(p.x),2)  +pow((this->p.z)-(p.z),2));
}

void StaticCharacter::Draw()
{
	glPushMatrix();
	glTranslatef(p.x,0.0f,p.z);

	glRotatef(90,1,0,0);
	glScalef(0.08f*r,0.08f*r,0.08f*r);
    Draw3ds(ObjNo);
    glPopMatrix();
   
}

#endif