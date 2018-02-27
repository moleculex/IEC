#include "flash.h"
#include "stm32f30x_flash.h"
#include "core_cm4.h"

void flash_erase(uint32_t address)
{
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
	FLASH_ErasePage(address);
	FLASH_Lock();
}

void flash_write(uint32_t address, char *buffer, int length)
{
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);

	for(int n = 0; n < length; n++)
		FLASH_ProgramWord((uint32_t)(address + n*4), (uint32_t)buffer[n]);

	FLASH_Lock();
}

void flash_read(uint32_t address, char *buffer, int length)
{
	for(int n = 0; n < length; n++)
	{
		buffer[n] = (char)(*(__IO uint32_t *)(address + n*4));
	}
}
