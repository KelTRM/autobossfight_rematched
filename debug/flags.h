#define LOGFILE_LOCATION		"bossfight.log"
#ifndef NDEBUG

// Default flags for debug build

// #define DISABLE_LUA_ATTACKS		// Disables all lua attacks
#define TRANSFORM_IMMEDIATE		// Allow transforming round 1
#define VERBOSE_LOG			// Enable verbose logging

#else

// Default flags for release build

#endif
