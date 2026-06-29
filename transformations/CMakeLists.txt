cmake_minimum_required(VERSION 4.0.0)

project(autobossfight_rematched)

add_library(autobossfight_transformations)

target_sources(autobossfight_transformations
	PRIVATE
		default_transformation.c
		transformation_menu.c
		transformation_manager.c
		register_transformations.c

		transformations/transform_base.c
		transformations/transform_boss.c
		transformations/transform_super.c
)

target_include_directories(autobossfight_transformations
	INTERFACE	.
)
