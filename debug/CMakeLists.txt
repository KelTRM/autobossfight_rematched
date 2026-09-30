cmake_minimum_required(VERSION 4.0.0)
project(autobossfight_rematched)

add_library(autobossfight_debug)

target_sources(autobossfight_debug
	PRIVATE
		debug.c
)

target_compile_definitions(autobossfight_debug
	PUBLIC
		$<$<BOOL:${VERBOSE}>:VERBOSE_LOG>
		$<$<BOOL:${IMMEDIATE_TRANSFORM}>:TRANSFORM_IMMEDIATE>
		$<$<BOOL:${LOG_LINE}>:LOG_LINE_FILE>
)
