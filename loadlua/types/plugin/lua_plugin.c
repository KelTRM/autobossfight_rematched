#include<lua.h>
#include<lauxlib.h>

int AddPluginEntry(lua_State *L) {
	int top = lua_gettop(L);
	if (top != 2) {
		lua_pushliteral(L, "Invalid parameters for plugin:NewEntry");
		lua_error(L);
	}

	// assume upvalues to be correct, as they should only come from CreateLuaPlugin
	int table = lua_upvalueindex(1);
	lua_newtable(L);

	lua_pushnil(L);
	while (lua_next(L, table) != 0) {
		const char *Keyname = luaL_checkstring(L, -2);
		int RequiredType = lua_type(L, -1);

		int ActualType = lua_getfield(L, 2, Keyname);
		if (ActualType == LUA_TNIL) {
			lua_getfield(L, table, Keyname);
			lua_setfield(L, -5, Keyname);
		} else if (ActualType == RequiredType) {
			lua_getfield(L, 2, Keyname);
			lua_setfield(L, -5, Keyname);
		} else {
			lua_pushfstring(L, "Unexpected type %s of value %s (expected %s)",
						lua_typename(L, ActualType), Keyname,
						lua_typename(L, RequiredType));
			lua_error(L);
		}

		lua_pop(L, 2);
	}
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

	lua_setfield(L, -2, "NewEntry");

	lua_newtable(L);
	lua_setfield(L, -2, "entries");
	return 1;
}

// 2 upvalues:
// string - Determines table name for where to store the registered plugin
// table - The plugin being registered.
int RegisterPlugin(lua_State *L) {
	
}
