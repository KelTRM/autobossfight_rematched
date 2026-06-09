#include<stdio.h>
#include"buffer.h"

void RefreshScreen() {
	printf("\x1b[H");
	
//	char ch = *Buffers[ActiveBuffer].Buffer;

	for (size_t i = 0; i < Buffers[ActiveBuffer].BufferLength; i++) {
		char ch = Buffers[ActiveBuffer].Buffer[i];
		if (ch == '\n')
			printf("\x1b[0K");
		putchar(ch);
	}

	printf("\x1b[0J");
	fflush(stdout);
}

int SwitchBuffer(BUFHANDLE Buffer) {
	if (ValidateBuffer(Buffer) == 0) return 0;
	ActiveBuffer = Buffer;

	RefreshScreen();
	return 1;
}
