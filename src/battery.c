#include "platform_config.h"
#include "battery.h"
#include "led.h"
#include "stm32f30x_adc.h"
#include "stm32f30x.h"
#include  "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "monitor.h"

#include "modem.h"

void battery_init(void);
uint16_t battery_sample(void);
void battery_populate(uint16_t level);
//void battery_monitor(void);
//void battery_status(void);
void battery_charge_on(void);
void battery_charge_off(void);

void battery_init(void)
{
//digital
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(BATTERY_CHARGE_EN_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = BATTERY_CHARGE_EN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(BATTERY_CHARGE_EN_PORT, &GPIO_InitStructure);
	
//analog
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	uint32_t calibration_value = 0;

	RCC_ADCCLKConfig(RCC_ADC12PLLCLK_OFF);
	RCC_AHBPeriphClockCmd(BATTERY_LEVEL_RCC, ENABLE);


	GPIO_InitStructure.GPIO_Pin = BATTERY_LEVEL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(BATTERY_LEVEL_PORT, &GPIO_InitStructure);


	ADC_StructInit(&ADC_InitStructure);

	ADC_VoltageRegulatorCmd(ADC1, ENABLE);

	vTaskDelay(5);

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Clock = ADC_Clock_SynClkModeDiv1;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_OneShot;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0;

	ADC_CommonInit(ADC1, &ADC_CommonInitStructure);

	ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
	ADC_StartCalibration(ADC1);

	while(ADC_GetCalibrationStatus(ADC1) != RESET );

	calibration_value = ADC_GetCalibrationValue(ADC1);


	ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;
	ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;
	ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;
	ADC_InitStructure.ADC_NbrOfRegChannel = 1;
	//ADC_InitStructure.ADC_NbrOfRegChannel = 3;
	ADC_Init(ADC1, &ADC_InitStructure);

	//ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_601Cycles5);//comment out for modular sample
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 2, ADC_SampleTime_7Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_7Cycles5);

	//ADC_Cmd(ADC1, ENABLE); //comment out for modular sample
	//ADC_DMACmd(ADC1, ENABLE);


	//while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY)); //comment out for modular sample

	//ADC_StartConversion(ADC1); //comment out for modular sample
}

void battery_manager(void)
{
	_battery.strLevel[0] = '0';
	_battery.strLevel[1] = '0';
	_battery.strLevel[2] = '0';
	_battery.strLevel[3] = '0';
	_battery.chargeTime = 0;
	_battery.chargeTimeEn = 0;
	_battery.charge = 0;

	battery_init();
	for(;;)
	{
		battery_charge_on();
		vTaskDelay(2000);
		_battery.intTmpLevel = battery_sample();
		battery_charge_off();
		vTaskDelay(100);
		_battery.intLevel = battery_sample();
		_battery.difference = _battery.intTmpLevel - _battery.intLevel;

		if(_monitor.status_bits.mains_presence == 1)
		{
			if(_battery.difference < 80)
			{
				vTaskDelay(1000);
				_battery.intLevel = battery_sample();

				if(_battery.intLevel <= 1250)
					_battery.charge = 1;

				if(_battery.intLevel >= 1300)
				{
					if(_battery.charge == 1)
					{
						_battery.chargeTime = 0;
						_battery.chargeTimeEn = 1;
						_battery.charge = 2;
					}
				}

				if(_battery.charge == 1)
				{
					battery_charge_on();
					_monitor.status_bits.battery = 2;
					_led.battery_status = 2;
				}
				else
				{
					if(_battery.charge == 2)
					{
						battery_charge_on();
						_monitor.status_bits.battery = 1;
						_led.battery_status = 1;
						if(_battery.chargeTime > 720)
						{
							_battery.charge = 0;
							_battery.chargeTimeEn = 0;
						}
					}
					else
					{
						battery_charge_off();
						_monitor.status_bits.battery = 0;
						_led.battery_status = 0;
					}
				}

				if(_battery.chargeTimeEn == 1)
					_battery.chargeTime++;
			}
			else
			{
				_monitor.status_bits.battery = 3;
				_led.battery_status = 3;
				_battery.intLevel = 0;
			}
		}
		else
		{
			_monitor.status_bits.battery = 4;
			_led.battery_status = 0;
		}

		battery_populate(_battery.intLevel);

		vTaskDelay(10000);
		//test();
	}
}

uint16_t battery_sample(void)
{
	uint16_t temp = 0;
	uint16_t temp_tot = 0;

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_601Cycles5);
	ADC_Cmd(ADC1, ENABLE);
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));
	ADC_StartConversion(ADC1);

	for(char n = 0; n < 4; n++)
	{
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		temp_tot += ADC_GetConversionValue(ADC1);
	}

	ADC_StopConversion(ADC1);
	ADC_Cmd(ADC1, DISABLE);

	temp = temp_tot/4;

	return (temp*1500/2800);
}

void battery_populate(uint16_t level)
{
	//populate level
	if(level >= 1000 )
		sprintf(_battery.strLevel, "%d", level);
	else
	{
		if(level >= 100 && level < 1000)
		{
			_battery.strLevel[0] = '0';
			sprintf(&_battery.strLevel[1], "%d", level);
		}
		else
		{
			if(level >= 10 && level < 100)
			{
				_battery.strLevel[0] = '0';
				_battery.strLevel[1] = '0';
				sprintf(&_battery.strLevel[2], "%d", level);
			}
			else
			{
				_battery.strLevel[0] = '0';
				_battery.strLevel[1] = '0';
				_battery.strLevel[2] = '0';
				sprintf(&_battery.strLevel[3], "%d", level);
			}
		}
	}
}

void battery_charge_on(void)
{
	GPIO_SetBits(BATTERY_CHARGE_EN_PORT, BATTERY_CHARGE_EN_PIN);
}

void battery_charge_off(void)
{
	GPIO_ResetBits(BATTERY_CHARGE_EN_PORT, BATTERY_CHARGE_EN_PIN);
}

