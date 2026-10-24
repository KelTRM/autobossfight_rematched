#pragma once

#include<lua.h>
#include"../ui/console_manager/console.h"

struct PrintConf {
	BUFHANDLE Buffer;
	const char *PrintLineTerminator;
};

#define LUA_PRINT_BUFFER	INVALID_BUFFER_HANDLE
#define LUA_PRINT_LINE_TERM	"\n"

int LuaPrint(lua_State *L);
void LuaSetPrint(lua_State *L);
struct PrintConf GetPrintConfig(lua_State *L);

