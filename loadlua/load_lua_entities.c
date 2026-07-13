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

#define BOSS_TABLE_DEFINITION	"{ [\"Name\"]=string, [\"HP\"]=number }"

struct {
	DefMgr_t *Players;
	DefMgr_t *Bosses;
} EntityLoaderData;

int LuaAddBoss(lua_State *L);

// i have no idea what i'm doing, so it's blank for now
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
		{ NULL, NULL }
	};
	lua_newtable(L);

	luaL_setfuncs(L, fns, 0);
	lua_setglobal(L, "AddBoss");
//	luaL_setfuncs(L, fns, int nup)

	//	printf("buffer = %s\nfilesize = %zu\n", Buffer, FileSize);
	int err = luaL_dostring(L, Buffer);

	if (err != LUA_OK) {
		const char *LuaErr = lua_tostring(L, -1);

		write_debug(Lua, "%s", LuaErr);
	} else {
		write_debug(Lua, "Exited script %s successfully.", EntityLua);
	}


//	sleep(1000);

	free(Buffer);
	fclose(f);

	lua_close(L);

	return 0;
}

int LuaAddBoss(lua_State *L) {
	int n = lua_gettop(L);

//	write_debug(LuaAddBoss, "recieved %d args", n);

	if (n != 1) {
		lua_pushliteral(L, "expected 1 argument of " BOSS_TABLE_DEFINITION);
		lua_error(L);
	}

	int ParamType = lua_type(L, 1);
	if (ParamType != LUA_TTABLE) {
		lua_pushliteral(L, "expected param " BOSS_TABLE_DEFINITION);
		lua_error(L);
	}

	int NameType = lua_getfield(L, 1, "Name");
	if (NameType != LUA_TSTRING) {
		lua_pushliteral(L, "Expected [\"Name\"] of type string");
		lua_error(L);
	}

	const char *Name = lua_tostring(L, -1);
	write_debug(Lua, "Read boss name of %s", Name);

	int HealthType = lua_getfield(L, 1, "HP");
	if (HealthType != LUA_TNUMBER) {
		lua_pushliteral(L, "Expected [\"HP\"] of type number.");
		lua_pushfstring(L, "Expected [\"HP\"] of type number. Recieved %s instead",
						lua_typename(L, HealthType));
		lua_error(L);
	}

	lua_Number HP = lua_tonumber(L, -1);

	write_debug(EntityManager, "Loading boss { %s, %d }", Name, HP);

	struct BossDefinition Boss = {
		.Name=Name,
		.HP=HP
	};

	AddBoss(EntityLoaderData.Bosses, &Boss);

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
