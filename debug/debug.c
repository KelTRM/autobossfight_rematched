#include <stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include"debug.h"
#include "console_manager/console.h"

BUFHANDLE DebugBuffer = INVALID_BUFFER_HANDLE;

void InitDebugBuffer(void) {
	BUFHANDLE Buffer = DebugBuffer;

	if (Buffer == INVALID_BUFFER_HANDLE)
		Buffer = CreateBuffer();

	DebugBuffer = Buffer;
}

#define FORMAT_FMT	"[ %s ] %s\n"

int DebugWrite(const char *Source, const char *restrict format, ...) {
	// generate the format
	size_t FormatLen = snprintf(NULL, 0,
			FORMAT_FMT, Source, format);

	char *FormatBuffer = malloc(FormatLen+1);
	FormatLen = snprintf(FormatBuffer, FormatLen+1,
			FORMAT_FMT, Source, format);

	va_list args;
	va_start(args, format);
	int DebugLen = vsnprintf(NULL, 0, FormatBuffer, args);

	if (DebugLen < 0) {
		free(FormatBuffer);
		return 0;
	}

	va_end(args);
	va_start(args, format);

//	printf("formatbuffer = %s", FormatBuffer);

	char *DebugTextBuffer = malloc(DebugLen+1);
	DebugLen = vsnprintf(DebugTextBuffer, DebugLen+1, FormatBuffer, args);

//	printf("DebugTextBuffer = %s", DebugTextBuffer);

	va_end(args);

	PutsBuffer(DebugBuffer, DebugTextBuffer, DebugLen);

	free(DebugTextBuffer);
	free(FormatBuffer);

//	FlushBuffer(DebugBuffer);

	return DebugLen;
}

#undef FORMAT_FMT
