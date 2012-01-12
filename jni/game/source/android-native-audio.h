#ifndef __ANDROID_NATIVE_AUDIO_H
#define __ANDROID_NATIVE_AUDIO_H

#include <irrlicht.h>
// for native audio
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
using namespace irr;

#ifdef __cplusplus
extern "C" {
#endif

void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context);
void bqRecorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context);
void createEngine(); 
void createBufferQueueAudioPlayer();
bool createUriAudioPlayer(char* uri);
void setPlayingUriAudioPlayer(bool isPlaying);
bool enableReverb(bool enabled);
bool selectClip(int which, int count);
bool createAssetAudioPlayer(char* filename);
void setPlayingAssetAudioPlayer(bool isPlaying);
bool createAudioRecorder();
void startRecording();
void shutdown();

#ifdef __cplusplus 
} // extern "C"
#endif 

#endif //__ANDROID_NATIVE_AUDIO_H

