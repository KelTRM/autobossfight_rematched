#include"buffer.h"

size_t BufferCount = 0;

uint8_t ActiveBuffers[ACTIVE_BUFFER_ARRAY_SIZE];	// maybe for later
struct Buffer Buffers[MAXIMUM_BUFFER_COUNT];

BUFHANDLE CreateBuffer() {
	return 0;
}
