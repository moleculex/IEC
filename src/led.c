#include "platform_config.h"
#include "led.h"
#include "cfg.h"
#include "stm32f30x_iwdg.h"

#include "FreeRTOS.h"
#include "task.h"

void led_init(void);

void led_init(void)
{
	_led.modem = 0;
	_led.mains_presence = 0;
	_led.battery_status = 0;

	_led.sol_status = 0;
	_led.door_status = 0;
	_led.vib_status = 0;

	_led.vib_port = GPIOA;
	_led.door_port = GPIOA;

	_led.vib_pin = GPIO_Pin_9;
	_led.door_pin = GPIO_Pin_10;

#ifdef TWO_DOOR
	_led.sol2_status = 0;
	_led.door2_status = 0;
	_led.vib2_status = 0;

	_led.vib2_port = GPIOC;
	_led.door2_port = GPIOC;

	_led.vib2_pin = GPIO_Pin_8;
	_led.door2_pin = GPIO_Pin_9;
#endif

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(LED_MODEM_CONNECTED_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED_MODEM_CONNECTED_PIN | BUZZER_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_MODEM_CONNECTED_PORT, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(LED_MAINS_PRESENCE_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED_MAINS_PRESENCE_PIN | LED_SOLENOID_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_MAINS_PRESENCE_PORT, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(LED_DOOR_SENSOR_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED_DOOR_SENSOR_PIN | LED_VIBRATION_SENSOR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_DOOR_SENSOR_PORT, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(LED_BATTERY_STATUS_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED_BATTERY_STATUS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_BATTERY_STATUS_PORT, &GPIO_InitStructure);

#ifdef TWO_DOOR
	RCC_AHBPeriphClockCmd(LED_SOLENOID2_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED_SOLENOID2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_SOLENOID2_PORT, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(LED_VIBRATION2_SENSOR_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED_VIBRATION2_SENSOR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_VIBRATION2_SENSOR_PORT, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(LED_DOOR2_SENSOR_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED_DOOR2_SENSOR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_DOOR2_SENSOR_PORT, &GPIO_InitStructure);
#endif
}

void led_manager(void)
{
	char toggle = 0;
	char toggle2 = 0;
	char toggle2_hold = 0;
	reboot_timer = 0;
	_cfg.reboot_interval = 10080;
	led_init();

#ifdef TWO_DOOR
		led_on(LED_SOLENOID_PORT, LED_SOLENOID_PIN);
		led_on(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
		led_on(LED_DOOR_SENSOR_PORT, LED_DOOR_SENSOR_PIN);
		led_on(LED_VIBRATION_SENSOR_PORT, LED_VIBRATION_SENSOR_PIN);
		led_on(LED_BATTERY_STATUS_PORT, LED_BATTERY_STATUS_PIN);
		led_on(LED_MAINS_PRESENCE_PORT, LED_MAINS_PRESENCE_PIN);
		led_on(LED_VIBRATION2_SENSOR_PORT, LED_VIBRATION2_SENSOR_PIN);
		led_on(LED_DOOR2_SENSOR_PORT, LED_DOOR2_SENSOR_PIN);
		led_on(LED_SOLENOID2_PORT, LED_SOLENOID2_PIN);
		vTaskDelay(200);
		led_off(LED_SOLENOID_PORT, LED_SOLENOID_PIN);
		led_off(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
		led_off(LED_DOOR_SENSOR_PORT, LED_DOOR_SENSOR_PIN);
		led_off(LED_VIBRATION_SENSOR_PORT, LED_VIBRATION_SENSOR_PIN);
		led_off(LED_BATTERY_STATUS_PORT, LED_BATTERY_STATUS_PIN);
		led_off(LED_MAINS_PRESENCE_PORT, LED_MAINS_PRESENCE_PIN);
		led_off(LED_VIBRATION2_SENSOR_PORT, LED_VIBRATION2_SENSOR_PIN);
		led_off(LED_DOOR2_SENSOR_PORT, LED_DOOR2_SENSOR_PIN);
		led_off(LED_SOLENOID2_PORT, LED_SOLENOID2_PIN);
#endif

	for(;;)
	{
		if(xSemaphoreTake(xSemaphore, 1000))
		{
			if(_led.modem == 0)
				GPIO_SetBits(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
			else
				GPIO_ResetBits(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);

			if(_led.mains_presence == 0)
				GPIO_SetBits(LED_MAINS_PRESENCE_PORT, LED_MAINS_PRESENCE_PIN);
			else
				GPIO_ResetBits(LED_MAINS_PRESENCE_PORT, LED_MAINS_PRESENCE_PIN);

			if(_led.battery_status == 0)
				GPIO_SetBits(LED_BATTERY_STATUS_PORT, LED_BATTERY_STATUS_PIN);
			else
			{
				if(_led.battery_status == 1)
					GPIO_ResetBits(LED_BATTERY_STATUS_PORT, LED_BATTERY_STATUS_PIN);
				else
				{
					if(_led.battery_status == 2)
					{
						if(toggle == 0)
						{
							GPIO_SetBits(LED_BATTERY_STATUS_PORT, LED_BATTERY_STATUS_PIN);
						}
						else
						{
							GPIO_ResetBits(LED_BATTERY_STATUS_PORT, LED_BATTERY_STATUS_PIN);
						}
					}
					else
					{
						if(toggle2 == 0)
						{
							GPIO_SetBits(LED_BATTERY_STATUS_PORT, LED_BATTERY_STATUS_PIN);
						}
						else
						{
							GPIO_ResetBits(LED_BATTERY_STATUS_PORT, LED_BATTERY_STATUS_PIN);
						}
					}
				}
			}

			if(_led.sol_status == 0)
				GPIO_SetBits(LED_SOLENOID_PORT, LED_SOLENOID_PIN);
			else
			{
				if(_led.sol_status == 1)
					GPIO_ResetBits(LED_SOLENOID_PORT, LED_SOLENOID_PIN);
				else
				{
					if(toggle == 0)
					{
						GPIO_SetBits(LED_SOLENOID_PORT, LED_SOLENOID_PIN);
					}
					else
					{
						GPIO_ResetBits(LED_SOLENOID_PORT, LED_SOLENOID_PIN);
					}
				}
			}

			if(_led.door_status == 0)
				GPIO_SetBits(_led.door_port, _led.door_pin);
			else
			{
				if(_led.door_status == 1)
					GPIO_ResetBits(_led.door_port, _led.door_pin);
				else
				{
					if(toggle == 0)
					{
						GPIO_SetBits(_led.door_port, _led.door_pin);
					}
					else
					{
						GPIO_ResetBits(_led.door_port, _led.door_pin);
					}
				}
			}

			if(_led.vib_status == 0)
				GPIO_SetBits(_led.vib_port, _led.vib_pin);
			else
			{
				if(_led.vib_status == 1)
					GPIO_ResetBits(_led.vib_port, _led.vib_pin);
				else
				{
					if(toggle == 0)
					{
						GPIO_SetBits(_led.vib_port, _led.vib_pin);
					}
					else
					{
						GPIO_ResetBits(_led.vib_port, _led.vib_pin);
					}
				}
			}

#ifdef TWO_DOOR
			if(_led.sol2_status == 0)
				GPIO_SetBits(LED_SOLENOID2_PORT, LED_SOLENOID2_PIN);
			else
			{
				if(_led.sol2_status == 1)
					GPIO_ResetBits(LED_SOLENOID2_PORT, LED_SOLENOID2_PIN);
				else
				{
					if(toggle == 0)
					{
						GPIO_SetBits(LED_SOLENOID2_PORT, LED_SOLENOID2_PIN);
					}
					else
					{
						GPIO_ResetBits(LED_SOLENOID2_PORT, LED_SOLENOID2_PIN);
					}
				}
			}

			if(_led.door2_status == 0)
				GPIO_SetBits(_led.door2_port, _led.door2_pin);
			else
			{
				if(_led.door2_status == 1)
					GPIO_ResetBits(_led.door2_port, _led.door2_pin);
				else
				{
					if(toggle == 0)
					{
						GPIO_SetBits(_led.door2_port, _led.door2_pin);
					}
					else
					{
						GPIO_ResetBits(_led.door2_port, _led.door2_pin);
					}
				}
			}

			if(_led.vib2_status == 0)
				GPIO_SetBits(_led.vib2_port, _led.vib2_pin);
			else
			{
				if(_led.vib2_status == 1)
					GPIO_ResetBits(_led.vib2_port, _led.vib2_pin);
				else
				{
					if(toggle == 0)
					{
						GPIO_SetBits(_led.vib2_port, _led.vib2_pin);
					}
					else
					{
						GPIO_ResetBits(_led.vib2_port, _led.vib2_pin);
					}
				}
			}
#endif


			if(toggle == 0)
				toggle = 1;
			else
				toggle = 0;

			if(toggle2 == 0 && toggle2_hold == 5)
			{
				toggle2 = 1;
				toggle2_hold = 0;
			}
			else
			{
				toggle2 = 0;
				toggle2_hold++;
			}

			/*if(_led.door_sensor == 0)
				GPIO_SetBits(LED_DOOR_SENSOR_PORT, LED_DOOR_SENSOR_PIN);
			else
				GPIO_ResetBits(LED_DOOR_SENSOR_PORT, LED_DOOR_SENSOR_PIN);

			if(_led.vibration_sensor == 0)
				GPIO_SetBits(LED_VIBRATION_SENSOR_PORT, LED_VIBRATION_SENSOR_PIN);
			else
				GPIO_ResetBits(LED_VIBRATION_SENSOR_PORT, LED_VIBRATION_SENSOR_PIN);*/

			xSemaphoreGive(xSemaphore);
		}

		if(_cfg.reboot_interval <= 40320)
		{
			if(reboot_timer++ >= 5*60*_cfg.reboot_interval)
				while(1);
		}
		IWDG_ReloadCounter();

		vTaskDelay(200);
	}
}

void led_on(GPIO_TypeDef* led_port, uint16_t led_pin)
{
	GPIO_ResetBits(led_port, led_pin);
}

void led_off(GPIO_TypeDef* led_port, uint16_t led_pin)
{
	GPIO_SetBits(led_port, led_pin);
}


