#ifndef __BUFFER_H
#define __BUFFER_H

#define BUFFER_SIZE 128

typedef struct{
	char data[BUFFER_SIZE];
	char *inptr;
	char *outptr;
	char count;
}t_buffer;

t_buffer rn2483_rx_buf;
t_buffer rn2483_tx_buf;

void buffer_init(t_buffer *buffer);
void buffer_push(t_buffer *buffer, char data);
char buffer_pop(t_buffer *buffer);

#endif
