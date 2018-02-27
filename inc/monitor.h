#ifndef __MONITOR_H
#define __MONITOR_H

#include "platform_config.h"
#include "stm32f30x_gpio.h"

#define DEVICE_TYPE_SOLENOID 0x01
#define DEVICE_TYPE_VIB_SENSOR 0x02
#define DEVICE_TYPE_DOOR_SENSOR 0x03

typedef struct{

	uint16_t input_val[6];
	uint16_t int_input_val[6];

	GPIO_TypeDef* vib_port;
	GPIO_TypeDef* door_port;
	uint16_t vib_pin;
	uint16_t door_pin;
	GPIO_TypeDef* vib2_port;
	GPIO_TypeDef* door2_port;
	uint16_t vib2_pin;
	uint16_t door2_pin;

	char mains_presence_previous;
	char door_sensor_previous;
	char door2_sensor_previous;
	char vibration_sensor_count;
	char vibration2_sensor_count;

	union
	{
		char status[2];
		struct
		{
			unsigned mains_presence:1;
			unsigned solenoid_presence:1;
			unsigned door_sensor_presence:1;
			unsigned vibration_sensor_presence:1;

			unsigned solenoid2_presence:1;
			unsigned door2_sensor_presence:1;
			unsigned vibration2_sensor_presence:1;
			unsigned door_sensor:1;

			unsigned vibration_sensor:1;
			unsigned door2_sensor:1;
			unsigned vibration2_sensor:1;
			unsigned battery:5;
		}__attribute__((packed));
	}status_bits;

	struct{
		char mains_presence;
		char solenoid_presence;
		char door_sensor_presence;
		char vibration_sensor_presence;
		char door_sensor;
		char vibration_sensor;
		char solenoid2_presence;
		char vibration2_sensor_presence;
		char door2_sensor_presence;
		char door2_sensor;
		char vibration2_sensor;
	}pending;
}t_monitor;

typedef struct{
	char type;
	char previous_type;
	uint16_t id;
	GPIO_TypeDef* port;
	uint16_t pin;
}t_device;

t_monitor _monitor;

t_device _device[6];


void monitor_manager(void);

#endif
