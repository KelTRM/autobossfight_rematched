#pragma once

#include"../ui/ui.h"

#ifdef NDEBUG

// Define all debug macros as empty here

#define init_debug()
#define write_debug(DEBUG_MODE, format, ...)
#define define_debug_flush_location(file)

#else

extern BUFHANDLE DebugBuffer;

void InitDebugBuffer(void);
void DebugWrite(const char *Source, const char *restrict format, ...);

// init_debug doesn't need parameters, but it looks more natural with the empty param list
#define init_debug()				InitDebugBuffer()
#define define_debug_flush_location(file)	AttachBufferFile(DebugBuffer, file, 1)
#define flush_debug()				FlushBuffer(DebugBuffer)

#define write_debug(DEBUG_MODE, ...)	DebugWrite(#DEBUG_MODE, __VA_ARGS__)
// #define write_debug(DEBUG_MODE, format, ...)	bprintf(DebugBuffer, "[%s] " format "\n", #DEBUG_MODE, __VA_ARGS__)

#endif
