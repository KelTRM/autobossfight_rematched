#include"../debug/debug.h"
#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include"../utils/sleep.h"
#include"lua_registration.h"
#include"table_validation.h"

// lua shit
#include<lua.h>
#include<lauxlib.h>
#include<lualib.h>

int LuaAddBoss(lua_State *L);
int LuaAddPlayer(lua_State *L);
int InitializeEntityManagerTable(lua_State *L, int Index);

char *ReadFileAsStr(const char *Filename) {
	FILE *f = fopen(Filename, "r");

	if (f == NULL)
		return NULL;

	fseek(f, 0, SEEK_END);
	size_t Size = ftell(f);

	char *Buffer = malloc(Size+1);
	if (Buffer == NULL)
		return NULL;

	fseek(f, 0, SEEK_SET);
	fread(Buffer, 1, Size, f);

	Buffer[Size] = 0;

	fclose(f);

	return Buffer;
}

size_t LoadLuaEntities(DefMgr_t *Players, DefMgr_t *Bosses) {
	const char *EntityLua = "lua/entities.lua";

	char *Buffer = ReadFileAsStr(EntityLua);
	if (Buffer == NULL)
		return 0;

	CreateDefMgr(Players);
	CreateDefMgr(Bosses);

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	luaL_Reg fns[] = {
		{ "AddBoss", LuaAddBoss },
		{ "AddPlayer", LuaAddPlayer },
		{ NULL, NULL }
	};
	lua_newtable(L);

	InitializeEntityManagerTable(L, -1);

	luaL_setfuncs(L, fns, 0);
	lua_setglobal(L, "bossfight");

	int err = luaL_dostring(L, Buffer);

	if (err != LUA_OK) {
		const char *LuaErr = lua_tostring(L, -1);

		write_debug(Lua, "%s", LuaErr);
		printf("lua error in file %s - %s\n", EntityLua, LuaErr);
		sleep(1000);
	} else {
		write_debug(Lua, "Exited script %s successfully.", EntityLua);
	}

	lua_getglobal(L, "bossfight");

	lua_getfield(L, -1, "Bosses");
	int Count = RegisterLuaBosses(L, -1, Bosses);
	lua_pop(L, 1);

	lua_getfield(L, -1, "Players");
	Count += RegisterLuaPlayers(L, -1, Players);
	lua_pop(L, 1);

	free(Buffer);

	lua_close(L);

	return Count;
}

int CheckLuaEntities(void) {
	// check for lua/entities.lua file. This file is required for the game to function.
	FILE *tmp = fopen("lua/entities.lua", "r");
	if (tmp == NULL)
		return -1;

	fclose(tmp);

	return 0;
}
