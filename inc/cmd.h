#ifndef __CMD_H
#define __CMD_H

#include "cfg.h"

#define CMD_TYPE_SINGLE 0x01
#define CMD_TYPE_DOUBLE_DOOR 0x02

#define CMD_TYPE CMD_TYPE_SINGLE_DOOR

#define CMD_ERASE	0xF0
#define CMD_WRITE	0xF1
#define CMD_READ	0xF2

#define CMD_FLASH_CONFIG 0xA0
#define CMD_OUTPUT_SOLENOID 0xA1
#define CMD_OUTPUT_BUZZER 0xA2
#define CMD_APP_SOLENOID1 0xA3
#define CMD_STATUS 0xA4

#define CMD_APP_SOLENOID2 0xA6

#define	CMD_SRC_MODEM 0x00
#define	CMD_SRC_USB 0x01

/*#define CMD_FLASH_CONFIG_WRITE		0xD1
#define CMD_FLASH_CONFIG_READ		0xD2
#define CMD_STATUS_READ				0xE2*/

#define CMD_BUFFER_SIZE 64

//|erw|id|len|payload|

typedef struct
{
	union
	{
		char buffer[CMD_BUFFER_SIZE];
		struct
		{
			unsigned char preamble[3];
			unsigned char type;
			unsigned char erw;
			unsigned char id;
			char len;
			union
			{
				char payload[CMD_BUFFER_SIZE];
				struct
				{
					char state;
					char reserved[63];
				}__attribute__((packed)) solenoid;
				struct
				{
					char state;
					char reserved[63];
				}__attribute__((packed)) buzzer;
				struct
				{
					t_cfg cfg;
					char reserved[56];
				}__attribute__((packed)) config;
			};
		}__attribute__((packed));
	};
	char src;
}t_cmd;

t_cmd _cmd;

void cmd_process(void);

#endif
