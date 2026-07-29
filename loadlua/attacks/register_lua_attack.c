// oh fuck this is gonna be a pain.
#include"lua_attack.h"
#include"../../attacks/attacks.h"
#include<lua.h>

struct LuaAttack_t {
	lua_State *L;

	AttackID_t PluginID;
	AttackID_t AttackID;
};

/*
	// lua_pop(L, 1);
	lua_setfield(L, -2, "attack_handler");

	// push AttackHandler
	type = lua_getfield(L, 3, "AttackHandler");
	if (type != LUA_TFUNCTION) {
		lua_pop(L, 1);
		lua_pushnil(L);
	}

	lua_setfield(L, -2, "can_attack");

	lua_pushnumber(L, ID);
	lua_setfield(L, -2, "id");

	lua_pushnumber(L, EarliestRound);
	lua_setfield(L, -2, "first_round");

	lua_pushnumber(L, RequiredEnergy);
	lua_setfield(L, -2, "minimum_energy");

	lua_pushstring(L, AttackDispName);
	lua_setfield(L, -2, "disp_name");

	lua_pushstring(L, AttackName);
	lua_setfield(L, -2, "int_name");

	lua_getfield(L, 1, "current_entries");
	lua_pushvalue(L, -2);
*/

Attack_t ConvertTableToAttack(lua_State *L, int idx) {
	lua_pushvalue(L, idx);

	return (Attack_t){ 0 };
}

void RegisterLuaAttacks(lua_State *L) {}
