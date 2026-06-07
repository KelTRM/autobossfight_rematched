#include"buffer.h"
#include<stdlib.h>

size_t BufferCount = INVALID_BUFFER_HANDLE;

extern BUFHANDLE ActiveBuffer;
uint8_t ActiveBuffers[ACTIVE_BUFFER_ARRAY_SIZE];	// maybe for later
struct Buffer Buffers[MAXIMUM_BUFFER_COUNT] = { 0 };

BUFHANDLE CreateBuffer() {
	BUFHANDLE i;

	for (i = 0; i < MAXIMUM_BUFFER_COUNT; i++) {
		// exit loop if buffer is empty
		if (Buffers[i].BufferID != i || Buffers[i].Buffer == NULL)
			goto found_empty_buffer;
	}

	return INVALID_BUFFER_HANDLE;

found_empty_buffer:;
	struct Buffer *b = &Buffers[i];

	b->BufferID = i;
	b->BufferLength = 0;
	b->Buffer = malloc(0);

	return i;
}
