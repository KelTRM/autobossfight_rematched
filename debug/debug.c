#define PRESERVE_PRINTF

#include"debug.h"
#include "console_manager/console.h"

BUFHANDLE DebugBuffer = INVALID_BUFFER_HANDLE;

void InitDebugBuffer(void) {
	BUFHANDLE Buffer = DebugBuffer;

	if (Buffer == INVALID_BUFFER_HANDLE)
		Buffer = CreateBuffer();

	DebugBuffer = Buffer;
}
