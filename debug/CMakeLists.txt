cmake_minimum_required(VERSION 4.0.0)
project(autobossfight_rematched)

add_library(autobossfight_debug)

option(LOG_VERBOSE "Give a more verbose output in the log file" OFF)
option(IMMEDIATE_TRANSFORM "Allows transforming on round 1" OFF)
option(LOG_LINE "Shows file:line on log" OFF)

target_sources(autobossfight_debug
	PRIVATE
		debug.c
)

target_compile_definitions(autobossfight_debug
	PUBLIC
		$<$<BOOL:${LOG_VERBOSE}>:VERBOSE_LOG>
		$<$<BOOL:${IMMEDIATE_TRANSFORM}>:TRANSFORM_IMMEDIATE>
		$<$<BOOL:${LOG_LINE}>:LOG_LINE_FILE>
)
