#include<stdio.h>
#include"buffer.h"

#ifdef _MSC_VER
// disable setvbuf as it seems to have problems with the parameters on msvc
#define setvbuf(...)		0
#endif

void RefreshAnsiTerminal(BUFHANDLE Buffer);
void RefreshFileBuffer(BUFHANDLE Buffer);

void RefreshScreen(void) {
	FlushBuffer(ActiveBuffer);
//	struct Buffer *Current = &Buffers[ActiveBuffer];
//	if (Current->CloseFile == 0)
//		RefreshAnsiTerminal(ActiveBuffer);
//	else
//		RefreshFileBuffer(ActiveBuffer);
}

void RefreshFileBuffer(BUFHANDLE Buffer) {
	struct Buffer *Current = &Buffers[Buffer];
	FILE *f = freopen(NULL, "w", Current->BufferDestination);
	if (f == NULL)
		return;

	fwrite(Current->Buffer, 1, Current->BufferLength, Current->BufferDestination);
	fflush(Current->BufferDestination);
}

void RefreshAnsiTerminal(BUFHANDLE Buffer) {
	printf("\x1b[H");

	struct Buffer *Current = &Buffers[Buffer];

	// disable automatic flushing
	setvbuf(Current->BufferDestination, NULL, _IOFBF, 0);

	for (size_t i = 0; i < Current->BufferLength; i++) {
		char ch = Current->Buffer[i];
		if (ch == '\n')
			fprintf(Current->BufferDestination, "\x1b[0K");
		fputc(ch, Current->BufferDestination);
	}

	fprintf(Current->BufferDestination, "\x1b[0J");
	fflush(Current->BufferDestination);

	setvbuf(Current->BufferDestination, NULL, _IOLBF, 0);
}

int SwitchBuffer(BUFHANDLE Buffer) {
	if (ValidateBuffer(Buffer) == 0) return 0;
	ActiveBuffer = Buffer;

	RefreshScreen();
	return 1;
}
