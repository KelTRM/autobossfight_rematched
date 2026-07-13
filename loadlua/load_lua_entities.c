#include"../entity.h"
#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include"../utils/sleep.h"
// lua shit
#include<lua.h>
#include<lauxlib.h>
#include<lualib.h>

#define BOSS_TABLE_DEFINITION	"{ [\"Name\"]=string, [\"HP\"]=number }"

int LuaAddBoss(lua_State *L);

int CheckLuaEntities(void) {
	// check for lua/entities.lua file. This file is required for the game to function.
	FILE *tmp = fopen("lua/entities.lua", "r");
	if (tmp == NULL)
		return -1;

	fclose(tmp);

	return 0;
}

// i have no idea what i'm doing, so it's blank for now
size_t LoadLuaEntities(Entity_t **Entities) {
	// read contents of entities lua file
	FILE *f = fopen("lua/entities.lua", "r");

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

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	struct luaL_Reg fns[] = {
		{
			"AddBoss",
			LuaAddBoss
		},
		{ NULL, NULL }
	};
	luaL_setfuncs(L, fns, 0);

	printf("debug\n");
	//	printf("buffer = %s\nfilesize = %zu\n", Buffer, FileSize);
	int err = luaL_dostring(L, Buffer);

	if (err != LUA_OK) {
		printf("lua error.\n");
	} else {
		printf("lua exited successfully.\n");
	}


	sleep(1000);

	free(Buffer);
	fclose(f);

	(void)Entities;
	return 0;
}

int LuaAddBoss(lua_State *L) {
//	int n = lua_gettop(L);

//	if (n != 1) {
//		lua_pushliteral(L, "expected 1 argument of " BOSS_TABLE_DEFINITION);
//		lua_error(L);
//	}

//	int ParamType = lua_type(L, 1);
//	if (ParamType != LUA_TTABLE) {
//		lua_pushliteral(L, "expected param " BOSS_TABLE_DEFINITION);
//		lua_error(L);
//	}

//	int NameType = lua_getfield(L, 1, "Name");
//	if (NameType != LUA_TSTRING) {
//		lua_pushliteral(L, "Expected [\"Name\"] of type string");
//	}

//	lua_pushnumber(L, 0);

	return 0;
}
