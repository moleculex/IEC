#ifndef __LED_H
#define __LED_H

#include "platform_config.h"
#include "FreeRTOS.h"


typedef struct{
	char modem;
	char mains_presence;
	char sol_status;
	char battery_status;
	char door_status;
	char vib_status;
	GPIO_TypeDef* vib_port;
	GPIO_TypeDef* door_port;
	uint16_t vib_pin;
	uint16_t door_pin;

	char sol2_status;
	char door2_status;
	char vib2_status;
	GPIO_TypeDef* vib2_port;
	GPIO_TypeDef* door2_port;
	uint16_t vib2_pin;
	uint16_t door2_pin;

}t_led;

t_led _led;

void led_manager(void);
void led_on(GPIO_TypeDef* led_port, uint16_t led_pin);
void led_off(GPIO_TypeDef* led_port, uint16_t led_pin);

#endif
