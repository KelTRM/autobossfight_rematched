#include<stdlib.h>
#include<string.h>
#include<lua.h>
#include<lualib.h>
#include<lauxlib.h>
#include"lua_bossfight.h"
#include"../debug/debug.h"
#include"../utils/sleep.h"

const char *LuaInitFile = "lua/init.lua";
void LuaSetPrint(lua_State *L);

void *InitLua(void) {
	struct BossfightLuaState *State = malloc(sizeof(struct BossfightLuaState));
	memset(State, 0, sizeof(*State));

	State->Attacks.mgr = OpenAttackAllocator(0);

//	CreateDefMgr(&State.Entities.Players);
//	CreateDefMgr(&State.Entities.Bosses);

	lua_State *L = luaL_newstate();

	LoadBossfightTable(L, 0);
	luaL_openlibs(L);

	LuaSetPrint(L);

	int err = luaL_dofile(L, LuaInitFile);

	if (err != LUA_OK) {
		const char *LuaErr = lua_tostring(L, -1);

		write_debug(Lua, "%s", LuaErr);
		printf("lua error in file %s - %s\n", LuaInitFile, LuaErr);
		sleep(1000);
	} else {
		write_debug(Lua, "Exited script %s successfully.", LuaInitFile);
	}

	State->L = L;

	return State;
}
