#ifndef __SENSORS_H
#define __SENSORS_H

#include "stm32f30x.h"

typedef struct{
	char mains_presence;
	char solenoid_presence;
	char door_sensor_presence;
	char vibration_sensor_presence;
	char door_sensor;
	char vibration_sensor;
	struct{
		char mains_presence;
		char solenoid_presence;
		char door_sensor_presence;
		char vibration_sensor_presence;
		char door_sensor;
		char vibration_sensor;
	}pending;
}t_sensors;

t_sensors _sensors;

void sensors_manager(void);

#endif
