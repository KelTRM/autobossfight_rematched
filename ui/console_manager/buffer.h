#pragma once

#define PRESERVE_PRINTF

#include<stddef.h>
#include"console.h"

#define MAXIMUM_BUFFER_COUNT	256

extern BUFHANDLE ActiveBuffer;

// Definition of a console buffer
struct Buffer {
	BUFHANDLE BufferID;	// must be the index in the Buffers array. Differing values indicate an invalid buffer.

	// Buffer content
	size_t BufferLength;	// The length of the buffer
	const char *Buffer;	// The buffer content
};

#define ACTIVE_BUFFER_ARRAY_SIZE	((MAXIMUM_BUFFER_COUNT / 8) + (MAXIMUM_BUFFER_COUNT % 8 != 0))

extern uint8_t ActiveBuffers[ACTIVE_BUFFER_ARRAY_SIZE];
extern struct Buffer Buffers[MAXIMUM_BUFFER_COUNT];

int ValidateBuffer(BUFHANDLE b);
