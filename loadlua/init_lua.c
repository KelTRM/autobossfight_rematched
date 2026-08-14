#include<stdlib.h>
#include<lua.h>
#include<lauxlib.h>
#include"entity_ldr.h"
#include"lua_bossfight.h"
#include"attacks/lua_attack_manager.h"
#include"../debug/debug.h"
#include"../utils/sleep.h"

const char *LuaInitFile = "lua/init.lua";

struct BossfightLuaState {
	lua_State *L;
	struct {
		DefMgr_t Players;
		DefMgr_t Bosses;
	} Entities;

	// struct incase of future additions
	struct {
		AttackMgr_t mgr;
	} Attacks;
};

void *InitLua(void) {
	struct BossfightLuaState State = { 0 };

	CreateDefMgr(&State.Entities.Players);
	CreateDefMgr(&State.Entities.Bosses);

	lua_State *L = luaL_newstate();

	LoadBossfightTable(L, PERM_ENTITY | PERM_ATTACK);

	int err = luaL_dofile(L, LuaInitFile);

	if (err != LUA_OK) {
		const char *LuaErr = lua_tostring(L, -1);

		write_debug(Lua, "%s", LuaErr);
		printf("lua error in file %s - %s\n", LuaInitFile, LuaErr);
		sleep(1000);
	} else {
		write_debug(Lua, "Exited script %s successfully.", LuaInitFile);
	}

	return L;
}
