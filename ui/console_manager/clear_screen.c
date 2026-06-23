#include"buffer.h"
#include<stdlib.h>

void ClearBuffer(BUFHANDLE Buffer) {
	// should work since it'll stop reading any characters, and should correct the buffer size on next print
	Buffers[Buffer].BufferLength = 0;
	free((void*)Buffers[Buffer].Buffer);
	Buffers[Buffer].Buffer = malloc(1);
}

void ClearScreen(void) {
	ClearBuffer(ActiveBuffer);
	RefreshScreen();
}
