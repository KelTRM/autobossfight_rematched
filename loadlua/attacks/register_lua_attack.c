#define USE_STD_STRLEN

// oh fuck this is gonna be a pain.
#include"lua_attack.h"
#include"../../attacks/attacks.h"
#include"../debug/debug.h"
#include<stdlib.h>
#include<string.h>
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

const char *ReadLuaTableString(lua_State *L, const char *Name, char *DefaultValue);
lua_Number ReadLuaTableNumber(lua_State *L, const char *Name, lua_Number DefaultValue);

Attack_t ConvertTableToAttack(lua_State *L, int idx) {
	idx = lua_absindex(L, idx);

//	if (!lua_istable(L, idx)) {
//		lua_pushstring(L, "recieved non-table as attack.");
//		lua_error(L);
//	}

//	Attack_t Attack = { 0 };

//	int type = lua_getfield(L, idx, "id");
//	if (type != LUA_TNUMBER) {
//		lua_pushstring(L, "recieved field id of type other than number");
//		lua_error(L);
//	}
//	Attack.ID = lua_tonumber(L, -1);

//	type = lua_getfield(L, idx, "first_round");

	AttackID_t ID = ReadLuaTableNumber(L, "id", 0);
	lua_Number FirstRound = ReadLuaTableNumber(L, "first_round", 0);
	lua_Number MinimumEnergy = ReadLuaTableNumber(L, "minimum_energy", -1);

	const char *DisplayName = ReadLuaTableString(L, "disp_name", NULL);
	const char *Identifier = ReadLuaTableString(L, "int_name", NULL);

	if (DisplayName == NULL) {
		lua_pushstring(L, "invalid attack name.");
		lua_error(L);
	}

	if (Identifier == NULL) {
		lua_pushstring(L, "invalid attack identifier");
		lua_error(L);
	}

	write_debug(DEBUG_MODE, "Registering attack \"%s\" of { \"%s\", %d, %d, %d }");

	Attack_t LuaAttack = { 0 };

	LuaAttack.ID = ID;
	LuaAttack.FirstAvailableRound = FirstRound;
	LuaAttack.MinimumEnergy = MinimumEnergy;
	
	LuaAttack.Identifier = malloc(strlen(Identifier));
	LuaAttack.AttackName = malloc(strlen(DisplayName));

	return (Attack_t){ 0 };
}

void RegisterLuaAttacks(lua_State *L) {}
