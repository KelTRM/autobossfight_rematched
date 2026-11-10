#include<stdlib.h>
#include<string.h>
#include<lua.h>
#include<lualib.h>
#include<lauxlib.h>
#include"lua_print.h"
#include"lua_bossfight.h"
#include"../debug/debug.h"
#include"../utils/sleep.h"

const char *LuaInitFile = "lua/init.lua";
void InitRegistry(lua_State *L);

void *InitLua(void) {
	struct BossfightLuaState *State = malloc(sizeof(struct BossfightLuaState));
	if (State == NULL)
		return NULL;

	memset(State, 0, sizeof(*State));

	lua_State *L = luaL_newstate();
	InitRegistry(L);

	LoadBossfightTable(L);
	luaL_openlibs(L);

	LuaSetPrint(L);

	int err = luaL_dofile(L, LuaInitFile);

	if (err != LUA_OK) {
		const char *LuaErr = lua_tostring(L, -1);

		write_log(Lua, "%s", LuaErr);
		printf("%s\n", LuaErr);
		sleep(1000);
	} else {
		write_log(Lua, "Exited script %s successfully.", LuaInitFile);
	}

	State->L = L;

	return State;
}
