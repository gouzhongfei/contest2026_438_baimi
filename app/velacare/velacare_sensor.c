/****************************************************************************
 * apps/velacare/velacare_sensor.c — 传感器抽象层 (占位实现)
 ****************************************************************************/

#include <nuttx/config.h>
#include "velacare_sensor.h"

bool velacare_sensor_smoke_online(void)  { return true;  }
bool velacare_sensor_smoke_alarm(void)   { return false; }
bool velacare_sensor_water_online(void)  { return true;  }
bool velacare_sensor_water_alarm(void)   { return false; }
bool velacare_sensor_door_online(void)   { return true;  }
bool velacare_sensor_door_alarm(void)    { return false; }
bool velacare_sensor_fall_online(void)   { return true;  }
bool velacare_sensor_fall_alarm(void)    { return false; }
