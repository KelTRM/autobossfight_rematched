#pragma once

#include<stdint.h>

#ifndef PRESERVE_PRINTF
#define printf(...)	bprintf(INVALID_BUFFER_HANDLE, __VA_ARGS__)
#endif

#define INVALID_BUFFER_HANDLE		((BUFHANDLE)-1)

typedef uint64_t BUFHANDLE;

extern BUFHANDLE ActiveBuffer;

BUFHANDLE CreateBuffer(void);
int DeleteBuffer(BUFHANDLE b);

int SwitchBuffer(BUFHANDLE Buffer);
int CopyBuffer(BUFHANDLE From, BUFHANDLE To);
void ClearScreen(void);
void RefreshScreen(void);

int bprintf(BUFHANDLE b, const char *format, ...);

