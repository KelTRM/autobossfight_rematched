#include<stdlib.h>
#include<string.h>
#include"buffer.h"

int CopyBuffer(BUFHANDLE From, BUFHANDLE To) {
	if (ValidateBuffer(From) == 0) From = ActiveBuffer;
	if (ValidateBuffer(To) == 0) To = ActiveBuffer;

	if (From == To)
		return 1;

	// source buffer to be copied
	const char *SourceText = Buffers[From].Buffer;

	// delete old buffer
	free((void*)Buffers[To].Buffer);

	// copy the buffer
	Buffers[To].Buffer = malloc(Buffers[From].BufferLength);
	memcpy((void*)Buffers[To].Buffer, SourceText, Buffers[From].BufferLength);
	Buffers[To].BufferLength = Buffers[From].BufferLength;

	return 1;
}
