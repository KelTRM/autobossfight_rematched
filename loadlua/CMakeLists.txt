cmake_minimum_required(VERSION 4.0.0)
project(autobossfight_rematched)

find_package(Lua 5.5 REQUIRED)

add_library(autobossfight_lua_loader)

target_sources(autobossfight_lua_loader
	PRIVATE
		entities/load_lua_entities.c
		entity_ldr.c
		entities/init_lua_entity_manager.c
		entities/validate_entity_tables.c
		push_lua_array.c
		entities/lua_entity_registration.c
		entities/register_lua_entities.c
		entities/lua_add_entity.c
		read_file.c
		path_handler/split_path.c
		path_handler/parse_path.c
		entities/define_entity_table.c
		load_bossfight_table.c
)

target_include_directories(autobossfight_lua_loader
	PUBLIC .
)

add_library(Lua::Lua INTERFACE IMPORTED)
set_target_properties(
	Lua::Lua
	PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES	"${LUA_INCLUDE_DIR}"
		INTERFACE_LINK_DIRECTORIES	"${LUA_LIBRARIES}"
)

target_link_libraries(autobossfight_lua_loader
	PRIVATE
		${LUA_LIBRARIES}
)
