/****************************************************************************
 * apps/velacare/velacare_sensor.h — 传感器抽象层头文件
 ****************************************************************************/

#ifndef __APPS_VELACARE_VELACARE_SENSOR_H
#define __APPS_VELACARE_VELACARE_SENSOR_H

#include <stdbool.h>

/* 传感器读取接口 (真机上替换为 HAL) */

bool velacare_sensor_smoke_online(void);
bool velacare_sensor_smoke_alarm(void);
bool velacare_sensor_water_online(void);
bool velacare_sensor_water_alarm(void);
bool velacare_sensor_door_online(void);
bool velacare_sensor_door_alarm(void);
bool velacare_sensor_fall_online(void);
bool velacare_sensor_fall_alarm(void);

#endif
