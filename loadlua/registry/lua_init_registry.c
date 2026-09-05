#include<lua.h>
#include"../lua_print.h"

void InitRegistry(lua_State *L) {
	// REGISTRY.bossfight
	lua_createtable(L, 0, 2);

	// REGISTRY.bossfight.print
	lua_newtable(L);

	lua_pushinteger(L, LUA_PRINT_BUFFER);
	lua_setfield(L, -2, "buffer");

	lua_pushstring(L, LUA_PRINT_LINE_TERM);
	lua_setfield(L, -2, "print_term");

	lua_setfield(L, -2, "print");
	
	// REGISTRY.bossfight.plugins
	lua_createtable(L, 0, 2);

	// REGISTRY.bossfight.plugins.attack
	lua_newtable(L);
	lua_setfield(L, -2, "attack");

	// REGISTRY.bossfight.plugins.transformation (FOR LATER USE)
	lua_newtable(L);
	lua_setfield(L, -2, "transformation");

	lua_setfield(L, -2, "plugins");

	lua_setfield(L, LUA_REGISTRYINDEX, "bossfight");
}
