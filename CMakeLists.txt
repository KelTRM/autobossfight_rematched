cmake_minimum_required(VERSION 4.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(autobossfight_rematched)

if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(autobossfight_rematched)

target_sources(autobossfight_rematched
	PRIVATE
		init.c
		main.c
		rng.c
		player.c
		boss.c
		attack.c
		entity.c
		register_attacks.c
		energize_entities.c

		entity/entities.c
)

add_subdirectory(ui)
add_subdirectory(attacks)

target_link_libraries(autobossfight_attack_mgr
	PUBLIC
		autobossfight_ui_manager
)

#if (UNIX)
target_link_libraries(autobossfight_rematched
	PUBLIC
		autobossfight_ui_manager
		autobossfight_attack_mgr
)
#endif(UNIX)
