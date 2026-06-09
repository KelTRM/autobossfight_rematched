#pragma once

#include<stdint.h>

#define INVALID_BUFFER_HANDLE		((BUFHANDLE)-1)

typedef uint64_t BUFHANDLE;

int bprintf(BUFHANDLE b, const char *format, ...);
BUFHANDLE CreateBuffer();
int SwitchBuffer(BUFHANDLE Buffer);
void ClearScreen();
