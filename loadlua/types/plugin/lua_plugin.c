#include<stdlib.h>
#include<stdarg.h>
#include<lua.h>
#include<lauxlib.h>
#include"../../../debug/debug.h"

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

[[noreturn]] void TypeError(lua_State *L, int Expected, int Required) {
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

	AssertParameters(L, "plugin:NewEntry", 3,
				LUA_TTABLE, LUA_TSTRING, LUA_TTABLE);
	AssertUpvalues(L, 1, LUA_TTABLE);

	// assume upvalues to be correct, as they should only come from CreateLuaPlugin
	int table = lua_upvalueindex(1);
	lua_newtable(L);

	lua_pushnil(L);
	while (lua_next(L, table) != 0) {
		int KeyType = lua_type(L, -1);

		switch (KeyType) {
//			case LUA_TNIL: {
//				// push the key twice
//				lua_pushvalue(L, -3);
//				lua_pushvalue(L, -4);
//
//				// move the value from the format table into the 
//				lua_gettable(L, table);
//				lua_settable(L, -5);
//			} break;
			case LUA_TTABLE: {
				// template.key
				int RequiredType = lua_getfield(L, -1, "value");

				lua_getfield(L, -2, "has_default");
				int HasDefault = lua_toboolean(L, -1);
				lua_pop(L, 1);

				// entry.key
				lua_pushvalue(L, -3);
				int ActualType = lua_gettable(L, 3);

				lua_pop(L, 1);
				if (HasDefault) {
					if (ActualType != RequiredType) {
						TypeError(L, RequiredType, ActualType);
//						lua_pushfstring(L, "Expected type %s. Recieved %s instead.",
//								lua_typename(L, RequiredType),
//								lua_typename(L, ActualType));
//						lua_error(L);
					}
				} else {
					if (ActualType == LUA_TNIL) {
						lua_pushvalue(L, -2);
					} else if (ActualType == RequiredType) {
						lua_pushvalue(L, -1);
					} else {
						TypeError(L, RequiredType, ActualType);
//						lua_pushfstring(L, "Expected type %s. Recieved type %s.",
//								lua_typename(L, RequiredType),
//								lua_typename(L, ActualType));
					}
				}
			} break;
			default: {
				write_log(Error, "Expected key %s of type table. got %s instead.",
						lua_tostring(L, -3),
						lua_typename(L, lua_type(L, -1)));
				TypeError(L, LUA_TTABLE, lua_type(L, -1));
			}; break;
		}

		lua_pop(L, 1);
	}
	return 0;
}

// takes in upvalues of 2 tables
//
// the table is full of default values for plugins.
// Anything that's nil will take the default value.
// Anything that shares a type with the default will override, and anything else will create an error.
int CreateLuaPlugin(lua_State *L) {
	AssertUpvalues(L, 1, LUA_TTABLE);

	lua_newtable(L);

	lua_pushvalue(L, lua_upvalueindex(1));
	lua_pushcclosure(L, AddPluginEntry, 1);

	lua_setfield(L, -2, "NewEntry");

	lua_newtable(L);
	lua_setfield(L, -2, "entries");
	return 1;
}

// 2 upvalues:
// table - The plugin being registered.
// string - Determines table name for where to store the registered plugin
int RegisterLuaPlugin(lua_State *L) {
	(void)L;
	return 0;
}
