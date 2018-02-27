#include "platform_config.h"
#include "modem.h"
#include "led.h"
#include "monitor.h"
#include "solenoid.h"
#include "battery.h"
#include "stm32f30x_usart.h"
#include "stm32f30x_misc.h"
#include "string.h"
#include "stm32f30x_iwdg.h"
#include "flash.h"
#include "pkt.h"
#include "stdlib.h"
#include "stdio.h"
#include "buzzer.h"
#include "cfg.h"
#include "cmd.h"
#ifdef ZIM
#include "_3phase.h"
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void modem_init(void);
char modem_api(void);
void modem_downstream(char *buffer);
void modem_upstream(void);

const char sys_reset[] = "sys reset\r\n";
const char mac_set_appkey[] = "mac set appkey 13341334133413341334133413341334\r\n";
const char mac_set_appeui[] = "mac set appeui 70B3D576AE000007\r\n";
const char sys_get_hweui[] = "sys get hweui\r\n";
const char mac_set_deveui[] = "mac set deveui ";
const char mac_join_otaa[] = "mac join otaa\r\n";

void modem_init(void)
{
	buffer_init(&_modem.wr_buffer);
	buffer_init(&_modem.rd_buffer);
	_modem.modem = '0';
	_modem.upstream.alarm.timer = 0;
	_modem.upstream.status.timer = 0;
	_modem.upstream.alarm.allow = 1;

	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB | MODEM_RST_RCC , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	//RST
	GPIO_InitStructure.GPIO_Pin = MODEM_RST_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //was OD
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MODEM_RST_PORT, &GPIO_InitStructure);
	GPIO_SetBits(MODEM_RST_PORT,MODEM_RST_PIN);

	/* Connect USART pins to AF */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_7);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_7);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure USART Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 57600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3,&USART_InitStructure);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART3, ENABLE);
}


void modem_manager(void)
{
	char logon_count = 0;
	_modem.state = MODEM_START_UP;
	modem_init();

	for(;;)
	{
		switch(_modem.state)
		{
			case MODEM_START_UP:
				if(logon_count < 5)
				{
					modem_startup();
					logon_count++;
					_modem.state = MODEM_API;
				}
				else
				{
					GPIO_ResetBits(MODEM_RST_PORT,MODEM_RST_PIN);
					vTaskDelay(1000);
					led_on(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
					GPIO_SetBits(MODEM_RST_PORT,MODEM_RST_PIN);
					vTaskDelay(1000);
					led_off(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
					GPIO_ResetBits(MODEM_RST_PORT,MODEM_RST_PIN);
					vTaskDelay(1000);

					modem_mac_pause();
					vTaskDelay(1000);
					memcpy(&_modem.wr_buffer.data, (const char *)"radio set sf sf12\r\n", 19);
					buffer_init(&_modem.rd_buffer);
					modem_write(&_modem.wr_buffer, 19);
					vTaskDelay(200);

					memcpy(&_modem.wr_buffer.data, (const char *)"radio set rxbw 125\r\n", 20);
					buffer_init(&_modem.rd_buffer);
					modem_write(&_modem.wr_buffer, 20);
					vTaskDelay(200);

					memcpy(&_modem.wr_buffer.data, (const char *)"radio set freq 869525000\r\n", 26);
					buffer_init(&_modem.rd_buffer);
					modem_write(&_modem.wr_buffer, 26);
					vTaskDelay(200);
					modem_radio_rx_0();
					logon_count = 0;
					vTaskDelay(300000);
				}

				break;

			case MODEM_API:
				switch(modem_api())
				{
					case 0:
						break;

					case 1:
						_led.modem = 0;
						_modem.modem = '0';
						_modem.state = MODEM_START_UP;
						vTaskDelay(2000);
						break;

					case 2:
						_led.modem = 1;
						_modem.modem = '1';
						break;
				}


				break;

		}

		vTaskDelay(200);
	}
}

void skeleton_key(void)
{
	char *index;

	for(;;)
	{
		index = strstr(_modem.rd_buffer.data, "radio_rx");
		if(strncmp(index, "radio_rx", 8) == 0 && _modem.modem != '1')
		{
			vTaskDelay(200);
			packet_disassemble(&index[10]);
			payloadLen = atoi(packet_getPayloadLength());
			if(strncmp(packet_getDestinationId(), _modem.deveui, 16) == 0)
			{
				hexStr_to_hex(_modem.rd_buffer.data, packet_getPayload(), payloadLen);

				payloadLen /= 2;
				sprintf(strPayloadLen, "%04d", payloadLen);
				packet_setPayloadLength(strPayloadLen);
				packet_setPayload(_modem.rd_buffer.data);

				if(strncmp(packet_getPayload(), "activateSolenoid", payloadLen) == 0)
				{
					_solenoid.solenoid = 1;
				}
			}

			buffer_init(&_modem.rd_buffer);
			modem_radio_rx_0();
			vTaskDelay(200);
		}

		index = strstr(_modem.rd_buffer.data, "radio_err");
		if(strncmp(index, "radio_err", 9) == 0 && _modem.modem != '1')
		{
			buffer_init(&_modem.rd_buffer);
			modem_radio_rx_0();
			vTaskDelay(200);
		}

		vTaskDelay(500);
	}
}

void modem_startup(void)
{

	if(xSemaphoreTake(xSemaphore, 1000))
	{
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
		vTaskDelay(200);

		xSemaphoreGive(xSemaphore);
	}

	buzzer_on();
	vTaskDelay(50);
	buzzer_off();
	vTaskDelay(50);
	buzzer_on();
	vTaskDelay(50);
	buzzer_off();
	vTaskDelay(50);

	GPIO_ResetBits(MODEM_RST_PORT,MODEM_RST_PIN);
	vTaskDelay(1000);
	led_on(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
	GPIO_SetBits(MODEM_RST_PORT,MODEM_RST_PIN);
	vTaskDelay(1000);
	led_off(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
	GPIO_ResetBits(MODEM_RST_PORT,MODEM_RST_PIN);
	vTaskDelay(1000);

	memcpy(&_modem.wr_buffer.data, sys_reset, sizeof sys_reset);
	buffer_init(&_modem.rd_buffer);
	modem_write(&_modem.wr_buffer, sizeof sys_reset);
	led_on(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
	vTaskDelay(200);

	modem_mac_init();
}

void modem_mac_init(void)
{
	memcpy(&_modem.wr_buffer.data, sys_get_hweui, sizeof sys_get_hweui);
	buffer_init(&_modem.rd_buffer);
	modem_write(&_modem.wr_buffer, sizeof sys_get_hweui);
	led_off(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
	vTaskDelay(200);

	memcpy(&_modem.deveui, &_modem.rd_buffer.data, 16);
	memcpy(&_modem.wr_buffer.data, mac_set_deveui, sizeof mac_set_deveui);
	memcpy(&_modem.wr_buffer.data[15], _modem.deveui, sizeof _modem.deveui);
	memcpy(&_modem.wr_buffer.data[31], (const char *)"\r\n", 2);
	buffer_init(&_modem.rd_buffer);
	modem_write(&_modem.wr_buffer, 33);
	led_on(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
	vTaskDelay(200);

	memcpy(&_modem.wr_buffer.data, mac_set_appeui, sizeof mac_set_appeui);
	buffer_init(&_modem.rd_buffer);
	modem_write(&_modem.wr_buffer, sizeof mac_set_appeui);
	led_off(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
	vTaskDelay(200);

	memcpy(&_modem.wr_buffer.data, mac_set_appkey, sizeof mac_set_appkey);
	buffer_init(&_modem.rd_buffer);
	modem_write(&_modem.wr_buffer, sizeof mac_set_appkey);
	led_on(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
	vTaskDelay(200);

	memcpy(&_modem.wr_buffer.data, (const char *)"mac set dr 1\r\n", 14);
	buffer_init(&_modem.rd_buffer);
	modem_write(&_modem.wr_buffer, 14);
	led_off(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
	vTaskDelay(200);

	memcpy(&_modem.wr_buffer.data, (const char *)"mac set adr off\r\n", 17);
	buffer_init(&_modem.rd_buffer);
	modem_write(&_modem.wr_buffer, 17);
	led_on(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
	vTaskDelay(200);

	modem_mac_join();
	led_off(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
	vTaskDelay(200);
}

void modem_radio_init(void)
{
	memcpy(&_modem.wr_buffer.data, (const char *)"radio set sf sf12\r\n", 19);
	buffer_init(&_modem.rd_buffer);
	modem_write(&_modem.wr_buffer, 19);
	vTaskDelay(200);

	memcpy(&_modem.wr_buffer.data, (const char *)"radio set rxbw 125\r\n", 20);
	buffer_init(&_modem.rd_buffer);
	modem_write(&_modem.wr_buffer, 20);
	vTaskDelay(200);

	memcpy(&_modem.wr_buffer.data, (const char *)"radio set freq 869525000\r\n", 26);
	buffer_init(&_modem.rd_buffer);
	modem_write(&_modem.wr_buffer, 26);
	vTaskDelay(200);

	memcpy(&_modem.wr_buffer.data, (const char *)"radio set wdt 60000\r\n", 21);
	buffer_init(&_modem.rd_buffer);
	modem_write(&_modem.wr_buffer, 21);
	//modem_radio_set_wdt((char *)"60000", 5);
	vTaskDelay(200);

}

char modem_api(void)
{
	char retval = 0;
	char *index;

	index = strstr(_modem.rd_buffer.data, "accepted");
	if(strncmp(index, "accepted", 8) == 0)
	{
		vTaskDelay(5000);//200

		memcpy(&_modem.wr_buffer.data, (const char *)"mac get devaddr\r\n", 17);
		buffer_init(&_modem.rd_buffer);
		modem_write(&_modem.wr_buffer, 17);
		buzzer_on();
		vTaskDelay(200);
		buzzer_off();

		_modem.devaddr[0] = _modem.rd_buffer.data[6];
		_modem.devaddr[1] = _modem.rd_buffer.data[7];
		_modem.devaddr[2] = _modem.rd_buffer.data[4];
		_modem.devaddr[3] = _modem.rd_buffer.data[5];
		_modem.devaddr[4] = _modem.rd_buffer.data[2];
		_modem.devaddr[5] = _modem.rd_buffer.data[3];
		_modem.devaddr[6] = _modem.rd_buffer.data[0];
		_modem.devaddr[7] = _modem.rd_buffer.data[1];

		//memcpy(&_modem.devaddr, &_modem.rd_buffer.data, 8);
		_led.modem = 1;
		_modem.modem = '1';

		vTaskDelay(15000);
		modem_prepare_packet();
#ifdef ZIM
		modem_send(&_modem.temp_buffer, 14 * 2);
#else
		modem_send(&_modem.temp_buffer, 11 * 2);
#endif
		led_off(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
		vTaskDelay(200);
		led_on(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
		vTaskDelay(5000);

		modem_mac_pause();
		vTaskDelay(1000);
		modem_radio_init();
		vTaskDelay(200);
		modem_radio_rx_0();
		vTaskDelay(200);

		retval = 2;
	}

	index = strstr(_modem.rd_buffer.data, "denied");
	if(strncmp(index, "denied", 6) == 0)
	{
		vTaskDelay(100);
		buffer_init(&_modem.rd_buffer);
		retval = 1;
	}

	index = strstr(_modem.rd_buffer.data, "not_joined");
	if(strncmp(index, "not_joined", 10) == 0)
	{
		vTaskDelay(100);
		buffer_init(&_modem.rd_buffer);
		retval = 1;
	}

	index = strstr(_modem.rd_buffer.data, "radio_rx");
	if(strncmp(index, "radio_rx", 8) == 0 && _modem.modem == '1')
	{
		vTaskDelay(100);

		modem_downstream(&index[12]);

		buffer_init(&_modem.rd_buffer);
		modem_mac_pause();
		vTaskDelay(1000);

		modem_radio_init();
		vTaskDelay(200);
		modem_radio_rx_0();
		vTaskDelay(200);
	}

	//1 min radio timeout
	index = strstr(_modem.rd_buffer.data, "radio_err");
	if(strncmp(index, "radio_err", 9) == 0 && _modem.modem == '1')
	{
		vTaskDelay(100);
		buffer_init(&_modem.rd_buffer);

		flash_read(CFG_ROOT, _cfg.buffer, CFG_BUFFER_SIZE);
		if(_cfg.empty != 0x3C)
			cfg_default();
		else
		{
			_cfg.alarm_reset_interval = cfg_byteSwap(_cfg.alarm_reset_interval);
			_cfg.alarm_status_interval = cfg_byteSwap(_cfg.alarm_status_interval);
			_cfg.reboot_interval = cfg_byteSwap(_cfg.reboot_interval);
		}

		if(_modem.upstream.alarm.timer >= _cfg.alarm_reset_interval)
		{
			_modem.upstream.alarm.timer = 0;
			_modem.upstream.alarm.allow = 1;
		}
		else
		{
			if(_modem.upstream.status.timer >=_cfg.alarm_status_interval)
			{
				_modem.upstream.status.timer = 0;
				_modem.upstream.state = MODEM_UPSTREAM_STATUS;
			}
			else
			{
				if(_modem.upstream.state != MODEM_UPSTREAM_RESPONSE)
					_modem.upstream.state = MODEM_UPSTREAM_IDLE;
			}
		}

		if((
		   ((((_monitor.status_bits.door_sensor  == 0) && (_monitor.door_sensor_previous == 1)) || ((_monitor.status_bits.door_sensor  == 1) && (_monitor.door_sensor_previous == 0))) && _cfg.alarms_in_use.door_sensor == 1 ) ||
		   (_monitor.status_bits.door_sensor_presence == 0  && _cfg.alarms_in_use.door_sensor_presence == 1 ) ||
		   (_monitor.status_bits.vibration_sensor == 1 && _cfg.alarms_in_use.vibration_sensor == 1 ) ||
		   (_monitor.status_bits.vibration_sensor_presence == 0 && _cfg.alarms_in_use.vibration_sensor_presence == 1 ) ||
		   (_monitor.status_bits.solenoid_presence == 0 && _cfg.alarms_in_use.solenoid_presence == 1 ) ||
#ifdef TWO_DOOR
		   ((((_monitor.status_bits.door2_sensor  == 0) && (_monitor.door2_sensor_previous == 1)) || ((_monitor.status_bits.door2_sensor  == 1) && (_monitor.door2_sensor_previous == 0))) && _cfg.alarms_in_use.door_sensor == 1 ) ||
		   (_monitor.status_bits.door2_sensor_presence == 0  && _cfg.alarms_in_use.door_sensor_presence == 1 ) ||
#ifdef ZIM
		   (_monitor.status_bits.vibration2_sensor == 1 && _cfg.alarms_in_use.vibration_sensor == 1 ) ||
		   (_monitor.status_bits.vibration2_sensor_presence == 0 && _cfg.alarms_in_use.vibration_sensor_presence == 1 ) ||
#endif
		   (_monitor.status_bits.solenoid2_presence == 0 && _cfg.alarms_in_use.solenoid_presence == 1 ) ||
#endif
		   (_monitor.status_bits.battery == 3 && _cfg.alarms_in_use.battery_status == 1 ) ||
		   ((((_monitor.status_bits.mains_presence == 0) && (_monitor.mains_presence_previous == 1)) || ((_monitor.status_bits.mains_presence == 1) && (_monitor.mains_presence_previous == 0)))  && _cfg.alarms_in_use.mains_presence == 1
		   )) && _modem.upstream.alarm.allow == 1)
		{
			_monitor.door_sensor_previous = _monitor.status_bits.door_sensor;
			_monitor.mains_presence_previous = _monitor.status_bits.mains_presence;
#ifdef TWO_DOOR
			_monitor.door2_sensor_previous = _monitor.status_bits.door2_sensor;
#endif
			_modem.upstream.alarm.timer = 0;
			_modem.upstream.alarm.allow = 0;
			_modem.upstream.state = MODEM_UPSTREAM_ALARM;
		}

		_modem.upstream.alarm.timer++;
		_modem.upstream.status.timer++;

		modem_upstream();
	}

	return retval;
}

void test(void)
{

	flash_read(CFG_ROOT, _cfg.buffer, CFG_BUFFER_SIZE);
	if(_cfg.empty != 0x3C)
		cfg_default();
	else
	{
		_cfg.alarm_reset_interval = cfg_byteSwap(_cfg.alarm_reset_interval);
		_cfg.alarm_status_interval = cfg_byteSwap(_cfg.alarm_status_interval);
		_cfg.reboot_interval = cfg_byteSwap(_cfg.reboot_interval);
	}

	if((
	   ((((_monitor.status_bits.door_sensor  == 0) && (_monitor.door_sensor_previous == 1)) || ((_monitor.status_bits.door_sensor  == 1) && (_monitor.door_sensor_previous == 0))) && _cfg.alarms_in_use.door_sensor == 1 ) ||
	   (_monitor.status_bits.door_sensor_presence == 0  && _cfg.alarms_in_use.door_sensor_presence == 1 ) ||
	   (_monitor.status_bits.vibration_sensor == 1 && _cfg.alarms_in_use.vibration_sensor == 1 ) ||
	   (_monitor.status_bits.vibration_sensor_presence == 0 && _cfg.alarms_in_use.vibration_sensor_presence == 1 ) ||
	   (_monitor.status_bits.solenoid_presence == 0 && _cfg.alarms_in_use.solenoid_presence == 1 ) ||
#ifdef TWO_DOOR
	   ((((_monitor.status_bits.door2_sensor  == 0) && (_monitor.door2_sensor_previous == 1)) || ((_monitor.status_bits.door2_sensor  == 1) && (_monitor.door2_sensor_previous == 0))) && _cfg.alarms_in_use.door_sensor == 1 ) ||
	   (_monitor.status_bits.door2_sensor_presence == 0  && _cfg.alarms_in_use.door_sensor_presence == 1 ) ||
	   (_monitor.status_bits.vibration2_sensor == 1 && _cfg.alarms_in_use.vibration_sensor == 1 ) ||
	   (_monitor.status_bits.vibration2_sensor_presence == 0 && _cfg.alarms_in_use.vibration_sensor_presence == 1 ) ||
	   (_monitor.status_bits.solenoid2_presence == 0 && _cfg.alarms_in_use.solenoid_presence == 1 ) ||
#endif
	   (_monitor.status_bits.battery == 3 && _cfg.alarms_in_use.battery_status == 1 ) ||
	   ((((_monitor.status_bits.mains_presence == 0) && (_monitor.mains_presence_previous == 1)) || ((_monitor.status_bits.mains_presence == 1) && (_monitor.mains_presence_previous == 0)))  && _cfg.alarms_in_use.mains_presence == 1)
	   ) && _modem.upstream.alarm.allow == 1)
	{
		_monitor.door_sensor_previous = _monitor.status_bits.door_sensor;
		_monitor.mains_presence_previous = _monitor.status_bits.mains_presence;
#ifdef TWO_DOOR
		_monitor.door2_sensor_previous = _monitor.status_bits.door2_sensor;
#endif
		_modem.upstream.alarm.timer = 0;
		_modem.upstream.alarm.allow = 0;
		_modem.upstream.state = MODEM_UPSTREAM_ALARM;
	}
}

void modem_downstream(char *buffer)
{
	char *index;

	if((strncmp(buffer, _modem.devaddr, 8) == 0) || (strncmp(buffer, "77777777", 8) == 0))
	{
		index = strstr(_modem.rd_buffer.data, "3F3F3F");
		hexStr_to_hex(_cmd.buffer, index, CMD_BUFFER_SIZE);
		_cmd.src = CMD_SRC_MODEM;
		cmd_process();
	}
}

void modem_upstream(void)
{
	switch(_modem.upstream.state)
	{
		case MODEM_UPSTREAM_STATUS:
			modem_mac_resume();
			vTaskDelay(1000);

			modem_prepare_packet();

			modem_send(&_modem.temp_buffer, 11 * 2);

			led_off(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
			vTaskDelay(200);
			led_on(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
			vTaskDelay(5000);

			modem_mac_pause();
			vTaskDelay(1000);

			modem_radio_init();
			vTaskDelay(200);
			modem_radio_rx_0();
			vTaskDelay(200);

			_modem.upstream.state = MODEM_UPSTREAM_IDLE;
			break;

		case MODEM_UPSTREAM_ALARM:
			modem_mac_resume();
			vTaskDelay(1000);

			modem_prepare_packet();
#ifdef ZIM
			modem_send(&_modem.temp_buffer, 14 * 2);
#else
			modem_send(&_modem.temp_buffer, 11 * 2);
#endif

			led_off(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
			vTaskDelay(200);
			led_on(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
			vTaskDelay(5000);

			modem_mac_pause();
			vTaskDelay(1000);

			modem_radio_init();
			vTaskDelay(200);
			modem_radio_rx_0();
			vTaskDelay(200);

			_modem.upstream.state = MODEM_UPSTREAM_IDLE;
			break;

		case MODEM_UPSTREAM_RESPONSE:
			modem_mac_resume();
			vTaskDelay(1000);

			modem_send(&_modem.temp_buffer, _modem.temp_buffer.count);
			led_off(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
			vTaskDelay(200);
			led_on(LED_MODEM_CONNECTED_PORT, LED_MODEM_CONNECTED_PIN);
			vTaskDelay(5000);

			modem_mac_pause();
			vTaskDelay(1000);

			modem_radio_init();
			vTaskDelay(200);
			modem_radio_rx_0();
			vTaskDelay(200);

			_modem.upstream.state = MODEM_UPSTREAM_IDLE;
			break;

		case MODEM_UPSTREAM_IDLE:
			modem_mac_resume();
			vTaskDelay(5000);
			modem_mac_pause();
			vTaskDelay(1000);

			modem_radio_init();
			vTaskDelay(200);
			modem_radio_rx_0();
			vTaskDelay(200);
			break;

	}
}

void modem_write(t_buffer *buffer, char length)
{
	for(uint8_t i = 0; i < length; i++)
	{
		USART_SendData(USART3, buffer->data[i]);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	}
}

void modem_send(t_buffer *buffer, size_t length)
{
	buffer_init(&_modem.rd_buffer);

	memcpy(&_modem.wr_buffer.data, (char *)"mac tx uncnf 10 ", 16);
	memcpy(&_modem.wr_buffer.data[16], buffer->data, length);
	memcpy(&_modem.wr_buffer.data[16 + length], (char *)"\r\n", 2);
	modem_write(&_modem.wr_buffer,(16 + length + 2));
}

void modem_mac_pause(void)
{
	memcpy(&_modem.wr_buffer.data, (const char *)"mac pause\r\n", 11);
	buffer_init(&_modem.rd_buffer);
	modem_write(&_modem.wr_buffer, 11);
}

void modem_radio_rx_0(void)
{
	memcpy(&_modem.wr_buffer.data, (const char *)"radio rx 0\r\n", 12);
	buffer_init(&_modem.rd_buffer);
	modem_write(&_modem.wr_buffer, 12);
}

void modem_radio_set_wdt(char *period, size_t length)
{
	memcpy(&_modem.wr_buffer.data, (const char *)"radio set wdt ", 14);
	memcpy(&_modem.wr_buffer.data[14], period, length);
	memcpy(&_modem.wr_buffer.data[14 + length], (const char *)"\r\n", 2);
	buffer_init(&_modem.rd_buffer);
	modem_write(&_modem.wr_buffer, (16 + length));
}

void modem_mac_resume(void)
{
	memcpy(&_modem.wr_buffer.data, (const char *)"mac resume\r\n", 12);
	buffer_init(&_modem.rd_buffer);
	modem_write(&_modem.wr_buffer, 12);
}

void modem_mac_join(void)
{
	memcpy(&_modem.wr_buffer.data, mac_join_otaa, sizeof mac_join_otaa);
	buffer_init(&_modem.rd_buffer);
	modem_write(&_modem.wr_buffer, sizeof mac_join_otaa);
}

void modem_prepare_packet(void)
{
	_cmd.preamble[0] = 0x3F;
	_cmd.preamble[1] = 0x3F;
	_cmd.preamble[2] = 0x3F;
	_cmd.type= 0x01;
	_cmd.erw = 0xF2;
	_cmd.id = 0xA4;
#ifdef ZIM
	_cmd.len = 0x07;
#else
	_cmd.len = 0x04;
#endif

	_cmd.payload[0] = _monitor.status_bits.status[0];
	_cmd.payload[1] = _monitor.status_bits.status[1];
	_monitor.status_bits.vibration_sensor = 0;
	_monitor.status_bits.vibration2_sensor = 0;
	_cmd.payload[2] = (_battery.strLevel[0] << 4) | (_battery.strLevel[1] & 0X0F);
	_cmd.payload[3] = (_battery.strLevel[2] << 4) | (_battery.strLevel[3] & 0X0F);

#ifdef ZIM
		_cmd.payload[4] = _3phase.v[0];
		_cmd.payload[5] = _3phase.v[1];
	    _cmd.payload[6] = _3phase.v[2];

	    hex_to_hexStr(&_cmd.buffer[0], &_modem.temp_buffer.data[0], 14);
#else
		hex_to_hexStr(&_cmd.buffer[0], &_modem.temp_buffer.data[0], 11);
#endif
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		char inchar = (char)USART_ReceiveData(USART3);
		buffer_push(&_modem.rd_buffer, inchar);
	}
}
