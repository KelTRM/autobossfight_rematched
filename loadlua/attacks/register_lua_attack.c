#include <assert.h>
#include <stdio.h>
#define USE_STD_STRLEN

// oh fuck this is gonna be a pain.
// #include"lua_attack.h"
#include"../../attacks/attack_manager.h"
#include"../../attacks/attacks.h"
#include"lua_registration.h"
#include"types/lua_types.h"
#include"../debug/debug.h"
#include<stdlib.h>
#include<string.h>
#include<lua.h>

const char *PluginRegistrationsName = "__bossfight_registered_plugins";

typedef struct LuaAttack {
	lua_State *L;

	const char *AttackTableName;
	const char *AttackTableIndex;
} LuaAttack_t;

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

AttackData_t LuaAttackManager(Attack_t *Self, Entity_t *Target, Entity_t *Attacker) {
	LuaAttack_t *Attack = (LuaAttack_t*)Self->LuaAttackData;

	lua_State *L = Attack->L;

	// get the attack table
	lua_getglobal(L, Attack->AttackTableName);
	
	int type = lua_getfield(L, -1, Attack->AttackTableIndex);
	assert(type == LUA_TTABLE);

	lua_remove(L, -2);

	type = lua_getfield(L, -1, "attack_handler");
	if (type != LUA_TFUNCTION) {
		printf("Not implemented.");
		write_debug(LuaAttackManager, "Attempted to call unimplemented lua function '%s'",
				Attack->AttackTableIndex);

		lua_pop(L, 2);
		return (AttackData_t){ 0 };
	}

	// self arg
	lua_pushvalue(L, -2);

	// attack arg
	lua_newtable(L);

	// attack.target
	CreateEntityTable(L, Target);
	lua_setfield(L, -2, "target");

	// attack.attacker
	CreateEntityTable(L, Attacker);
	lua_setfield(L, -2, "attacker");

	lua_call(L, 2, 1);
	lua_pop(L, 1);

	return (AttackData_t){ 0 };
}

Attack_t ConvertTableToAttack(lua_State *L, int idx) {
	idx = lua_absindex(L, idx);

	// read the table
	AttackID_t ID = ReadLuaTableNumber(L, "id", 0);
	lua_Number FirstRound = ReadLuaTableNumber(L, "first_round", 0);
	lua_Number MinimumEnergy = ReadLuaTableNumber(L, "minimum_energy", -1);

	const char *DisplayName = ReadLuaTableString(L, "disp_name", NULL);
	const char *Identifier = ReadLuaTableString(L, "int_name", NULL);

	// validate the data
	if (DisplayName == NULL) {
		lua_pushstring(L, "invalid attack name.");
		lua_error(L);
	}

	if (Identifier == NULL) {
		lua_pushstring(L, "invalid attack identifier");
		lua_error(L);
	}

	if (MinimumEnergy < 0) {
		lua_pushstring(L, "Invalid value for minimum energy.");
		lua_error(L);
	}

	write_debug(DEBUG_MODE, "Registering attack \"%s\" of { \"%s\", %d, %d, %d }",
			Identifier, DisplayName, FirstRound, MinimumEnergy, ID);

	// define the attack's struct
	Attack_t LuaAttack = { 0 };

	LuaAttack.ID = ID;
	LuaAttack.FirstAvailableRound = FirstRound;
	LuaAttack.MinimumEnergy = MinimumEnergy;

	LuaAttack.Attack = LuaAttackManager;
	LuaAttack.Available = DefaultCanAttack;

	LuaAttack.AppliesToAllies = 0;
	LuaAttack.AppliesToEnemies = 1;
	
	// copy the strings
	size_t IdentifierLength = strlen(Identifier);
	size_t NameLength = strlen(DisplayName);

	LuaAttack.Identifier = malloc(IdentifierLength);
	LuaAttack.AttackName = malloc(NameLength);

	strncpy((char*)LuaAttack.Identifier, Identifier, IdentifierLength);
	strncpy((char*)LuaAttack.AttackName, DisplayName, NameLength);

	// define the lua attack data
	LuaAttack_t LuaAttackData = {
		.L=L,
		.AttackTableName="nil",
//		.AttackTable=idx
	};

	LuaAttack.LuaAttackData = malloc(sizeof(LuaAttackData));
	memcpy(LuaAttack.LuaAttackData, &LuaAttackData, sizeof(LuaAttackData));

	return LuaAttack;
}

int RegisterLuaAttacks(lua_State *L) {
	int args = lua_gettop(L);

	if (args != 2) {
		lua_pushstring(L, "plugin:RegisterLuaAttacks - expected 1 parameter.");
		lua_error(L);
	}

	int type = lua_getglobal(L, PluginRegistrationsName);
	if (type == LUA_TNIL) {
		// define new registered plugin table
		lua_newtable(L);
		lua_setglobal(L, PluginRegistrationsName);
		lua_getglobal(L, PluginRegistrationsName);
	} else if (type != LUA_TTABLE) {
		// can be presumed another instance of the name exists. results in failure
		lua_pushnumber(L, 0);
		return 1;
	}

	lua_getfield(L, 2, "current_entries");

	size_t RegistrationCount = 0;
	size_t RegistrationsProvided = lua_rawlen(L, -1);

	int top = lua_gettop(L);

	for (size_t i = 1; i <= RegistrationsProvided; i++) {
		assert(top == lua_gettop(L));
		// get the registration
		lua_rawgeti(L, -1, i);

		// get the identifier
		const char *Ident = ReadLuaTableString(L, "int_name", NULL);
		if (Ident == NULL)
			continue;

		// this was bad - attack registrar is still uninitialized
//		RegisterAttack(Attack);

		lua_setfield(L, -3, Ident);

		// clear the registration
		lua_pushnil(L);
		lua_rawseti(L, -2, i);
	}
	write_debug(RegisterLuaAttacks, "Writing to index %d", lua_absindex(L, -2));
	PushLuaArray(L, -2);

	lua_pushnumber(L, RegistrationCount);
	return 1;
}
