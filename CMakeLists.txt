cmake_minimum_required(VERSION 4.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(autobossfight_rematched)

if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(
		-Wall -Wextra -Wpedantic
		$<$<BOOL:${ANALYZE}>:-fanalyzer>
	)

endif()

add_executable(autobossfight_rematched)

target_sources(autobossfight_rematched
	PRIVATE
		init.c
		main.c
		rng.c
		attack.c
		register_attacks.c
		energize_entities.c
)

add_subdirectory(string)
add_subdirectory(loadlua)
add_subdirectory(debug)
add_subdirectory(ui)
add_subdirectory(attacks)
add_subdirectory(transformations)
add_subdirectory(registration)
add_subdirectory(entity)

target_link_libraries(autobossfight_lua_loader
	PUBLIC
		autobossfight_registration_manager
		autobossfight_debug
)

target_link_libraries(autobossfight_debug
	PUBLIC
		autobossfight_ui_manager
)

target_link_libraries(autobossfight_transformations
	PUBLIC
		autobossfight_debug
		autobossfight_ui_manager
		autobossfight_registration_manager
		autobossfight_lua_loader
)

target_link_libraries(autobossfight_attack_mgr
	PUBLIC
		autobossfight_debug
		autobossfight_ui_manager
		autobossfight_registration_manager
)

target_link_libraries(autobossfight_rematched
	PUBLIC
		autobossfight_lua_loader
		autobossfight_ui_manager
		autobossfight_attack_mgr
		autobossfight_transformations
		autobossfight_entity_manager
)

target_link_libraries(autobossfight_registration_manager
	PUBLIC
		autobossfight_debug
)

target_link_libraries(autobossfight_ui_manager
	PUBLIC
		autobossfight_debug
		autobossfight_entity_manager
		autobossfight_attack_mgr
)

target_link_libraries(autobossfight_entity_manager
	PUBLIC
		autobossfight_debug
		autobossfight_ui_manager
		autobossfight_lua_loader
		autobossfight_transformations
)

target_link_libraries(autobossfight_string
	PUBLIC
		autobossfight_debug
)
