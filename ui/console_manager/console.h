#pragma once

#include<stdint.h>

#ifndef PRESERVE_PRINTF
#define printf(...)	bprintf(INVALID_BUFFER_HANDLE, __VA_ARGS__)
#endif

#define INVALID_BUFFER_HANDLE		((BUFHANDLE)-1)

typedef uint64_t BUFHANDLE;

int bprintf(BUFHANDLE b, const char *format, ...);
BUFHANDLE CreateBuffer();
int SwitchBuffer(BUFHANDLE Buffer);
void ClearScreen();
int CopyBuffer(BUFHANDLE From, BUFHANDLE To);
void RefreshScreen();
