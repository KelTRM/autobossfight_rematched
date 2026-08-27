#define USE_STD_STRLEN

#include"entity_ldr.h"
#include"../debug/debug.h"
#include<lua.h>
#include<lauxlib.h>
#include<lualib.h>
#include"entity_ldr.h"
#include"table_validation.h"
#include<stddef.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

void LuaAssertType(lua_State *L, int Type, int Expected) {
	if (Type != Expected) {
		lua_pushfstring(L, "Expected type %s. got %s instead",
				lua_typename(L, Expected),
				lua_typename(L, Type));
		lua_error(L);
	}
}

const char *ReadLuaTableString(lua_State *L, const char *Name, char *DefaultValue) {
	assert(lua_type(L, -1) == LUA_TTABLE);
	int Type = lua_getfield(L, -1, Name);
	
	const char *str = DefaultValue;
	if (Type == LUA_TSTRING) {
		str = lua_tostring(L, -1);
	}

	lua_pop(L, 1);

	return str;
}

lua_Number ReadLuaTableNumber(lua_State *L, const char *Name, lua_Number DefaultValue) {
	assert(lua_type(L, -1) == LUA_TTABLE);
	int Type = lua_getfield(L, -1, Name);

	lua_Number n = DefaultValue;
	if (Type == LUA_TNUMBER) {
		n = lua_tonumber(L, -1);
	}

	lua_pop(L, 1);
	return n;
}

int RegisterLuaBosses(lua_State *L, int ArrayIdx, DefMgr_t *Bosses) {
	size_t BossCount = lua_rawlen(L, ArrayIdx);

	write_debug(LuaBossRegistration, "Registering %zu bosses...", BossCount);

	for (size_t i = 1; i <= BossCount; i++) {
		int Type = lua_rawgeti(L, ArrayIdx, i);

		if (Type == LUA_TNIL) continue;

		LuaAssertType(L, Type, LUA_TTABLE);
		VerifyBossDefTable(L);

		const char *Name = ReadLuaTableString(L, "Name", "nil");
		lua_Number HP    = ReadLuaTableNumber(L, "HP", 10000);

		char *NewName = malloc(strlen(Name)+1);
		strncpy(NewName, Name, strlen(Name)+1);

		write_debug(LuaBossRegistration, "Found boss { %s, %d }", NewName, (int)HP);

		struct BossDefinition Boss = { .Name=NewName, .HP=HP };

		AddBoss(Bosses, &Boss);

		lua_pop(L, 1);
	}

	return BossCount;
}

int RegisterLuaPlayers(lua_State *L, int ArrayIdx, DefMgr_t *Players) {
	size_t PlayerCount = lua_rawlen(L, ArrayIdx);

	write_debug(LuaBossRegistration, "Registering %zu players...", PlayerCount);

	for (size_t i = 1; i <= PlayerCount; i++) {
		int Type = lua_rawgeti(L, ArrayIdx, i);

		if (Type == LUA_TNIL) continue;

		LuaAssertType(L, Type, LUA_TTABLE);
		VerifyPlayerDefTable(L);

		const char *Name   = ReadLuaTableString(L, "Name", "nil");
		lua_Number HP      = ReadLuaTableNumber(L, "HP", 10000);
		lua_Number HealMin = ReadLuaTableNumber(L, "HealMin", 2500);
		lua_Number HealMax = ReadLuaTableNumber(L, "HealMax", 7500);
		lua_Number Color   = ReadLuaTableNumber(L, "Color", 0xFFFFFF);

		char *NewName = malloc(strlen(Name)+1);
		strncpy(NewName, Name, strlen(Name)+1);

		write_debug(LuaBossRegistration, "Found player { %s, %d }", NewName, (int)HP);

		struct PlayerDefinition Player = {
			.Name=NewName, .HP=HP,
			.Heal_Min=HealMin, .Heal_Max=HealMax, .Color=Color
		};

		AddPlayer(Players, &Player);

		lua_pop(L, 1);
	}
	
	return PlayerCount;
}
