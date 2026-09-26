#include<lua.h>

int AddPluginEntry(lua_State *L) {
	return 0;
}

// takes in upvalues of 2 tables
//
// the table is full of default values for plugins.
// Anything that's nil will take the default value.
// Anything that shares a type with the default will override, and anything else will create an error.
int CreateLuaPlugin(lua_State *L) {
	int idx = lua_upvalueindex(1);
	if (lua_type(L, idx) != LUA_TTABLE) {
		lua_pushliteral(L, "invalid upvalues on CreateLuaPlugin.");
		lua_error(L);
	}

	lua_newtable(L);

	lua_pushvalue(L, idx);
	lua_pushcclosure(L, AddPluginEntry, 1);

	lua_setfield(L, -2, "Add");
	return 1;
}
