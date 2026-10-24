#pragma once

#define LOGFILE_LOCATION		"bossfight.log"
#ifndef NDEBUG

// Default flags for debug build

// Allow transforming round 1
#ifndef TRANSFORM_IMMEDIATE
#define TRANSFORM_IMMEDIATE			// Enables transformations on round 1
#endif

// Enable verbose logging
//#ifndef VERBOSE_LOG
//#define VERBOSE_LOG				// Enable verbose logging
//#endif

// Show file:line numbers in log
#ifndef LOG_LINE_FILE
#define LOG_LINE_FILE
#endif

#else

// Default flags for release build

#endif

