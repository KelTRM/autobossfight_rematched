#pragma once

#include"../ui/ui.h"
#include"flags.h"

#ifdef NDEBUG

static inline int NoDebug(size_t len, ...) {
	return len;
}

// Define all debug macros as empty here

#define init_debug()
#define write_debug(DEBUG_MODE, format, ...)
#define define_debug_flush_location(file)
#define write_log(DEBUG_MODE, ...)	NoDebug(sizeof(#DEBUG_MODE), __VA_ARGS__)
#define write_verbose(DEBUG_MODE, ...)	NoDebug(sizeof(#DEBUG_MODE), __VA_ARGS__)
#define flush_debug()

#else

extern BUFHANDLE DebugBuffer;

void InitDebugBuffer(void);
int DebugWrite(const char *Source, const char *restrict format, ...);

// init_debug doesn't need parameters, but it looks more natural with the empty param list
#define init_debug()				InitDebugBuffer()
#define define_debug_flush_location(file)	AttachBufferFile(DebugBuffer, file, 1)
#define flush_debug()				FlushBuffer(DebugBuffer)



#ifdef VERBOSE_LOG
#define write_verbose(DEBUG_MODE, ...)	DebugWrite(#DEBUG_MODE"<verbose>", __VA_ARGS__)
#else

static inline int NoDebug(size_t len, const char *Mode, ...) {
	(void)Mode;
	return len;
}

// #define write_verbose(DEBUG_MODE, ...)	((int)(sizeof(#DEBUG_MODE)))
#define write_verbose(DEBUG_MODE, ...)		NoDebug(sizeof(#DEBUG_MODE), #DEBUG_MODE, __VA_ARGS__)

#endif

#define write_log(DEBUG_MODE, ...)	DebugWrite(#DEBUG_MODE, __VA_ARGS__)


#endif
