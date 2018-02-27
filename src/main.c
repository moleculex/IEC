#include "platform_config.h"
#include "sensors.h"
#include "monitor.h"
#include "battery.h"
#include "modem.h"
#include "solenoid.h"
#include "led.h"
#include "buzzer.h"
#include "vcp.h"
#ifdef ZIM
#include "_3phase.h"
#endif
#include "stm32f30x_iwdg.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define SENSORS_MANAGER_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 50 )
#define SENSORS_MANAGER_TASK_PRIORITY			( tskIDLE_PRIORITY + 2 )
#define MONITOR_MANAGER_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 50 )
#define MONITOR_MANAGER_TASK_PRIORITY			( tskIDLE_PRIORITY + 2 )
#define SOLENOID_MANAGER_TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE + 50 )
#define SOLENOID_MANAGER_TASK_PRIORITY			( tskIDLE_PRIORITY + 2 )
#define MODEM_MANAGER_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 50 )
#define MODEM_MANAGER_TASK_PRIORITY				( tskIDLE_PRIORITY + 2 )
#define LED_MANAGER_TASK_STACK_SIZE				( configMINIMAL_STACK_SIZE + 50 )
#define LED_MANAGER_TASK_PRIORITY				( tskIDLE_PRIORITY + 2 )
#define BUZZER_MANAGER_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 50 )
#define BUZZER_MANAGER_TASK_PRIORITY			( tskIDLE_PRIORITY + 2 )
#define VCP_MANAGER_TASK_STACK_SIZE				( configMINIMAL_STACK_SIZE + 100 )
#define VCP_MANAGER_TASK_PRIORITY				( tskIDLE_PRIORITY + 2 )
#define BATTERY_MANAGER_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 100 )
#define BATTERY_MANAGER_TASK_PRIORITY			( tskIDLE_PRIORITY + 2 )

int main(void)
{
	SysTick_Config(72000);

	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	IWDG_SetReload(72000/32); //72000/32 = 1s
	IWDG_ReloadCounter();
	IWDG_Enable();

	xSemaphore = xSemaphoreCreateMutex();

	xTaskCreate( (pdTASK_CODE)monitor_manager, (const signed char * const)"monitor_manager", MONITOR_MANAGER_TASK_STACK_SIZE, NULL, MONITOR_MANAGER_TASK_PRIORITY, NULL );
	xTaskCreate( (pdTASK_CODE)solenoid_manager, (const signed char * const)"solenoid_manager", SOLENOID_MANAGER_TASK_STACK_SIZE, NULL, SOLENOID_MANAGER_TASK_PRIORITY, NULL );
	xTaskCreate( (pdTASK_CODE)modem_manager, (const signed char * const)"modem_manager", MODEM_MANAGER_TASK_STACK_SIZE, NULL, MODEM_MANAGER_TASK_PRIORITY, NULL );
	xTaskCreate( (pdTASK_CODE)led_manager, (const signed char * const)"led_manager", LED_MANAGER_TASK_STACK_SIZE, NULL, LED_MANAGER_TASK_PRIORITY, NULL );
	xTaskCreate( (pdTASK_CODE)buzzer_manager, (const signed char * const)"buzzer_manager", BUZZER_MANAGER_TASK_STACK_SIZE, NULL, BUZZER_MANAGER_TASK_PRIORITY, NULL );
	xTaskCreate( (pdTASK_CODE)vcp_manager, (const signed char * const)"vcp_manager", VCP_MANAGER_TASK_STACK_SIZE, NULL, VCP_MANAGER_TASK_PRIORITY, NULL );
	xTaskCreate( (pdTASK_CODE)battery_manager, (const signed char * const)"battery_manager", BATTERY_MANAGER_TASK_STACK_SIZE, NULL, BATTERY_MANAGER_TASK_PRIORITY, NULL );
	xTaskCreate( (pdTASK_CODE)skeleton_key, (const signed char * const)"skeleton_key", 128, NULL, 2, NULL );
#ifdef ZIM
	xTaskCreate( (pdTASK_CODE)_3phase_manager, (const signed char * const)"_3phase_manager", 128, NULL, 2, NULL );
#endif

	vTaskStartScheduler();

	while(1);
}


