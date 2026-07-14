#include"../debug/debug.h"
#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include"../utils/sleep.h"
// lua shit
#include<lua.h>
#include<lauxlib.h>
#include<lualib.h>
#include"entity_ldr.h"
#include"table_validation.h"

#define BOSS_TABLE_DEFINITION	"{ [\"Name\"]=string, [\"HP\"]=number }"

struct {
	DefMgr_t *Players;
	DefMgr_t *Bosses;
} EntityLoaderData;

int LuaAddBoss(lua_State *L);
int LuaAddPlayer(lua_State *L);
int InitializeEntityManagerTable(lua_State *L, int Index);

int RegisterLuaBosses(lua_State *L, int ArrayIdx, DefMgr_t *Bosses);
int RegisterLuaPlayers(lua_State *L, int ArrayIdx, DefMgr_t *Players);

size_t LoadLuaEntities(DefMgr_t *Players, DefMgr_t *Bosses) {
	const char *EntityLua = "lua/entities.lua";

	// read contents of entities lua file
	FILE *f = fopen(EntityLua, "r");

	// no entities if entities.lua isn't
	if (f == NULL)
		return 0;

	fseek(f, 0, SEEK_END);
	size_t FileSize = ftell(f);

	// buffer contains file contents of entities.lua
	void *Buffer = malloc(FileSize+1);

	fseek(f, 0, SEEK_SET);
	fread(Buffer, 1, FileSize, f);
	*((char*)Buffer + FileSize + 1) = 0;

	CreateDefMgr(Players);
	CreateDefMgr(Bosses);

	EntityLoaderData.Players = Players;
	EntityLoaderData.Bosses = Bosses;

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
	} else {
		write_debug(Lua, "Exited script %s successfully.", EntityLua);
	}

	lua_getglobal(L, "bossfight");

	lua_getfield(L, -1, "Bosses");
	RegisterLuaBosses(L, -1, Bosses);
	lua_pop(L, 1);

	lua_getfield(L, -1, "Players");
	RegisterLuaPlayers(L, -1, Players);
	lua_pop(L, 1);

	free(Buffer);
	fclose(f);

	lua_close(L);

	return 0;
}

int PushLuaArray(lua_State *L, int ArrayIndex);

int LuaAddBoss(lua_State *L) {
	int n = lua_gettop(L);

//	write_debug(LuaAddBoss, "recieved %d args", n);

	if (n != 2) {
		lua_pushliteral(L, "expected 1 argument of " BOSS_TABLE_DEFINITION);
		lua_error(L);
	}

	VerifyBossDefTable(L);

	int Type = lua_getfield(L, -2, "Bosses");
	if (Type != LUA_TTABLE) {
		lua_pushfstring(L, "Expected [\"Bosses\"] of type table");
		lua_error(L);
	}

	lua_pushvalue(L, -2);
	PushLuaArray(L, -2);
	lua_pop(L, 1);	// pop(Bosses)
	
	lua_pushnumber(L, 1);

	return 1;
}

int LuaAddPlayer(lua_State *L) {
	int n = lua_gettop(L);

//	write_debug(LuaAddBoss, "recieved %d args", n);

	if (n != 2) {
		lua_pushliteral(L, "expected 1 argument of " BOSS_TABLE_DEFINITION);
		lua_error(L);
	}

	VerifyPlayerDefTable(L);

	int Type = lua_getfield(L, -2, "Players");
	if (Type != LUA_TTABLE) {
		lua_pushfstring(L, "Expected [\"Players\"] of type table");
		lua_error(L);
	}

	lua_pushvalue(L, -2);
	PushLuaArray(L, -2);
	lua_pop(L, 1);	// pop(Players)

	lua_pushnumber(L, 1);

	return 1;
}

int CheckLuaEntities(void) {
	// check for lua/entities.lua file. This file is required for the game to function.
	FILE *tmp = fopen("lua/entities.lua", "r");
	if (tmp == NULL)
		return -1;

	fclose(tmp);

	return 0;
}
