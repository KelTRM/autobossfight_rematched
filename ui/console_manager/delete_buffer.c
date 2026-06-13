#include<stdlib.h>
#include"buffer.h"

int DeleteBuffer(BUFHANDLE b) {
	if (ValidateBuffer(b) == 0)
		return 0;

	free((void*)Buffers[b].Buffer);
	Buffers[b] = (struct Buffer){
		.Buffer = NULL,
		.BufferID = 0,
		.BufferLength = 0
	};

	return 1;
}
