#ifndef __PKT_H
#define __PKT_H

int payloadLen;
char strPayloadLen[5];

int packet_getHeaderLength(void);
void packet_setSourceId(char *val);
char* packet_getSourceId(void);
void packet_setDestinationId(char *val);
char* packet_getDestinationId(void);
void packet_setFrameId(char *val);
char* packet_getFrameId(void);
void packet_setHops(char *val);
char* packet_getHops(void);
void packet_setPayloadLength(char *val);
char* packet_getPayloadLength(void);
void packet_setPayload(char *val);
char* packet_getPayload(void);

void packet_assemble(char *pkt);
void packet_disassemble(char *pkt);

void hex_to_hexStr(char *hex, char *ascii, int byte_count);
void hexStr_to_hex(char *result, char *data, int byte_count);

#endif
