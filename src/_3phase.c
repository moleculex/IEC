#include "_3phase.h"
#include "platform_config.h"
#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_spi.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void _3phase_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the SPI periph */
	RCC_APB1PeriphClockCmd(SPIx_CLK, ENABLE);

	/* Enable SCK, MOSI, MISO and CS GPIO clocks */
	RCC_AHBPeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK | SPIx_CS_GPIO_CLK, ENABLE);

	GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
	GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);
	GPIO_PinAFConfig(SPIx_MISO_GPIO_PORT, SPIx_MISO_SOURCE, SPIx_MISO_AF);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/* SPI SCK pin configuration */
	GPIO_InitStructure.GPIO_Pin = SPIx_SCK_PIN;
	GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);

	/* SPI  MOSI pin configuration */
	GPIO_InitStructure.GPIO_Pin =  SPIx_MOSI_PIN;
	GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);

	/* SPI MISO pin configuration */
	GPIO_InitStructure.GPIO_Pin = SPIx_MISO_PIN;
	GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = SPIx_CS_PIN;
	GPIO_Init(SPIx_CS_GPIO_PORT, &GPIO_InitStructure);

	/* SPI configuration -------------------------------------------------------*/
	SPI_I2S_DeInit(SPIx);
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPIx, &SPI_InitStructure);

	SPI_RxFIFOThresholdConfig(SPIx, SPI_RxFIFOThreshold_QF);

	/* Enable the SPI peripheral */
	SPI_Cmd(SPIx, ENABLE);
}

void _3phase_manager(void)
{
	_3phase_init();

	vTaskDelay(100);

	for(int i = 0; i < 3; i++)
	{
		_3phase_cs(1);
		vTaskDelay(1);
		_3phase_cs(0);
		vTaskDelay(1);
	}

	vTaskDelay(100);
	_3phase_reset();
	vTaskDelay(100);

	for(;;)
	{
		_3phase_startDSP();
		vTaskDelay(250);
		_3phase_readV1();
		_3phase_readV2();
		_3phase_readV3();
		vTaskDelay(100);
		_3phase_stopDSP();
		vTaskDelay(5000);
	}
}

void _3phase_readStatus(void)
{
	_3phase.outBuf[0] = 0x01;
	_3phase.outBuf[1] = 0xE5;
	_3phase.outBuf[2] = 0x03;
	_3phase.outBuf[3] = 0x00;
	_3phase.outBuf[4] = 0x00;
	_3phase.outBuf[5] = 0x00;
	_3phase.outBuf[6] = 0x00;

	_3phase_cs(0);
	_3phase_read_write(_3phase.outBuf, _3phase.inBuf, 7);
	_3phase_cs(1);
}

void _3phase_writeVLevel(void)
{
	_3phase.outBuf[0] = 0x00;
	_3phase.outBuf[1] = 0x43;
	_3phase.outBuf[2] = 0xA2;
	_3phase.outBuf[3] = 0x58;
	_3phase.outBuf[4] = 0x60;
	_3phase.outBuf[5] = 0x4A;

	_3phase_cs(0);
	_3phase_read_write(_3phase.outBuf, _3phase.inBuf, 6);
	_3phase_cs(1);
}

void _3phase_readVLevel(void)
{
	_3phase.outBuf[0] = 0x01;
	_3phase.outBuf[1] = 0x43;
	_3phase.outBuf[2] = 0xA2;
	_3phase.outBuf[3] = 0x00;
	_3phase.outBuf[4] = 0x00;
	_3phase.outBuf[5] = 0x00;

	_3phase_cs(0);
	_3phase_read_write(_3phase.outBuf, _3phase.inBuf, 6);
	_3phase_cs(1);
}

void _3phase_startDSP(void)
{
	_3phase.outBuf[0] = 0x00;
	_3phase.outBuf[1] = 0xE2;
	_3phase.outBuf[2] = 0x28;
	_3phase.outBuf[3] = 0x00;
	_3phase.outBuf[4] = 0x01;

	_3phase_cs(0);
	_3phase_read_write(_3phase.outBuf, _3phase.inBuf, 5);
	_3phase_cs(1);
}

void _3phase_stopDSP(void)
{
	_3phase.outBuf[0] = 0x00;
	_3phase.outBuf[1] = 0xE2;
	_3phase.outBuf[2] = 0x28;
	_3phase.outBuf[3] = 0x00;
	_3phase.outBuf[4] = 0x00;

	_3phase_cs(0);
	_3phase_read_write(_3phase.outBuf, _3phase.inBuf, 5);
	_3phase_cs(1);
}

void _3phase_reset(void)
{
	_3phase.outBuf[0] = 0x00;
	_3phase.outBuf[1] = 0xE6;
	_3phase.outBuf[2] = 0x18;
	_3phase.outBuf[3] = 0x00;
	_3phase.outBuf[4] = 0x80;

	_3phase_cs(0);
	_3phase_read_write(_3phase.outBuf, _3phase.inBuf, 5);
	_3phase_cs(1);
}

void _3phase_writeStatus(void)
{
	_3phase.outBuf[0] = 0x00;
	_3phase.outBuf[1] = 0xE5;
	_3phase.outBuf[2] = 0x03;
	_3phase.outBuf[3] = 0x00;
	_3phase.outBuf[4] = 0x01;
	_3phase.outBuf[5] = 0xFF;
	_3phase.outBuf[6] = 0xFF;

	_3phase_cs(0);
	_3phase_read_write(_3phase.outBuf, _3phase.inBuf, 7);
	_3phase_cs(1);
}

void _3phase_readV1(void)
{
	//AIRMS
	uint32_t *tmp;

	_3phase.outBuf[0] = 0x01; //read
	_3phase.outBuf[1] = 0x43;
	_3phase.outBuf[2] = 0xC0;
	_3phase.outBuf[3] = 0x00;
	_3phase.outBuf[4] = 0x00;
	_3phase.outBuf[5] = 0x00;
	_3phase.outBuf[6] = 0x00;
	_3phase_cs(0);
	_3phase_read_write(_3phase.outBuf, _3phase.inBuf, 7);
	_3phase_cs(1);

	_3phase.phaseBytes[3] = _3phase.inBuf[3];
	_3phase.phaseBytes[2] = _3phase.inBuf[4];
	_3phase.phaseBytes[1] = _3phase.inBuf[5];
	_3phase.phaseBytes[0] = _3phase.inBuf[6];

	tmp = (uint32_t *) _3phase.phaseBytes;
	_3phase.phaseInt = *tmp / 20000;
	_3phase.v[1] = (char) _3phase.phaseInt;
}

//10000
void _3phase_readV2(void)
{
	//AVRMS
	uint32_t *tmp;

	_3phase.outBuf[0] = 0x01; //read
	_3phase.outBuf[1] = 0x43;
	_3phase.outBuf[2] = 0xC1;
	_3phase.outBuf[3] = 0x00;
	_3phase.outBuf[4] = 0x00;
	_3phase.outBuf[5] = 0x00;
	_3phase.outBuf[6] = 0x00;
	_3phase_cs(0);
	_3phase_read_write(_3phase.outBuf, _3phase.inBuf, 7);
	_3phase_cs(1);

	_3phase.phaseBytes[3] = _3phase.inBuf[3];
	_3phase.phaseBytes[2] = _3phase.inBuf[4];
	_3phase.phaseBytes[1] = _3phase.inBuf[5];
	_3phase.phaseBytes[0] = _3phase.inBuf[6];

	tmp = (uint32_t *) _3phase.phaseBytes;
	_3phase.phaseInt = *tmp / 20000;
	_3phase.v[0] = (char) _3phase.phaseInt;

}

void _3phase_readV3(void)
{
	//AV2RMS
	uint32_t *tmp;

	_3phase.outBuf[0] = 0x01; //read
	_3phase.outBuf[1] = 0x43;
	_3phase.outBuf[2] = 0xC2;
	_3phase.outBuf[3] = 0x00;
	_3phase.outBuf[4] = 0x00;
	_3phase.outBuf[5] = 0x00;
	_3phase.outBuf[6] = 0x00;
	_3phase_cs(0);
	_3phase_read_write(_3phase.outBuf, _3phase.inBuf, 7);
	_3phase_cs(1);

	_3phase.phaseBytes[3] = _3phase.inBuf[3];
	_3phase.phaseBytes[2] = _3phase.inBuf[4];
	_3phase.phaseBytes[1] = _3phase.inBuf[5];
	_3phase.phaseBytes[0] = _3phase.inBuf[6];

	tmp = (uint32_t *) _3phase.phaseBytes;
	_3phase.phaseInt = *tmp / 20000;
	_3phase.v[2] = (char) _3phase.phaseInt;
}

void _3phase_read_write(char *outBuffer, char *inBuffer, int length)
{
	for(int i = 0; i < length; i++)
	{
		while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
		SPI_SendData8(SPIx, outBuffer[i]);
		while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
		inBuffer[i] = SPI_ReceiveData8(SPIx) ;
	}
}

void _3phase_cs(char state)
{
	if(state == 1)
		GPIO_SetBits(SPIx_CS_GPIO_PORT, SPIx_CS_PIN);
	else
		GPIO_ResetBits(SPIx_CS_GPIO_PORT, SPIx_CS_PIN);
}
