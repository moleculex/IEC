#include "platform_config.h"
#include "buzzer.h"
#include "sensors.h"
#include "led.h"

#include "FreeRTOS.h"
#include "task.h"

void buzzer_init(void);

void buzzer_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(BUZZER_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);

}

void buzzer_manager(void)
{
	buzzer_init();
	for(;;)
	{
		//_buzzer.buzz = 3;

		/*if(_sensors.solenoid_presence == '0')
			_buzzer.buzz++;
		if(_sensors.door_sensor_presence == '0')
			_buzzer.buzz++;
		if(_sensors.vibration_sensor_presence == '0')
			_buzzer.buzz++;

		for(char count = _buzzer.buzz; count > 0; count--)
		{*/
			//GPIO_SetBits(BUZZER_PORT, BUZZER_PIN);
			/*_led.solenoid = 1;
			_led.door_sensor = 1;
			_led.vibration_sensor = 1;*/
			//vTaskDelay(200);
			/*_led.solenoid = 0;
			_led.door_sensor = 0;
			_led.vibration_sensor = 0;*/
			//GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN);
			//vTaskDelay(200);
		//}
		vTaskDelay(400);
	}
}

void buzzer_on(void)
{
	GPIO_SetBits(BUZZER_PORT, BUZZER_PIN);
}

void buzzer_off(void)
{
	GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN);
}
