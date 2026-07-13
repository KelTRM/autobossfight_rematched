cmake_minimum_required(VERSION 4.0.0)
project(autobossfight_rematched)

find_package(Lua 5.5 REQUIRED)

add_library(autobossfight_lua_loader)

target_sources(autobossfight_lua_loader
	PRIVATE
		load_lua_entities.c
		entity_ldr.c
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
