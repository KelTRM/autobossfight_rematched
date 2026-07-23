#include"entity_ldr.h"
#include"../debug/debug.h"
#include<lua.h>
#include<lauxlib.h>
#include<lualib.h>
#include"entity_ldr.h"
#include"table_validation.h"
#include<stddef.h>

int RegisterLuaBosses(lua_State *L, int ArrayIdx, DefMgr_t *Bosses) {
	size_t BossCount = lua_rawlen(L, ArrayIdx);

	write_debug(LuaBossRegistration, "Registering %zu bosses...", BossCount);

	for (size_t i = 1; i <= BossCount; i++) {
		int type = lua_rawgeti(L, ArrayIdx, i);

		if (type == LUA_TNIL) continue;

		if (type != LUA_TTABLE) {
			lua_pushfstring(L, "Expected type %s. got %s instead", "table", lua_typename(L, type));
			lua_error(L);
		}

		VerifyBossDefTable(L);

		lua_getfield(L, -1, "Name");
		const char *Name = lua_tostring(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, -1, "HP");
		lua_Number HP = lua_tonumber(L, -1);
		lua_pop(L, 1);

		write_debug(LuaBossRegistration, "Found boss { %s, %d }", Name, (int)HP);

		struct BossDefinition Boss = { .Name=Name, .HP=HP };

		AddBoss(Bosses, &Boss);

		lua_pop(L, 1);
	}

	return BossCount;
}

int RegisterLuaPlayers(lua_State *L, int ArrayIdx, DefMgr_t *Players) {
	size_t PlayerCount = lua_rawlen(L, ArrayIdx);

	write_debug(LuaBossRegistration, "Registering %zu players...", PlayerCount);

	for (size_t i = 1; i <= PlayerCount; i++) {
		int type = lua_rawgeti(L, ArrayIdx, i);

		if (type == LUA_TNIL) continue;

		if (type != LUA_TTABLE) {
			lua_pushfstring(L, "Expected type %s. got %s instead", "table", lua_typename(L, type));
			lua_error(L);
		}

		VerifyPlayerDefTable(L);

		lua_getfield(L, -1, "Name");
		const char *Name = lua_tostring(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, -1, "HP");
		lua_Number HP = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, -1, "HealMin");
		lua_Number HealMin = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, -1, "HealMax");
		lua_Number HealMax = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, -1, "Color");
		lua_Number Color = lua_tonumber(L, -1);
		lua_pop(L, 1);

		write_debug(LuaBossRegistration, "Found player { %s, %d }", Name, (int)HP);

		struct PlayerDefinition Player = {
			.Name=Name,
			.HP=HP,
			.Heal_Min=HealMin,
			.Heal_Max=HealMax,
			.Color=Color
		};

		AddPlayer(Players, &Player);

		lua_pop(L, 1);
	}
	
	return PlayerCount;
}
