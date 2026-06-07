#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include"buffer.h"

int PutsBuffer(BUFHANDLE b, const char *str, size_t StrLen) {
	if (b == INVALID_BUFFER_HANDLE)
		b = ActiveBuffer;

	if (Buffers[b].BufferID != b || Buffers[b].Buffer == NULL)
		b = ActiveBuffer;

	// can only trigger if ActiveBuffer = INVALID_BUFFER_HANDLE
	if (b == INVALID_BUFFER_HANDLE) {
		ActiveBuffer
	}

	if (StrLen == 0)
		StrLen = strlen(str);


	size_t OldBufferSize = Buffers[b].BufferID;
	
	size_t NewBufferSize = OldBufferSize + StrLen;
	char *Buffer = realloc((void*)Buffers[b].Buffer, NewBufferSize);

	if (Buffer == NULL)
		return 0;

	memcpy(Buffer+OldBufferSize, str, StrLen);

	return StrLen;
}

int bprintf(BUFHANDLE b, const char *format, ...) {
	va_list args;
	va_start(args, format);

	int n = vsnprintf(NULL, 0, format, args);
	char *str = malloc(n);

	if (str == NULL)
		return 0;

	va_end(args);
	va_start(args, format);

	n = vsnprintf(str, n, format, args);

	va_end(args);

	PutsBuffer(b, str, n);
	free(str);

	return n;
}
