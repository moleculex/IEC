#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
//version: EMM-FINAL
//date: 2017/02/21 09:11
#define TWO_DOOR
#define ZIM

//SPI 3PHASE
#define SPIx                             SPI2
#define SPIx_CLK                         RCC_APB1Periph_SPI2
#define SPIx_IRQn                        SPI2_IRQn

#define SPIx_SCK_PIN                     GPIO_Pin_13
#define SPIx_SCK_GPIO_PORT               GPIOB
#define SPIx_SCK_GPIO_CLK                RCC_AHBPeriph_GPIOB
#define SPIx_SCK_SOURCE                  GPIO_PinSource13
#define SPIx_SCK_AF                      GPIO_AF_5

#define SPIx_MISO_PIN                    GPIO_Pin_14
#define SPIx_MISO_GPIO_PORT              GPIOB
#define SPIx_MISO_GPIO_CLK               RCC_AHBPeriph_GPIOB
#define SPIx_MISO_SOURCE                 GPIO_PinSource14
#define SPIx_MISO_AF                     GPIO_AF_5

#define SPIx_MOSI_PIN                    GPIO_Pin_15
#define SPIx_MOSI_GPIO_PORT              GPIOB
#define SPIx_MOSI_GPIO_CLK               RCC_AHBPeriph_GPIOB
#define SPIx_MOSI_SOURCE                 GPIO_PinSource15
#define SPIx_MOSI_AF                     GPIO_AF_5

#define SPIx_CS_PIN                      GPIO_Pin_12
#define SPIx_CS_GPIO_PORT                GPIOB
#define SPIx_CS_GPIO_CLK                 RCC_AHBPeriph_GPIOB
#define SPIx_CS_SOURCE                   GPIO_PinSource12

//RS485
#define RS485_RX_PORT		GPIOA
#define RS485_RX_PIN		GPIO_Pin_3
#define RS485_RX_PIN_SOURCE GPIO_PinSource3
#define RS485_RX_RCC_AHB	RCC_AHBPeriph_GPIOA
#define RS485_RX_RCC_APB	RCC_APB1Periph_USART2
#define RS485_RX_USART		USART2

#define RS485_TX_PORT		GPIOA
#define RS485_TX_PIN		GPIO_Pin_2
#define RS485_TX_PIN_SOURCE GPIO_PinSource2
#define RS485_TX_RCC_AHB	RCC_AHBPeriph_GPIOA
#define RS485_TX_RCC_APB	RCC_APB1Periph_USART2
#define RS485_TX_USART		USART2

#define RS485_RW_PORT		GPIOA
#define RS485_RW_PIN		GPIO_Pin_1 //!RE/DE
#define RS485_RW_RCC		RCC_AHBPeriph_GPIOA
#define RS485_IRQN			USART2_IRQn

//PRESENCE
#define MAINS_PRESENCE_PORT		GPIOC
#define MAINS_PRESENCE_PIN		GPIO_Pin_10
#define MAINS_PRESENCE_RCC		RCC_AHBPeriph_GPIOC

/*#define SOLENOID_PRESENCE_PORT			GPIOA
#define SOLENOID_PRESENCE_PIN 			GPIO_Pin_5
#define SOLENOID_PRESENCE_RCC*/			/*RCC_AHBPeriph_ADC12 |*/ /*RCC_AHBPeriph_GPIOA*/

#define DOOR_SENSOR_PRESENCE_PORT		GPIOA
#define DOOR_SENSOR_PRESENCE_PIN 		GPIO_Pin_7
#define DOOR_SENSOR_PRESENCE_RCC		/*RCC_AHBPeriph_ADC12 |*/ RCC_AHBPeriph_GPIOA

#define VIBRATION_SENSOR_PRESENCE_PORT		GPIOF
#define VIBRATION_SENSOR_PRESENCE_PIN 		GPIO_Pin_4
#define VIBRATION_SENSOR_PRESENCE_RCC		/*RCC_AHBPeriph_ADC12 |*/ RCC_AHBPeriph_GPIOF

//MODEM
#define MODEM_RX_PORT		GPIOB
#define MODEM_RX_PIN		GPIO_Pin_11
#define MODEM_RX_RCC_AHB	RCC_AHBPeriph_GPIOB
#define MODEM_RX_RCC_APB	RCC_APB1Periph_USART3
#define MODEM_RX_USART		USART3

#define MODEM_TX_PORT		GPIOB
#define MODEM_TX_PIN		GPIO_Pin_10
#define MODEM_TX_RCC_AHB	RCC_AHBPeriph_GPIOB
#define MODEM_TX_RCC_APB	RCC_APB1Periph_USART3
#define MODEM_TX_USART		USART3

#define MODEM_RTS_PORT		GPIOB
#define MODEM_RTS_PIN		GPIO_Pin_14
#define MODEM_RTS_RCC		RCC_AHBPeriph_GPIOB

#define MODEM_CTS_PORT		GPIOB
#define MODEM_CTS_PIN		GPIO_Pin_13
#define MODEM_CTS_RCC		RCC_AHBPeriph_GPIOB

//was B5
#define MODEM_RST_PORT		GPIOC
#define MODEM_RST_PIN		GPIO_Pin_11
#define MODEM_RST_RCC		RCC_AHBPeriph_GPIOC

//SOLENOID
#define SOLENOID_PORT			GPIOB
#define SOLENOID_PIN 			GPIO_Pin_9
#define SOLENOID_RCC			RCC_AHBPeriph_GPIOB

#define SOLENOID2_PORT			GPIOB
#define SOLENOID2_PIN 			GPIO_Pin_6
#define SOLENOID2_RCC			RCC_AHBPeriph_GPIOB

//SENSORS
#define DOOR_SENSOR_PORT		GPIOB
#define DOOR_SENSOR_PIN 		GPIO_Pin_7
#define DOOR_SENSOR_RCC			RCC_AHBPeriph_GPIOB

#define VIBRATION_SENSOR_PORT		GPIOB
#define VIBRATION_SENSOR_PIN 		GPIO_Pin_8
#define VIBRATION_SENSOR_RCC		RCC_AHBPeriph_GPIOB

#define DOOR2_SENSOR_PORT		GPIOB
#define DOOR2_SENSOR_PIN 		GPIO_Pin_5
#define DOOR2_SENSOR_RCC			RCC_AHBPeriph_GPIOB

#define VIBRATION2_SENSOR_PORT		GPIOD
#define VIBRATION2_SENSOR_PIN 		GPIO_Pin_2
#define VIBRATION2_SENSOR_RCC		RCC_AHBPeriph_GPIOD

//LEDs
#ifdef ZIM
#define LED_MODEM_CONNECTED_PORT	GPIOC
#define LED_MODEM_CONNECTED_PIN		GPIO_Pin_12 //LED1
#define LED_MODEM_CONNECTED_RCC		RCC_AHBPeriph_GPIOB
#else
#define LED_MODEM_CONNECTED_PORT	GPIOB
#define LED_MODEM_CONNECTED_PIN		GPIO_Pin_15 //LED1
#define LED_MODEM_CONNECTED_RCC		RCC_AHBPeriph_GPIOB
#endif

#define LED_MAINS_PRESENCE_PORT		GPIOC
#define LED_MAINS_PRESENCE_PIN		GPIO_Pin_6 //LED2
#define LED_MAINS_PRESENCE_RCC		RCC_AHBPeriph_GPIOC

#ifdef ZIM
#define LED_SOLENOID_PORT		GPIOC
#define LED_SOLENOID_PIN		GPIO_Pin_13 //LED4
#define LED_SOLENOID_RCC		RCC_AHBPeriph_GPIOC
#else
#define LED_SOLENOID_PORT		GPIOC
#define LED_SOLENOID_PIN		GPIO_Pin_12 //LED4
#define LED_SOLENOID_RCC		RCC_AHBPeriph_GPIOC
#endif

#define LED_DOOR_SENSOR_PORT	GPIOA
#define LED_DOOR_SENSOR_PIN		GPIO_Pin_10 //LED7
#define LED_DOOR_SENSOR_RCC		RCC_AHBPeriph_GPIOA

#define LED_VIBRATION_SENSOR_PORT		GPIOA
#define LED_VIBRATION_SENSOR_PIN		GPIO_Pin_9 //LED5
#define LED_VIBRATION_SENSOR_RCC		RCC_AHBPeriph_GPIOA

#define LED_BATTERY_STATUS_PORT		GPIOC
#define LED_BATTERY_STATUS_PIN		GPIO_Pin_7 //LED ?
#define LED_BATTERY_STATUS_RCC		RCC_AHBPeriph_GPIOC

#define LED_SOLENOID2_PORT		GPIOA
#define LED_SOLENOID2_PIN		GPIO_Pin_8 //LED6
#define LED_SOLENOID2_RCC		RCC_AHBPeriph_GPIOA

#define LED_DOOR2_SENSOR_PORT	GPIOC
#define LED_DOOR2_SENSOR_PIN		GPIO_Pin_9
#define LED_DOOR2_SENSOR_RCC		RCC_AHBPeriph_GPIOC

#define LED_VIBRATION2_SENSOR_PORT		GPIOC
#define LED_VIBRATION2_SENSOR_PIN		GPIO_Pin_8
#define LED_VIBRATION2_SENSOR_RCC		RCC_AHBPeriph_GPIOC

//BUZZER
#ifdef ZIM
#define BUZZER_PORT		GPIOC
#define BUZZER_PIN		GPIO_Pin_0 //LED ?
#define BUZZER_RCC		RCC_AHBPeriph_GPIOC
#else
#define BUZZER_PORT				GPIOB
#define BUZZER_PIN 				GPIO_Pin_12
#define BUZZER_RCC				RCC_AHBPeriph_GPIOB
#endif

//BATTERY
#define BATTERY_LEVEL_PORT		GPIOA
#define BATTERY_LEVEL_PIN 		GPIO_Pin_0
#define BATTERY_LEVEL_RCC		RCC_AHBPeriph_ADC12 | RCC_AHBPeriph_GPIOA

#define BATTERY_CHARGE_EN_PORT		GPIOB
#define BATTERY_CHARGE_EN_PIN 		GPIO_Pin_2
#define BATTERY_CHARGE_EN_RCC		RCC_AHBPeriph_GPIOB

//USB
#define         ID1          (0x1FFFF7AC)
#define         ID2          (0x1FFFF7B0)
#define         ID3          (0x1FFFF7B4)

//MONITOR
#define MONITOR_ID0_PIN 	GPIO_Pin_5
#define MONITOR_ID0_PORT	GPIOA
#define MONITOR_ID0_RCC		RCC_AHBPeriph_GPIOA

#define MONITOR_ID1_PIN		GPIO_Pin_7
#define MONITOR_ID1_PORT	GPIOA
#define MONITOR_ID1_RCC		RCC_AHBPeriph_GPIOA

#define MONITOR_ID2_PIN		GPIO_Pin_4
#define MONITOR_ID2_PORT	GPIOF
#define MONITOR_ID2_RCC		RCC_AHBPeriph_GPIOF

#define MONITOR_ID3_PIN 	GPIO_Pin_6
#define MONITOR_ID3_PORT	GPIOA
#define MONITOR_ID3_RCC		RCC_AHBPeriph_GPIOA

#define MONITOR_ID4_PIN 	GPIO_Pin_4
#define MONITOR_ID4_PORT	GPIOC
#define MONITOR_ID4_RCC		RCC_AHBPeriph_GPIOC

#define MONITOR_ID5_PIN 	GPIO_Pin_5
#define MONITOR_ID5_PORT	GPIOC
#define MONITOR_ID5_RCC		RCC_AHBPeriph_GPIOC


#define FASTNET

uint32_t reboot_timer;

#include "FreeRTOS.h"
#include "semphr.h"

xSemaphoreHandle xSemaphore;

#endif
