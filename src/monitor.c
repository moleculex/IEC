#include "platform_config.h"
#include "monitor.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_adc.h"
#include "led.h"
#include "solenoid.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void monitor_init(void);
void monitor_init_digital(void);
void monitor_init_adc1(void);
void monitor_init_adc2(void);
void monitor_analog(void);
void monitor_digital(void);
void monitor_mains_presence_output(void);
void monitor_mains_presence_input(void);

uint16_t monitor_sample(ADC_TypeDef* ADCx, uint8_t ADC_Channel_x);
char monitor_device_identifier(uint16_t value);
char monitor_compatibility_check(t_device* device);
char monitor_compatibility2_check(t_device* device);

void monitor_init_digital(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(MAINS_PRESENCE_RCC | DOOR_SENSOR_RCC | VIBRATION_SENSOR_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = MAINS_PRESENCE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(MAINS_PRESENCE_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = DOOR_SENSOR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(DOOR_SENSOR_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = VIBRATION_SENSOR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(VIBRATION_SENSOR_PORT, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(DOOR2_SENSOR_RCC | VIBRATION2_SENSOR_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DOOR2_SENSOR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(DOOR2_SENSOR_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = VIBRATION2_SENSOR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(VIBRATION2_SENSOR_PORT, &GPIO_InitStructure);
}

void monitor_init_adc1(void)
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	uint32_t calibration_value = 0;

	RCC_ADCCLKConfig(RCC_ADC12PLLCLK_OFF);
	RCC_AHBPeriphClockCmd(MONITOR_ID2_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = MONITOR_ID2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(MONITOR_ID2_PORT, &GPIO_InitStructure);

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

	ADC_Init(ADC1, &ADC_InitStructure);
}

void monitor_init_adc2(void)
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	uint32_t calibration_value = 0;

	RCC_ADCCLKConfig(RCC_ADC12PLLCLK_OFF);
	RCC_AHBPeriphClockCmd(MONITOR_ID0_RCC | MONITOR_ID1_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = MONITOR_ID0_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(MONITOR_ID0_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MONITOR_ID1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(MONITOR_ID1_PORT, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(MONITOR_ID3_RCC |MONITOR_ID4_RCC | MONITOR_ID5_RCC , ENABLE);

	GPIO_InitStructure.GPIO_Pin = MONITOR_ID3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(MONITOR_ID3_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MONITOR_ID4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(MONITOR_ID4_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MONITOR_ID5_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(MONITOR_ID5_PORT, &GPIO_InitStructure);

	ADC_StructInit(&ADC_InitStructure);

	ADC_VoltageRegulatorCmd(ADC2, ENABLE);

	vTaskDelay(5);

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Clock = ADC_Clock_SynClkModeDiv1;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_OneShot;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0;

	ADC_CommonInit(ADC2, &ADC_CommonInitStructure);

	ADC_SelectCalibrationMode(ADC2, ADC_CalibrationMode_Single);

	ADC_StartCalibration(ADC2);

	while(ADC_GetCalibrationStatus(ADC2) != RESET );

	calibration_value = ADC_GetCalibrationValue(ADC2);

	ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;
	ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;
	ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;
	ADC_InitStructure.ADC_NbrOfRegChannel = 1;

	ADC_Init(ADC2, &ADC_InitStructure);
}

void monitor_init(void)
{

	_monitor.status_bits.mains_presence = 0;
	_monitor.status_bits.solenoid_presence = 0;
	_monitor.status_bits.door_sensor_presence = 0;
	_monitor.status_bits.vibration_sensor_presence = 0;
	_monitor.status_bits.door_sensor = 0;
	_monitor.status_bits.vibration_sensor = 0;
	_monitor.status_bits.battery = 0;

	_monitor.pending.mains_presence = 0;
	_monitor.pending.solenoid_presence = 0;
	_monitor.pending.door_sensor_presence = 0;
	_monitor.pending.vibration_sensor_presence = 0;
	_monitor.pending.door_sensor = 0;
	_monitor.pending.vibration_sensor = 0;

	_monitor.vib_port = GPIOB;
	_monitor.door_port = GPIOB;

	_monitor.vib_pin = GPIO_Pin_8;
	_monitor.door_pin = GPIO_Pin_7;

	_monitor.vibration_sensor_count = 0;

	_monitor.door_sensor_previous = 1;

	_monitor.status_bits.solenoid2_presence = 0;
	_monitor.status_bits.door2_sensor_presence = 0;
	_monitor.status_bits.vibration2_sensor_presence = 0;
	_monitor.status_bits.door2_sensor = 0;
	_monitor.status_bits.vibration2_sensor = 0;


	_monitor.pending.solenoid2_presence = 0;
	_monitor.pending.door2_sensor_presence = 0;
	_monitor.pending.vibration2_sensor_presence = 0;
	_monitor.pending.door2_sensor = 0;
	_monitor.pending.vibration2_sensor = 0;

	_monitor.vib2_port = GPIOD;
	_monitor.door2_port = GPIOB;

	_monitor.vib2_pin = GPIO_Pin_2;
	_monitor.door2_pin = GPIO_Pin_5;

	_monitor.vibration2_sensor_count = 0;

	_monitor.door2_sensor_previous = 1;

	_monitor.mains_presence_previous = 1;

	monitor_init_digital();
	monitor_init_adc1();
	monitor_init_adc2();
}

void monitor_manager(void)
{
	monitor_init();

	for(;;)
	{
		monitor_analog();
		monitor_digital();

		vTaskDelay(500);
	}
}

void monitor_analog(void)
{
	_monitor.input_val[0] = monitor_sample(ADC2, ADC_Channel_2); //default sol	-	vib | door | sol
	_monitor.input_val[1] = monitor_sample(ADC2, ADC_Channel_4); //default door
	_monitor.input_val[2] = monitor_sample(ADC1, ADC_Channel_5); //default vib

	_monitor.input_val[3] = monitor_sample(ADC2, ADC_Channel_3); //default sol2	-	vib2 | door2 | sol2
	_monitor.input_val[4] = monitor_sample(ADC2, ADC_Channel_5); //default door2
	_monitor.input_val[5] = monitor_sample(ADC2, ADC_Channel_11); //default vib2

	_monitor.int_input_val[0] = (_monitor.input_val[0]*3300/4096);
	_monitor.int_input_val[1] = (_monitor.input_val[1]*3300/4096);
	_monitor.int_input_val[2] = (_monitor.input_val[2]*3300/4096);

	_monitor.int_input_val[3] = (_monitor.input_val[3]*3300/4096);
	_monitor.int_input_val[4] = (_monitor.input_val[4]*3300/4096);
	_monitor.int_input_val[5] = (_monitor.input_val[5]*3300/4096);

	char response;

	_device[0].type = monitor_device_identifier(_monitor.int_input_val[0]);
	_device[0].port = GPIOB;
	_device[0].pin = GPIO_Pin_9;
	response =	monitor_compatibility_check(&_device[0]);
	if(response == 1)
	{
		_monitor.status_bits.solenoid_presence = 1;

		if(_solenoid.solenoid != 1)
			_led.sol_status = 0;
	}
	else
	{
		_monitor.status_bits.solenoid_presence = 0;

		_led.sol_status = 2;
	}


	_device[1].type = monitor_device_identifier(_monitor.int_input_val[1]);
	_device[1].port = GPIOB;
	_device[1].pin = GPIO_Pin_7;
	response =	monitor_compatibility_check(&_device[1]);
	if(response == 1)
	{
		if(_device[1].type == DEVICE_TYPE_VIB_SENSOR)
		{
			_device[1].previous_type = DEVICE_TYPE_VIB_SENSOR;
			_monitor.vib_port = _device[1].port;
			_monitor.vib_pin = _device[1].pin;
			_led.vib_port = GPIOA;
			_led.vib_pin = GPIO_Pin_10;

			_device[2].previous_type = DEVICE_TYPE_DOOR_SENSOR;
			_monitor.door_port = GPIOB;
			_monitor.door_pin = GPIO_Pin_8;
			_led.door_port = GPIOA;
			_led.door_pin = GPIO_Pin_9;

			_monitor.status_bits.vibration_sensor_presence = 1;
		}

		if(_device[1].type == DEVICE_TYPE_DOOR_SENSOR)
		{
			_device[1].previous_type = DEVICE_TYPE_DOOR_SENSOR;
			_monitor.door_port = _device[1].port;
			_monitor.door_pin = _device[1].pin;
			_led.door_port = GPIOA;
			_led.door_pin = GPIO_Pin_10;

			_device[2].previous_type = DEVICE_TYPE_VIB_SENSOR;
			_monitor.vib_port = GPIOB;
			_monitor.vib_pin = GPIO_Pin_8;
			_led.vib_port = GPIOA;
			_led.vib_pin = GPIO_Pin_9;

			_monitor.status_bits.door_sensor_presence = 1;
		}
	}
	else
	{
		if(_device[1].previous_type == DEVICE_TYPE_VIB_SENSOR)
			_monitor.status_bits.vibration_sensor_presence = 0;

		if(_device[1].previous_type == DEVICE_TYPE_DOOR_SENSOR)
			_monitor.status_bits.door_sensor_presence = 0;
	}


	_device[2].type = monitor_device_identifier(_monitor.int_input_val[2]);
	_device[2].port = GPIOB;
	_device[2].pin = GPIO_Pin_8;
	response =	monitor_compatibility_check(&_device[2]);
	if(response == 1)
	{
		if(_device[2].type == DEVICE_TYPE_VIB_SENSOR)
		{
			_device[2].previous_type = DEVICE_TYPE_VIB_SENSOR;
			_monitor.vib_port = _device[2].port;
			_monitor.vib_pin = _device[2].pin;
			_led.vib_port = GPIOA;
			_led.vib_pin = GPIO_Pin_9;

			_device[1].previous_type = DEVICE_TYPE_DOOR_SENSOR;
			_monitor.door_port = GPIOB;
			_monitor.door_pin = GPIO_Pin_7;
			_led.door_port = GPIOA;
			_led.door_pin = GPIO_Pin_10;

			_monitor.status_bits.vibration_sensor_presence = 1;
		}

		if(_device[2].type == DEVICE_TYPE_DOOR_SENSOR)
		{
			_device[2].previous_type = DEVICE_TYPE_DOOR_SENSOR;
			_monitor.door_port = _device[2].port;
			_monitor.door_pin = _device[2].pin;
			_led.door_port = GPIOA;
			_led.door_pin = GPIO_Pin_9;

			_device[1].previous_type = DEVICE_TYPE_VIB_SENSOR;
			_monitor.vib_port = GPIOB;
			_monitor.vib_pin = GPIO_Pin_7;
			_led.vib_port = GPIOA;
			_led.vib_pin = GPIO_Pin_10;;

			_monitor.status_bits.door_sensor_presence = 1;
		}
	}
	else
	{
		if(_device[2].previous_type == DEVICE_TYPE_VIB_SENSOR)
			_monitor.status_bits.vibration_sensor_presence = 0;

		if(_device[2].previous_type == DEVICE_TYPE_DOOR_SENSOR)
			_monitor.status_bits.door_sensor_presence = 0;
	}

	_device[3].type = monitor_device_identifier(_monitor.int_input_val[3]);
	_device[3].port = GPIOB;
	_device[3].pin = GPIO_Pin_6;
	response =	monitor_compatibility2_check(&_device[3]);
	if(response == 1)
	{
		_monitor.status_bits.solenoid2_presence = 1;

		if(_solenoid.solenoid2 != 1)
			_led.sol2_status = 0;
	}
	else
	{
		_monitor.status_bits.solenoid2_presence = 0;

		_led.sol2_status = 2;
	}


	_device[4].type = monitor_device_identifier(_monitor.int_input_val[4]);
	_device[4].port = GPIOB;
	_device[4].pin = GPIO_Pin_5;
	response =	monitor_compatibility2_check(&_device[4]);
	if(response == 1)
	{
		if(_device[4].type == DEVICE_TYPE_VIB_SENSOR)
		{
			_device[4].previous_type = DEVICE_TYPE_VIB_SENSOR;
			_monitor.vib2_port = _device[4].port;
			_monitor.vib2_pin = _device[4].pin;
			_led.vib2_port = GPIOC;
			_led.vib2_pin = GPIO_Pin_9;

			_device[5].previous_type = DEVICE_TYPE_DOOR_SENSOR;
			_monitor.door2_port = GPIOD;
			_monitor.door2_pin = GPIO_Pin_2;
			_led.door2_port = GPIOC;
			_led.door2_pin = GPIO_Pin_8;

			_monitor.status_bits.vibration2_sensor_presence = 1;
		}

		if(_device[4].type == DEVICE_TYPE_DOOR_SENSOR)
		{
			_device[4].previous_type = DEVICE_TYPE_DOOR_SENSOR;
			_monitor.door2_port = _device[4].port;
			_monitor.door2_pin = _device[4].pin;
			_led.door2_port = GPIOC;
			_led.door2_pin = GPIO_Pin_9;

			_device[5].previous_type = DEVICE_TYPE_VIB_SENSOR;
			_monitor.vib2_port = GPIOD;
			_monitor.vib2_pin = GPIO_Pin_2;
			_led.vib2_port = GPIOC;
			_led.vib2_pin = GPIO_Pin_8;

			_monitor.status_bits.door2_sensor_presence = 1;
		}
	}
	else
	{
		if(_device[4].previous_type == DEVICE_TYPE_VIB_SENSOR)
			_monitor.status_bits.vibration2_sensor_presence = 0;

		if(_device[4].previous_type == DEVICE_TYPE_DOOR_SENSOR)
			_monitor.status_bits.door2_sensor_presence = 0;
	}


	_device[5].type = monitor_device_identifier(_monitor.int_input_val[5]);
	_device[5].port = GPIOD;
	_device[5].pin = GPIO_Pin_2;
	response =	monitor_compatibility2_check(&_device[5]);
	if(response == 1)
	{
		if(_device[5].type == DEVICE_TYPE_VIB_SENSOR)
		{
			_device[5].previous_type = DEVICE_TYPE_VIB_SENSOR;
			_monitor.vib2_port = _device[5].port;
			_monitor.vib2_pin = _device[5].pin;
			_led.vib2_port = GPIOC;
			_led.vib2_pin = GPIO_Pin_8;

			_device[4].previous_type = DEVICE_TYPE_DOOR_SENSOR;
			_monitor.door2_port = GPIOB;
			_monitor.door2_pin = GPIO_Pin_5;
			_led.door2_port = GPIOC;
			_led.door2_pin = GPIO_Pin_9;

			_monitor.status_bits.vibration2_sensor_presence = 1;
		}

		if(_device[5].type == DEVICE_TYPE_DOOR_SENSOR)
		{
			_device[5].previous_type = DEVICE_TYPE_DOOR_SENSOR;
			_monitor.door2_port = _device[5].port;
			_monitor.door2_pin = _device[5].pin;
			_led.door2_port = GPIOC;
			_led.door2_pin = GPIO_Pin_8;

			_device[4].previous_type = DEVICE_TYPE_VIB_SENSOR;
			_monitor.vib2_port = GPIOB;
			_monitor.vib2_pin = GPIO_Pin_5;
			_led.vib2_port = GPIOC;
			_led.vib2_pin = GPIO_Pin_9;

			_monitor.status_bits.door2_sensor_presence = 1;
		}
	}
	else
	{
		if(_device[5].previous_type == DEVICE_TYPE_VIB_SENSOR)
			_monitor.status_bits.vibration2_sensor_presence = 0;

		if(_device[5].previous_type == DEVICE_TYPE_DOOR_SENSOR)
			_monitor.status_bits.door2_sensor_presence = 0;
	}
}

void monitor_digital(void)
{
	monitor_mains_presence_output();
	GPIO_ResetBits(MAINS_PRESENCE_PORT, MAINS_PRESENCE_PIN);
	vTaskDelay(10);
	monitor_mains_presence_input();
	vTaskDelay(10);

	if(GPIO_ReadInputDataBit(MAINS_PRESENCE_PORT, MAINS_PRESENCE_PIN) == 1)
		_monitor.pending.mains_presence = 1;
	else
		_monitor.pending.mains_presence = 0;

	//Door open - Low
	if(GPIO_ReadInputDataBit(_monitor.door_port, _monitor.door_pin) == 1)
		_monitor.pending.door_sensor = 1;
	else
		_monitor.pending.door_sensor = 0;

	if(GPIO_ReadInputDataBit(_monitor.vib_port, _monitor.vib_pin) == 1)
		_monitor.pending.vibration_sensor = 1;
	else
		_monitor.pending.vibration_sensor = 0;

	if(GPIO_ReadInputDataBit(_monitor.door2_port, _monitor.door2_pin) == 1)
		_monitor.pending.door2_sensor = 1;
	else
		_monitor.pending.door2_sensor = 0;

	if(GPIO_ReadInputDataBit(_monitor.vib2_port, _monitor.vib2_pin) == 1)
		_monitor.pending.vibration2_sensor = 1;
	else
		_monitor.pending.vibration2_sensor = 0;

	vTaskDelay(100);

	if(GPIO_ReadInputDataBit(MAINS_PRESENCE_PORT, MAINS_PRESENCE_PIN) == 1 && _monitor.pending.mains_presence == 1)
	{
		_monitor.status_bits.mains_presence = 1;
		_led.mains_presence = 1;
	}
	else
	{
		_monitor.status_bits.mains_presence = 0;
		_led.mains_presence = 0;
	}

	if(_monitor.status_bits.door_sensor_presence == 1)
	{
		if(GPIO_ReadInputDataBit(_monitor.door_port, _monitor.door_pin) == 1 && _monitor.pending.door_sensor == 1)
		{
			_monitor.status_bits.door_sensor = 0;
			_led.door_status = 1;
		}
		else
		{
			_monitor.status_bits.door_sensor = 1;
			_led.door_status = 0;
		}
	}
	else
		_led.door_status = 2;

	if(_monitor.status_bits.vibration_sensor_presence == 1)
	{
		if(GPIO_ReadInputDataBit(_monitor.vib_port, _monitor.vib_pin) == 1 && _monitor.pending.vibration_sensor == 1)
		{
			_monitor.vibration_sensor_count++;
			if(_monitor.vibration_sensor_count > 10)
			{
				_monitor.vibration_sensor_count = 0;
				_monitor.status_bits.vibration_sensor = 1;//cleared after logged by rpc
			}
			_led.vib_status = 1;
		}
		else
		{
			_led.vib_status = 0;
			_monitor.vibration_sensor_count = 0;
		}
	}
	else
		_led.vib_status = 2;

	if(_monitor.status_bits.door2_sensor_presence == 1)
	{
		if(GPIO_ReadInputDataBit(_monitor.door2_port, _monitor.door2_pin) == 1 && _monitor.pending.door2_sensor == 1)
		{
			_monitor.status_bits.door2_sensor = 0;
			_led.door2_status = 1;
		}
		else
		{
			_monitor.status_bits.door2_sensor = 1;
			_led.door2_status = 0;
		}
	}
	else
		_led.door2_status = 2;

	if(_monitor.status_bits.vibration2_sensor_presence == 1)
	{
		if(GPIO_ReadInputDataBit(_monitor.vib2_port, _monitor.vib2_pin) == 1 && _monitor.pending.vibration2_sensor == 1)
		{
			_monitor.vibration2_sensor_count++;
			if(_monitor.vibration2_sensor_count > 10)
			{
				_monitor.vibration2_sensor_count = 0;
				_monitor.status_bits.vibration2_sensor = 1; //cleared after logged by rpc
			}
			_led.vib2_status = 1;
		}
		else
		{
			_led.vib2_status = 0;
			_monitor.vibration2_sensor_count = 0;
		}
	}
	else
		_led.vib2_status = 2;

}

uint16_t monitor_sample(ADC_TypeDef* ADCx, uint8_t ADC_Channel_x)
{
	uint16_t temp = 0;
	uint16_t temp_tot = 0;

	ADC_RegularChannelConfig(ADCx, ADC_Channel_x, 1, ADC_SampleTime_601Cycles5);
	ADC_Cmd(ADCx, ENABLE);
	while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_RDY));
	ADC_StartConversion(ADCx);

	for(char n = 0; n < 4; n++)
	{
		while(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);
		temp_tot += ADC_GetConversionValue(ADCx);
	}

	ADC_StopConversion(ADCx);
	ADC_Cmd(ADCx, DISABLE);

	temp = temp_tot/4;

	return temp;
}

char monitor_device_identifier(uint16_t value)
{
	char device_type = 0;

	if(value > 750 && value < 950)
		device_type = DEVICE_TYPE_SOLENOID;

	if(value > 1550 && value < 1750)
		device_type = DEVICE_TYPE_VIB_SENSOR;

	if(value > 2350 && value < 2550)
		device_type = DEVICE_TYPE_DOOR_SENSOR;

	return device_type;
}

char monitor_compatibility_check(t_device* device)
{
	char retval = 0;

	if(device->type == DEVICE_TYPE_SOLENOID)
	{
		if(device->port == GPIOB && device->pin == GPIO_Pin_9)
			retval = 1;

		if((device->port == GPIOB) && (device->pin == GPIO_Pin_7 || device->pin == GPIO_Pin_8))
			retval = 2;
	}

	if(device->type == DEVICE_TYPE_DOOR_SENSOR || device->type == DEVICE_TYPE_VIB_SENSOR)
	{
		if((device->port == GPIOB) && (device->pin == GPIO_Pin_7 || device->pin == GPIO_Pin_8))
			retval = 1;

		if((device->port == GPIOB) && (device->pin == GPIO_Pin_9))
			retval = 2;
	}

	return retval;
}

char monitor_compatibility2_check(t_device* device)
{
	char retval = 0;

	if(device->type == DEVICE_TYPE_SOLENOID)
	{
		if(device->port == GPIOB && device->pin == GPIO_Pin_6)
			retval = 1;

		if((device->port == GPIOB) && (device->pin == GPIO_Pin_5))
			retval = 2;

		if((device->port == GPIOD) && (device->pin == GPIO_Pin_2))
			retval = 2;
	}

	if(device->type == DEVICE_TYPE_DOOR_SENSOR || device->type == DEVICE_TYPE_VIB_SENSOR)
	{
		if((device->port == GPIOB) && (device->pin == GPIO_Pin_5))
			retval = 1;

		if((device->port == GPIOD) && (device->pin == GPIO_Pin_2))
			retval = 1;

		if((device->port == GPIOB) && (device->pin == GPIO_Pin_6))
			retval = 2;
	}

	return retval;
}

void monitor_mains_presence_output(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = MAINS_PRESENCE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(MAINS_PRESENCE_PORT, &GPIO_InitStructure);
}

void monitor_mains_presence_input(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = MAINS_PRESENCE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(MAINS_PRESENCE_PORT, &GPIO_InitStructure);
}
