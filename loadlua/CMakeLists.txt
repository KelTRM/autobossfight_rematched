cmake_minimum_required(VERSION 4.0.0)
project(autobossfight_rematched)

find_package(Lua 5.5 REQUIRED)

add_library(autobossfight_lua_loader)

target_sources(autobossfight_lua_loader
	PRIVATE
		load_lua_entities.c
)

target_link_libraries(autobossfight_lua_loader
	PRIVATE
		Lua
)
