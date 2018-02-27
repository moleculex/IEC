#include "buffer.h"
#include "string.h"

void buffer_init(t_buffer *buffer)
{
	memset(buffer->data, '\0', BUFFER_SIZE);
	buffer->inptr = (char *)buffer->data;
	buffer->outptr = (char *)buffer->data;
	buffer->count = 0;
}

void buffer_push(t_buffer *buffer, char data)
{
	if(buffer->inptr == &buffer->data[BUFFER_SIZE])
		buffer->inptr = (char *)buffer->data;

	*buffer->inptr++ = data;
	buffer->count++;
}

char buffer_pop(t_buffer *buffer)
{
	if(buffer->outptr == &buffer->data[BUFFER_SIZE])
		buffer->outptr = (char *)buffer->data;

	buffer->count--;

	return *buffer->outptr--;
}
