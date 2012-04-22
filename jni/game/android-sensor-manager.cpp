/**
 *@brief
 *@author Huidong Gao
 */


#include <android/log.h>
#include <android-sensor-manager.h>
#include <android/sensor.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "android-sensor-manager", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "android-sensor-manager", __VA_ARGS__))


//AsensorManager* sensorManager;
//ASensor* accelerometerSensor;
//ASensorEventQueue sensorEventQueue;


void sensorManagerInit(){
    // Prepare to monitor accelerometer
//    sensorManager = ASensorManager_getInstance();
/*    AsensorManager* sensorManager = ASensorManager_getInstance();

    ASensor* accelerometerSensor = ASensorManager_getDefaultSensor(sensorManager,
    ASENSOR_TYPE_ACCELEROMETER);
    ASensorEventQueue sensorEventQueue = ASensorManager_createEventQueue(sensorManager,
    state->looper, LOOPER_ID_USER, NULL, NULL);
*/  
}

void sensorProcess(){
  /*  ASensorEvent event;                                                                         
    if(ASensorEventQueue_hasEvents(sensorEventQueue) == 1){
        LOGI("the queue has events.");        
    }

    if(ASensorEventQueue_hasEvents(sensorEventQueue)){
        LOGI("the queue have one or more events.");
    }
*/
/*
    if(ASensorEventQueue_getEvents(sensorEventQueue,
       &event, 1) > 0) {
       LOGI("accelerometer: x=%f y=%f z=%f",
            event.acceleration.x, event.acceleration.y,
            event.acceleration.z);
    }
*/
}
