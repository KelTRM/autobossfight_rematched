#include"../debug/debug.h"
#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include"../utils/sleep.h"
#include"lua_registration.h"
#include"lua_file_mgr.h"
#include"lua_entities.h"

// lua shit
#include<lua.h>
#include<lauxlib.h>
#include<lualib.h>

size_t LoadLuaEntities(DefMgr_t *Players, DefMgr_t *Bosses) {
	const char *EntityLua = "lua/entities.lua";

	char *Buffer = ReadFileAsStr(EntityLua);
	if (Buffer == NULL)
		return 0;

	CreateDefMgr(Players);
	CreateDefMgr(Bosses);

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	DefineEntityTable(L);

	int err = luaL_dostring(L, Buffer);

	if (err != LUA_OK) {
		const char *LuaErr = lua_tostring(L, -1);

		write_debug(Lua, "%s", LuaErr);
		printf("lua error in file %s - %s\n", EntityLua, LuaErr);
		sleep(1000);
	} else {
		write_debug(Lua, "Exited script %s successfully.", EntityLua);
	}

//	lua_getglobal(L, "bossfight");
//	lua_getfield(L, -1, "entity");

//	lua_getfield(L, -1, "Bosses");
//	int Count = RegisterLuaBosses(L, -1, Bosses);
//	lua_pop(L, 1);

//	lua_getfield(L, -1, "Players");
//	Count += RegisterLuaPlayers(L, -1, Players);
//	lua_pop(L, 1);

//	lua_pop(L, 2);

	int Count = RegisterLuaEntities(L, Players, Bosses);

	free(Buffer);

	lua_close(L);

	return Count;
}

