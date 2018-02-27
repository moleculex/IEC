#ifndef __3PHASE_H
#define __3PHASE_H

#include "stm32f30x.h"

typedef struct{
	char outBuf[32];
	char inBuf[32];
	char phaseBytes[4];
	uint32_t phaseInt;
	char v[3];
}t_3phase;

t_3phase _3phase;

void _3phase_init(void);
void _3phase_manager(void);
void _3phase_startDSP(void);
void _3phase_stopDSP(void);
void _3phase_reset(void);
void _3phase_readStatus(void);
void _3phase_writeVLevel(void);
void _3phase_readVLevel(void);
void _3phase_writeStatus(void);
void _3phase_readV1(void);
void _3phase_readV2(void);
void _3phase_readV3(void);
void _3phase_read_write(char *outBuffer, char *inBuffer, int length);
void _3phase_cs(char state);

#endif
