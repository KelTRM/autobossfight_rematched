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

// init_debug doesn't need parameters, but it looks more natural with the empty param list
#define init_debug()				InitDebugBuffer()
#define write_debug(DEBUG_MODE, format, ...)	bprintf(DebugBuffer, "[%s]" format, #DEBUG_MODE, __VA_ARGS__)
#define define_debug_flush_location(file)
#define flush_debug()

#endif
