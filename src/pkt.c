#include "pkt.h"
#include "string.h"
#include "stdlib.h"


char sourceId[16];
char destinationId[16];
char frameId[3];
char hops[3];
char payloadLength[5];
char payload[128];

int packet_getHeaderLength(void)
{
	return 40;
}

void packet_setSourceId(char *val)
{
	memcpy(sourceId, val, 16);
}

char* packet_getSourceId(void)
{
	return sourceId;
}

void packet_setDestinationId(char *val)
{
	memcpy(destinationId, val, 16);
}

char* packet_getDestinationId(void)
{
	return destinationId;
}

void packet_setFrameId(char *val)
{
	memcpy(frameId, val, 2);
}

char* packet_getFrameId(void)
{
	return frameId;
}

void packet_setHops(char *val)
{
	memcpy(hops, val, 2);
}

char* packet_getHops(void)
{
	return hops;
}

void packet_setPayloadLength(char *val)
{
	memcpy(payloadLength, val, 4);
	payloadLength[4] = '\0';
}

char* packet_getPayloadLength(void)
{
	return payloadLength;
}

void packet_setPayload(char *val)
{
	int length;

	payloadLength[4] = '\0';
	length = atoi(payloadLength);
	memcpy(payload, val, length);
}

char* packet_getPayload(void)
{
	return payload;
}

void packet_assemble(char *pkt)
{
	int length;

	payloadLength[4] = '\0';
	length = atoi(payloadLength);

	memcpy(pkt,sourceId, 16);
	memcpy(&pkt[16],destinationId, 16);
	memcpy(&pkt[32],frameId, 2);
	memcpy(&pkt[34],hops, 2);
	memcpy(&pkt[36],payloadLength, 4);
	memcpy(&pkt[40],payload, length);
}

void packet_disassemble(char *pkt)
{
	int length;

	memcpy(sourceId, pkt, 16);
	memcpy(destinationId, &pkt[16], 16);
	memcpy(frameId, &pkt[32], 2);
	frameId[3] = '\0';
	memcpy(hops, &pkt[34], 2);
	hops[3] = '\0';
	memcpy(payloadLength, &pkt[36], 4);
	payloadLength[4] = '\0';
	length = atoi(payloadLength);
	memcpy(payload, &pkt[40], length);
}

void hex_to_hexStr(char *hex, char *ascii, int byte_count)
{
    unsigned char a;
    for(int i = 0; i < byte_count; i++)
    {
        a=*hex;
        a>>=4;
        a&=0x0f;
        if(a<10)
        {
            a|=0x30;
            *ascii=a;
        }
        else
        {
            switch(a)
            {
                case 10:
                    *ascii='A';
                    break;
                case 11:
                    *ascii='B';
                    break;
                case 12:
                    *ascii='C';
                    break;
                case 13:
                    *ascii='D';
                    break;
                case 14:
                    *ascii='E';
                    break;
                case 15:
                    *ascii='F';
                    break;
            }
        }
        *ascii++;

        a=*hex;
        a&=0x0f;
        if(a<10)
        {
            a|=0x30;
            *ascii=a;
        }
        else
        {
            switch(a)
            {
                case 10:
                    *ascii='A';
                    break;
                case 11:
                    *ascii='B';
                    break;
                case 12:
                    *ascii='C';
                    break;
                case 13:
                    *ascii='D';
                    break;
                case 14:
                    *ascii='E';
                    break;
                case 15:
                    *ascii='F';
                    break;
            }
        }
        *ascii++;
        *hex++;
    }
}

void hexStr_to_hex(char *result, char *data, int byte_count)
{
    for(int c = 0; c < byte_count; c++)
    {
        if((*data & 0x30) == 0x30)
        {
            *result = (*data & 0x0F) << 4;
        }
        else
        {
            switch(*data)
            {
                case 'A':
                    *result = 0xA0;
                    break;

                case 'B':
                    *result = 0xB0;
                    break;

                case 'C':
                    *result = 0xC0;
                    break;

                case 'D':
                    *result = 0xD0;
                    break;

                case 'E':
                    *result = 0xE0;
                    break;

                case 'F':
                    *result = 0xF0;
                    break;
            }
        }
        *data++;
        if((*data & 0x30) == 0x30)
        {
            *result |= (*data & 0x0F);
        }
        else
        {
            switch(*data)
            {
                case 'A':
                    *result |= 0x0A;
                    break;

                case 'B':
                    *result |= 0x0B;
                    break;

                case 'C':
                    *result |= 0x0C;
                    break;

                case 'D':
                    *result |= 0x0D;
                    break;

                case 'E':
                    *result |= 0x0E;
                    break;

                case 'F':
                    *result |= 0x0F;
                    break;
            }
        }

        *result++;
        *data++;
    }
}
