#include<stdlib.h>
#include<stdarg.h>
#include<assert.h>
#include<lua.h>
#include<lauxlib.h>
#include"../../../debug/debug.h"
#include"lua_plugin.h"

// takes table from idx, returns if there's a default
// if DefaultType isn't NULL, sets it to the type of the value, and pushes the value
int ValueHasDefault(lua_State *L, int idx, int *RequiredType) {
	lua_getfield(L, idx, "has_default");
	int res = lua_toboolean(L, -1);

	lua_pop(L, 1);

	if (RequiredType != NULL) {
		lua_getfield(L, idx, "type");
		*RequiredType = lua_tointeger(L, -1);
		lua_pop(L, 1);
	}

	lua_getfield(L, idx, "value");
	
	return res;
}

void AssertUpvalues(lua_State *L, size_t Count, ...) {
	va_list args;
	va_start(args, Count);

	for (size_t i = 1; i <= Count; i++) {
		int Expected = va_arg(args, int);
		int Recieved = lua_type(L, lua_upvalueindex(i));
		if (Expected != Recieved) {
			write_log(Fatal, "Malformed upvalues passed at idx %d.", i);
			write_log(Info, "Malformed upvalue expected type %s. But recieved type %s.",
					lua_typename(L, Expected), lua_typename(L, Recieved));
			exit(-1);
		}
	}
}

sagn_noreturn void TypeError(lua_State *L, int Expected, int Required) {
	lua_pushfstring(L, "Expected type %s. Got %s instead.",
				lua_typename(L, Expected),
				lua_typename(L, Required));
	lua_error(L);

	// should never be reached. it's for the [[noreturn]] warning
	while (1) {}
}

void AssertParameters(lua_State *L, const char *Prototype, int ParamCount, ...);

int AddPluginEntry(lua_State *L) {
	int top = lua_gettop(L);

	int ValCount = 0;

	AssertParameters(L, "plugin:NewEntry", 3,
				LUA_TTABLE, LUA_TSTRING, LUA_TTABLE);
	AssertUpvalues(L, 1, LUA_TTABLE);

	lua_getfield(L, 1, "entries");

	int table = lua_upvalueindex(1);
	lua_newtable(L);


	lua_pushnil(L);
	while (lua_next(L, table) != 0) {
		int KeyType = lua_type(L, -1);

//		write_verbose(Debug, "Checking entry of key=%s",
//				lua_tostring(L, -2));

		if (KeyType != LUA_TTABLE) {
			write_log(Error, "Expected key %s of type table. got %s instead.",
					lua_tostring(L, -3),
					lua_typename(L, lua_type(L, -1)));
			TypeError(L, LUA_TTABLE, lua_type(L, -1));
		}

		int RequiredType;
		int HasDefault = ValueHasDefault(L, -1, &RequiredType);
	
		// entry.key
		lua_pushvalue(L, -3);
		int ActualType = lua_gettable(L, 3);

		if (!HasDefault) {
			if (ActualType != RequiredType) {
				TypeError(L, RequiredType, ActualType);
			}
		}

		int type = lua_getfield(L, -3, "name");
		if (type != LUA_TSTRING) TypeError(L, LUA_TSTRING, type);
//		lua_pushvalue(L, -4);
		
		if (ActualType == LUA_TNIL) {
			lua_pushvalue(L, -3);
		} else if (ActualType == RequiredType) {
			lua_pushvalue(L, -2);
		} else {
			TypeError(L, RequiredType, ActualType);
		}

//		write_verbose(Debug, "Copying key of %s to table with type %s",
//			lua_tostring(L, -2), lua_typename(L, lua_type(L, -1)));
		lua_settable(L, -7);
		ValCount++;

		lua_pop(L, 3);
	}

//	write_verbose(Debug, "Assigned %d values to entry '%s'",
//				ValCount, lua_tostring(L, 2));

	lua_pushvalue(L, 2);
	lua_setfield(L, -2, "int_name");

	lua_rawseti(L, -2, 1);
	lua_pop(L, 1);

	assert(top == lua_gettop(L));
	return 0;
}

// takes in upvalues of 2 tables
//
// the table is full of default values for plugins.
// Anything that's nil will take the default value.
// Anything that shares a type with the default will override, and anything else will create an error.
int CreateLuaPlugin(lua_State *L) {
	int top = lua_gettop(L);
	AssertUpvalues(L, 1, LUA_TTABLE);

	lua_newtable(L);

	lua_pushvalue(L, lua_upvalueindex(1));
	lua_pushcclosure(L, AddPluginEntry, 1);

	lua_setfield(L, -2, "NewEntry");

	lua_newtable(L);
	lua_setfield(L, -2, "entries");

	assert(top+1 == lua_gettop(L));
	return 1;
}

// 2 upvalues:
// table - The plugin being registered.
// string - Determines table name for where to store the registered plugin
int RegisterLuaPlugin(lua_State *L) {
	(void)L;
	return 0;
}
