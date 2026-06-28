cmake_minimum_required(VERSION 4.0.0)

project(autobossfight_rematched)

add_library(autobossfight_registration_manager)

target_sources(autobossfight_registration_manager
	PRIVATE
		registration.c	
#		linked_list/list.c
)

target_include_directories(autobossfight_registration_manager
	INTERFACE .
)
