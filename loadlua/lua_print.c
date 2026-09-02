#include<stdio.h>
#include<lua.h>
#include"../ui/console_manager/console.h"

#define LUA_PRINT_BUFFER	INVALID_BUFFER_HANDLE
#define LUA_PRINT_LINE_TERM	"\n"

struct PrintConf {
	BUFHANDLE Buffer;
	const char *PrintLineTerminator;
};

int LuaPrint(lua_State *L);
struct PrintConf GetPrintConfig(lua_State *L);

void LuaSetPrint(lua_State *L) {
	lua_pushcclosure(L, LuaPrint, 0);
	lua_setglobal(L, "print");
}

int LuaPrint(lua_State *L) {
	int top = lua_gettop(L);
	if (top < 1) {
		return 0;
	}

	// get registry options
	struct PrintConf conf = GetPrintConfig(L);

	const char *str = lua_tolstring(L, 1, NULL);
	if (str == NULL) {
		lua_pushfstring(L, "Cannot convert type %s to string",
				lua_typename(L, lua_type(L, 1)));
		lua_error(L);
	}

	bprintf(conf.Buffer, "%s%s", str, conf.PrintLineTerminator);

	return 0;
}

struct PrintConf GetPrintConfig(lua_State *L) {
	int type;
	struct PrintConf conf = {
		.Buffer=LUA_PRINT_BUFFER,
		.PrintLineTerminator=LUA_PRINT_LINE_TERM
	};

	// get registry options
	type = lua_getfield(L, LUA_REGISTRYINDEX, "bossfight");
	if (type != LUA_TTABLE) { lua_pop(L, 1); return conf; }
	
	type = lua_getfield(L, -1, "print");
	if (type != LUA_TTABLE) { lua_pop(L, 2); return conf; }

	type = lua_getfield(L, -1, "buffer");
	if (type == LUA_TNUMBER) conf.Buffer = lua_tonumber(L, -1);
	lua_pop(L, 1);

	type = lua_getfield(L, -1, "print_term");
	if (type == LUA_TSTRING) conf.PrintLineTerminator = lua_tostring(L, -1);
	lua_pop(L, 1);
	
	return conf;
}
