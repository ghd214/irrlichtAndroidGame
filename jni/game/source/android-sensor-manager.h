/**
 *@brief
 *@author Huidong Gao
 */

#ifndef __ANDROID_SENSOR_MANAGER_H
#define __ANDROID_SENSOR_MANAGER_H

#include <android/sensor.h>

#ifdef __cplusplus
extern "C" {
#endif

void sensorManagerInit();
void sensorProcess();

#ifdef __cplusplus
}
#endif

#endif// __ANDROID_SENSOR_MANAGER_H
