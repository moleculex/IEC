#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f30x.h"

typedef struct{
	//char writeBuffer[64];
	//char readBuffer[64];
	char buffer[32];
	union{
		uint32_t address_32bit;
		char address_8bit[4];
	};
}t_flash;

t_flash _flash;

void flash_erase(uint32_t address);
void flash_write(uint32_t address, char *buffer, int length);
void flash_read(uint32_t address, char *buffer, int length);

#endif
