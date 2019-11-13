#ifndef _3DSOUND_H
#define _3DSOUND_H

class FMOD_Stream{
public:
FMOD_Stream(char* fname,bool status);
void loopSound();
void setVolume(int val);
void setPause(bool flag);
bool getPause();
void stop();
void play();
FSOUND_STREAM *stream;
bool status;
int channel;
};

class FMOD_Sample{
public:
    ~FMOD_Sample();
	FMOD_Sample(char* fname,float pX,float pY,float pZ, float vX,float vY,float vZ,bool status);
	void loopSound();
	void setVolume(int val);
	void setPause(bool flag);
	bool getPause();
	void setMax(float min,float max);

FSOUND_SAMPLE *sample;
int	channel;
bool status;
float sourcePos[3];
float sourceVel[3];
};

FMOD_Stream::FMOD_Stream(char* fname,bool status){
	stream = FSOUND_Stream_OpenFile(fname, FSOUND_NORMAL | FSOUND_MPEGACCURATE, 0);
	if (!stream){
		printf("stream: %s\n", FMOD_ErrorString(FSOUND_GetError()));		
		return;
	}
	channel = FSOUND_Stream_PlayEx(FSOUND_FREE, stream, NULL, TRUE);
	this->status=status;
	setPause(status);
}


void FMOD_Stream::stop(){
FSOUND_Stream_Stop(stream);
}

void FMOD_Stream::play(){
FSOUND_Stream_Play(channel,stream);
}
void FMOD_Stream::setVolume(int val){
FSOUND_SetVolume(channel, val);
}
void FMOD_Stream::setPause(bool flag){
	FSOUND_SetPaused(channel, flag);
}
bool FMOD_Stream::getPause(){
if(FSOUND_GetPaused(channel)==TRUE)
return true;
return false;
}

FMOD_Sample::~FMOD_Sample(){
	FSOUND_Sample_Free(sample);	
}
FMOD_Sample::FMOD_Sample(char* fname,float pX,float pY,float pZ, float vX,float vY,float vZ,bool status){
sourcePos[0]=pX;
sourcePos[1]=pY;
sourcePos[2]=pZ;
sourceVel[0]=vX;
sourceVel[1]=vY;
sourceVel[2]=vZ;
setMax(1000.0,7.0);
	sample = FSOUND_Sample_Load(FSOUND_UNMANAGED, fname, FSOUND_HW3D | FSOUND_STREAMABLE, 0);
	if (!sample){
		printf("sample: %s\n", FMOD_ErrorString(FSOUND_GetError()));		
		return;
	}
	this->status=status;
	loopSound();
}
void FMOD_Sample::setVolume(int val){
FSOUND_SetVolume(channel, val);
}
void FMOD_Sample::setPause(bool flag){
	FSOUND_SetPaused(channel, flag);
}
bool FMOD_Sample::getPause(){
if(FSOUND_GetPaused(channel)==TRUE)
return true;
return false;
}

void FMOD_Sample::setMax(float min,float max){
//FSOUND_3D_SetDistanceFactor(max);
 FSOUND_Sample_SetMinMaxDistance(sample,min,max);
}
void FMOD_Sample::loopSound() {	
	channel = FSOUND_PlaySoundEx(FSOUND_FREE, sample, NULL, TRUE);
	FSOUND_3D_SetAttributes(channel, sourcePos, sourceVel);	
	FSOUND_SetLoopMode(channel, FSOUND_LOOP_NORMAL);
	setPause(status);
}


void initfMOD() {	
   FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
   if (!FSOUND_Init(44100, 32, 0)) {
		printf("Init: %s\n", FMOD_ErrorString(FSOUND_GetError()));
		exit(1);
   }	
}
#endif