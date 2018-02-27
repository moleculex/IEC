#ifndef __VCP_H
#define __VCP_H

#include "stm32f30x.h"
#include "buffer.h"

typedef struct{
	t_buffer rd_buffer;
	t_buffer wr_buffer;
}t_vcp;

t_vcp _vcp;

void vcp_init(void);
void vcp_manager(void);
void vcp_reset_port(void);
void vcp_write(t_buffer *buffer, char length);
void vcp_read(void);
void USB_LP_CAN1_RX0_IRQHandler(void);
void USBWakeUp_IRQHandler(void);
void Get_SerialNum(void);
void Leave_LowPowerMode(void);


#endif
