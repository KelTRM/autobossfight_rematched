cmake_minimum_required(VERSION 4.0.0)

project(autobossfight_rematched)

add_library(autobossfight_transformations)

target_sources(autobossfight_transformations
	PRIVATE
		default_transformation.c
		transform_base.c
		transform_boss.c
)

target_include_directories(autobossfight_transformations
	INTERFACE	.
)
