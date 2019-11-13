#include "h\main.h"

CCamera::CCamera(){
actif=true;
mouse_moved=true;
}


void CCamera::Position_Camera(float pos_x,  float pos_y,  float pos_z,
							  float view_x, float view_y, float view_z,
							  float up_x,   float up_y,   float up_z)
{
	mPos	= tVector3(pos_x,  pos_y,  pos_z ); 
	mView	= tVector3(view_x, view_y, view_z); 
	mUp		= tVector3(up_x,   up_y,   up_z  ); 
}
void CCamera::Move_Camera(float speed)
{
	tVector3 vVector = mView - mPos;	
	
	mPos.x  = mPos.x  + vVector.x * speed;
	mPos.z  = mPos.z  + vVector.z * speed;
	mView.x = mView.x + vVector.x * speed;
	mView.z = mView.z + vVector.z * speed;
}
void CCamera::Rotate_View(float speed)
{
	tVector3 vVector = mView - mPos;	
	mView.z = (float)(mPos.z + sin(speed)*vVector.x + cos(speed)*vVector.z);
	mView.x = (float)(mPos.x + cos(speed)*vVector.x - sin(speed)*vVector.z);
}
void CCamera::Rotate_Position(float speed)
{	
	tVector3 vVector = mPos - mView;
	mPos.z = (float)(mView.z + sin(speed)*vVector.x + cos(speed)*vVector.z);
	mPos.x = (float)(mView.x + cos(speed)*vVector.x - sin(speed)*vVector.z);
	ang=(GLfloat)(180/PI)*atan((mView.x-mPos.x)/(mView.z-mPos.z));
	if(mView.x<mPos.x && mView.z > mPos.z)
		ang+=180;
	else if(mView.x<mPos.x && mView.z < mPos.z)
		ang-=0;
	else if(mView.x>mPos.x && mView.z < mPos.z)
		ang+=360;
	else
		ang+=180;
	mouse_moved=true;
}
void CCamera::Mouse_Move( int wndWidth, int wndHeight )
{
	if(actif){
	POINT mousePos;	
	int mid_x = wndWidth  >> 1;	
	int mid_y = wndHeight >> 1;	
	float angle_y  = 0.0f;				
	float angle_z  = 0.0f;						
	
	GetCursorPos(&mousePos);		
	if( (mousePos.x == mid_x) && (mousePos.y == mid_y) ) return;

	SetCursorPos(mid_x, mid_y);					

	angle_y = (float)( (mid_x - mousePos.x) ) / 1000;		
	angle_z = (float)( (mid_y - mousePos.y) ) / 1000;

	mView.y += angle_z * 2;

	if(mView.y > 7.8f)		mView.y =7.8f;
	if(mView.y < 0.4f)		mView.y = 0.4f;
mouse_moved=false;
	Rotate_Position(-angle_y);
	}
}
