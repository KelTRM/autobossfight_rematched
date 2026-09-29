#define LOGFILE_LOCATION		"bossfight.log"
#ifndef NDEBUG

// Default flags for debug build

// Allow transforming round 1
#ifndef TRANSFORM_IMMEDIATE
#define TRANSFORM_IMMEDIATE
#endif

// Enable verbose logging
#ifndef VERBOSE_LOG
#define VERBOSE_LOG			// Enable verbose logging
#endif

#else

// Default flags for release build

#endif
