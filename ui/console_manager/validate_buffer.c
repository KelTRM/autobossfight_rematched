#include"buffer.h"

int ValidateBuffer(BUFHANDLE b) {
	if (b >= MAXIMUM_BUFFER_COUNT)
		return 0;
	if (Buffers[b].BufferID != b)
		return 0;
	if (Buffers[b].Buffer == NULL)
		return 0;
	return 1;
}
