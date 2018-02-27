#include "platform_config.h"
#include "solenoid.h"
#include "led.h"
#include "buzzer.h"
#include "cfg.h"
#include "flash.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void solenoid_init(void);

void solenoid_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//HV Solenoid
	RCC_AHBPeriphClockCmd(SOLENOID_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SOLENOID_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(SOLENOID_PORT, &GPIO_InitStructure);

#ifdef TWO_DOOR
	RCC_AHBPeriphClockCmd(SOLENOID2_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SOLENOID2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(SOLENOID2_PORT, &GPIO_InitStructure);
#endif

#ifdef ZIM
	RCC_AHBPeriphClockCmd(RS485_TX_RCC_AHB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = RS485_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(RS485_TX_PORT, &GPIO_InitStructure);
#endif
}

void solenoid_manager(void)
{
	solenoid_init();
	for(;;)
	{
		if(_solenoid.solenoid == 1)
		{
			flash_read(CFG_ROOT, _cfg.buffer, CFG_BUFFER_SIZE);
			if(_cfg.empty != 0x3C)
				cfg_default();
			else
				_cfg.solenoid_timeout = cfg_byteSwap(_cfg.solenoid_timeout);

			GPIO_SetBits(SOLENOID_PORT, SOLENOID_PIN);
			_led.sol_status = 1;
			buzzer_on();
			vTaskDelay(_cfg.solenoid_timeout);
			GPIO_ResetBits(SOLENOID_PORT, SOLENOID_PIN);
			_solenoid.solenoid = 0;
			_led.sol_status = 0;
			buzzer_off();
		}
#ifdef TWO_DOOR
		if(_solenoid.solenoid2 == 1)
		{
			flash_read(CFG_ROOT, _cfg.buffer, CFG_BUFFER_SIZE);
			if(_cfg.empty != 0x3C)
				cfg_default();
			else
				_cfg.solenoid_timeout = cfg_byteSwap(_cfg.solenoid_timeout);

			GPIO_SetBits(SOLENOID2_PORT, SOLENOID2_PIN);
			_led.sol2_status = 1;
			buzzer_on();
			vTaskDelay(_cfg.solenoid_timeout);
			GPIO_ResetBits(SOLENOID2_PORT, SOLENOID2_PIN);
			_solenoid.solenoid2 = 0;
			_led.sol2_status = 0;
			buzzer_off();
		}
#endif

#ifdef ZIM
		if(GPIO_ReadInputDataBit(RS485_TX_PORT, RS485_TX_PIN) == 0)
			_solenoid.solenoid = 1;
#endif
		vTaskDelay(100);
	}
}
