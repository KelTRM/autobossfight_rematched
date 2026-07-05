#include<stdio.h>
#include"buffer.h"

void AttachBufferFile(BUFHANDLE Buffer, FILE *Destination, int CloseOnDeletion) {
	int Result = ValidateBuffer(Buffer);
	if (Result == 0)
		return;

	Buffers[Buffer].BufferDestination = Destination;
	Buffers[Buffer].CloseFile = CloseOnDeletion;
}
