#include"debug.h"
#include "console_manager/console.h"

BUFHANDLE DebugBuffer = INVALID_BUFFER_HANDLE;

void InitDebugBuffer(void) {
	if (DebugBuffer != INVALID_BUFFER_HANDLE)
		DebugBuffer = CreateBuffer();
}
