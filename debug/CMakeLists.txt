cmake_minimum_required(VERSION 4.0.0)
project(autobossfight_rematched)

add_library(autobossfight_debug)

target_sources(autobossfight_debug
	PRIVATE
		debug.c
)
