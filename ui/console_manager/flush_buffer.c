#include"buffer.h"

void FlushBuffer(BUFHANDLE Buffer) {
	struct Buffer *Current = &Buffers[Buffer];

	if (Current->CloseFile == 0)
		RefreshAnsiTerminal(Buffer);
	else
		RefreshFileBuffer(Buffer);
}
