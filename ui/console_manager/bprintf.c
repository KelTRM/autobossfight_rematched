#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include"buffer.h"

int PutsBuffer(BUFHANDLE b, const char *str, size_t StrLen) {
	if (ValidateBuffer(b) == 0) {
		b = ActiveBuffer;
		if (ValidateBuffer(b) == 0) {
			return 0;
//			ActiveBuffer = CreateBuffer();
//			b = ActiveBuffer;
		}
	}

	if (StrLen == 0)
		StrLen = strlen(str);


	size_t OldBufferSize = Buffers[b].BufferLength;
	
	size_t NewBufferSize = OldBufferSize + StrLen;
	char *Buffer = realloc((void*)Buffers[b].Buffer, NewBufferSize);

	if (Buffer == NULL)
		return 0;

	memcpy(Buffer+OldBufferSize, str, StrLen);

	if (b == ActiveBuffer)
		printf("%.*s", (int)StrLen, str);
	fflush(stdout);

	Buffers[b].BufferLength = NewBufferSize;
//	printf("recieved string of length %zu\n", StrLen);

	return StrLen;
}

int bprintf(BUFHANDLE b, const char *format, ...) {
	va_list args;
	va_start(args, format);

	int n = vsnprintf(NULL, 0, format, args);
	char *str = malloc(n+1);

	if (str == NULL)
		return 0;

	va_end(args);
	va_start(args, format);

	n = vsnprintf(str, n+1, format, args);

	va_end(args);

	PutsBuffer(b, str, n);
	free(str);

	return n;
}
