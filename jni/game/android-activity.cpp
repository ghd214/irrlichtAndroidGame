#include <jni.h>
#include <android/log.h>
#include <sys/types.h>
#include <android/asset_manager_jni.h>
#include <irrlicht.h>
#include "stringaling.h"
#include "text.h"
#include "event.h"
#include <importgl.h>
#include <misc.h> // for setMusic()
#include <android-native-audio.h>
#include <android-sensor-manager.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "android-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "android-activity", __VA_ARGS__))

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

extern int importGLInit();
extern void importGLDeinit();
void nativeDrawIteration();

extern int  gWindowWidth;
extern int  gWindowHeight;
extern int  gAppAlive;
extern bool  OPEN_WEBPAGE_ON_EXIT;
extern stringc gSdCardPath;
extern int counter;
extern int gameState;

extern IrrlichtDevice *device;
extern IVideoDriver* driver;
extern ISceneManager* smgr;
extern ICameraSceneNode* camera;

AAssetManager* assetManager = NULL;
  
static JavaVM* gVM;
jclass jNativeCls;
jmethodID jPlaySoundMethod;

extern gui::IGUIStaticText *tPosition;

/* For JNI: C++ compiler need this */
extern "C" {

// this is called before nativeOnCreate
void Java_com_strom_irrlicht_rabbit_IrrlichtTest_nativeCreateAssetManager( JNIEnv* env, jclass clazz,
        jobject assetManagerTemp)
{
    assetManager = AAssetManager_fromJava(env, assetManagerTemp);
    assert(NULL != assetManager);
}

/** Activity onCreate */
//com/strom/irrlicht/rabbit
void Java_com_strom_irrlicht_rabbit_IrrlichtTest_nativeOnCreate( JNIEnv*  env )
{
    //sensorManagerInit(); 
    //for native sound
    createEngine();
    createBufferQueueAudioPlayer(); 
    createAssetAudioPlayer("sounds/backgroundmusic.mp3");
    setPlayingAssetAudioPlayer(true);
}

/** Activity onPause */
void Java_com_strom_irrlicht_rabbit_IrrlichtTest_nativeOnPause( JNIEnv*  env )
{
    setPlayingAssetAudioPlayer(false);
}

/** Activity onResume */
void Java_com_strom_irrlicht_rabbit_IrrlichtTest_nativeOnResume( JNIEnv*  env )
{
	counter = 0;
    gameState = IN_STARTSCREEN;
}

/** Activity onDestroy */
void Java_com_strom_irrlicht_rabbit_IrrlichtTest_nativeOnDestroy( JNIEnv*  env )
{
    shutdown();
    importGLDeinit();
}

// this method is called by onSurfaceCreated()
void Java_com_strom_irrlicht_rabbit_IrrlichtTest_nativeInitGL( JNIEnv*  env )
{
    // this gVM is used for other.
    env->GetJavaVM(&gVM);
    //com.strom.irrlicht.rabbit.IrrlichtTest
    jNativeCls = (env)->FindClass("com/strom/irrlicht/rabbit/IrrlichtTest");

    if ( jNativeCls == 0 ) {
    //  jni_printf("Unable to find class: %s", CB_CLASS);
        return;
    }

    jPlaySoundMethod = (env)->GetStaticMethodID(jNativeCls
            , "playSound" 
            , "(I)V");

    if ( jPlaySoundMethod == 0 ) {
    //  jni_printf("Unable to find method OnImageUpdate(char[]): %s", CB_CLASS);
        return;
    }

    importGLInit();
    device = createDevice( video::EDT_OGLES1, dimension2d<u32>(gWindowWidth, gWindowHeight), 16, false, false, false, 0);
    driver = device->getVideoDriver();
    
    LOGI("createDevice r=%d w=%d h=%d", device, gWindowWidth, gWindowHeight);
	LOGI("getVideoDriver r=%d", driver);

    if (!device)
       LOGI("No device");
    if (!driver)
       LOGI("No driver");
}

void Java_com_strom_irrlicht_rabbit_IrrlichtTest_nativeResize( JNIEnv*  env, jobject  thiz, jint w, jint h )
{
    LOGI("resize w=%d h=%d", w, h);
    gWindowWidth  = w;
    gWindowHeight = h;
    irr::core::dimension2d<unsigned int> size;
    size.Width = w;
    size.Height = h; 
    device->getVideoDriver()->OnResize(size);
}

void Java_com_strom_irrlicht_rabbit_IrrlichtTest_nativeSendSensorEvent( JNIEnv* env, jobject defaultObj, jfloatArray floatArr)
{
    jfloat buf[2];
    //////////////////////////////////////////////////
    // this is important differences between c and c++.
    // C (*env)->GetFloatArrayRegion(env, floatArr, 0, 2, buf);
    // C++ env->GetFloatArrayRegion(floatArr, 0, 2, buf);
    //
    env->GetFloatArrayRegion(floatArr, 0, 2, buf);
 
    float X = buf[0]; 
    //core::string positionStr = new string(X); 
 
    //tPosition = staticText(stringify(X).c_str(),screenHW(),4,4,"sdcard/Irrlicht/media/bigfont.xml");
    //LOGI("nativeSendSensorEvent");
    if(gameState == IN_GAME) {
        if(smgr != NULL && camera != NULL){
            core::vector3df nowPosition = smgr->getActiveCamera()->getPosition();
           // LOGI("nowPosition x == %d", nowPosition.X);
           // LOGI("nowPosition y == %d", nowPosition.Y);
           // LOGI("nowPosition z == %d", nowPosition.Z);
            nowPosition.X = nowPosition.X + (buf[0]/10);
            nowPosition.Y = nowPosition.Y + (buf[1]/10);
            nowPosition.Z = nowPosition.Z + (buf[2]/10); 
           smgr->getActiveCamera()->setPosition(nowPosition); 
        }
    }
   // LOGI("buf[0] == %d", buf[0]);
   // LOGI("buf[1] == %d", buf[1]);
   // LOGI("buf[2] == %d", buf[2]);
}

void Java_com_strom_irrlicht_rabbit_IrrlichtTest_nativeSendEvent( JNIEnv*  env, jobject defaultObj, jobject event) 
{
    jclass classEvent = env->GetObjectClass(event);
    jfieldID fieldAction = env->GetFieldID(classEvent, "mAction", "I");
    jfieldID fieldX = env->GetFieldID(classEvent, "mX", "F");
    jfieldID fieldY = env->GetFieldID(classEvent, "mY", "F");
    int action = env->GetIntField(event, fieldAction);
    // convert Android coord to OpenGL coords
    float x = env->GetFloatField(event, fieldX);
    float y = env->GetFloatField(event, fieldY);
    LOGI("touch X ===%d", x);
    LOGI("touch Y ===%d", y);
    SEvent irrevent;
    irrevent.MouseInput.ButtonStates = 0xffffffff;
    irrevent.EventType = irr::EET_MOUSE_INPUT_EVENT;
    irrevent.MouseInput.Event = irr::EMIE_COUNT;
	irrevent.MouseInput.X = x;
	irrevent.MouseInput.Y = y;
    if (action==0) {
        irrevent.MouseInput.ButtonStates = 0;
	    irrevent.MouseInput.ButtonStates = irr::EMBSM_LEFT;
        irrevent.MouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN; 
		irrevent.MouseInput.ButtonStates ^= irr::EMBSM_LEFT;   
	    device->postEventFromUser(irrevent);
	    LOGI("left click received %d %f %f", action, x, y);
    } else if (action==1){
        irrevent.MouseInput.ButtonStates = 0;
        irrevent.MouseInput.ButtonStates = irr::EMBSM_LEFT;
        irrevent.MouseInput.Event = irr::EMIE_LMOUSE_LEFT_UP;
		irrevent.MouseInput.ButtonStates ^= irr::EMBSM_LEFT;  
        device->postEventFromUser(irrevent);
        LOGI("left click release %d %f %f", action, x, y);
    } else {
        LOGI("not handled %d", action);   
    }    
}

void Java_com_strom_irrlicht_rabbit_IrrlichtTest_nativeGetStatus(JNIEnv*  env, jobject defaultObj, jobject status) 
{
    if (gAppAlive==7) {
    	gAppAlive   = 1;
        jclass classStatus = env->GetObjectClass(status);
        jfieldID fieldStatus = env->GetFieldID(classStatus, "mQuit", "Z");
        env->SetBooleanField(status, fieldStatus, true);
    }    
}


void Java_com_strom_irrlicht_rabbit_IrrlichtTest_nativeEnvJ2C(JNIEnv*  env, jobject defaultObj, jstring jsdcardPath) 
{
    char ligne[1024+1];
    const char *msg = env->GetStringUTFChars(jsdcardPath,0);
    gSdCardPath = msg;
    LOGI("not handled %s", gSdCardPath.c_str());
    env->ReleaseStringUTFChars(jsdcardPath,msg);
  
}

void Java_com_strom_irrlicht_rabbit_IrrlichtTest_nativeDrawIteration( JNIEnv*  env ) 
{
    //sensorProcess();
    nativeDrawIteration();   
}

}// extern "C"

// c callback java
void setMusic(char* filename){

   JNIEnv *env;

   if(!gVM){
       LOGI("gVM ==NULL %s", filename);   
       return;
   }

   (gVM)->AttachCurrentThread(&env, NULL);
            
   if ( jNativeCls == 0) {
       LOGI("jNativeCls == 0 %s", filename);   
       return;
   }

   if( jPlaySoundMethod == 0) {
       LOGI("jPlaySoundMethod == 0 %s", filename);   
       return;
   }

   (env)->CallStaticVoidMethod(jNativeCls
            , jPlaySoundMethod
            , (jint) 0);

      LOGI("setMusic end%s", filename);   
//   (env)->DeleteLocalRef(env,jSound);  
}


