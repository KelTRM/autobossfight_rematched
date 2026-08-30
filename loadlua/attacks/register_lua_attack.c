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

	const char *AttackPluginKey;
	size_t AttackPluginIndex;
} LuaAttack_t;

const char *ReadLuaTableString(lua_State *L, const char *Name, char *DefaultValue);
lua_Number ReadLuaTableNumber(lua_State *L, const char *Name, lua_Number DefaultValue);
AttackData_t ReadAttackDataTable(lua_State *L);

AttackData_t LuaAttackManager(Attack_t *Self, Entity_t *Target, Entity_t *Attacker) {
	LuaAttack_t *Attack = (LuaAttack_t*)Self->LuaAttackData;
	assert(Attack != NULL);

	lua_State *L = Attack->L;
	assert(L != NULL);

	assert(Attack->AttackPluginKey != NULL);

	// get the attack table
	printf("Getting PluginRegistrationsName\n");
	int type = lua_getglobal(L, PluginRegistrationsName);
	assert(type == LUA_TTABLE);

	// Get the plugin
	type = lua_rawgeti(L, -1, Attack->AttackPluginIndex);
	assert(type == LUA_TTABLE);

	lua_getfield(L, -1, Attack->AttackPluginKey);

	lua_remove(L, -2);

	// attack arg
	lua_newtable(L);

	// attack.target
	CreateEntityTable(L, Target);
	lua_setfield(L, -2, "target");

	// attack.attacker
	CreateEntityTable(L, Attacker);
	lua_setfield(L, -2, "attacker");

	type = lua_getfield(L, -2, "attack_handler");
	if (type != LUA_TFUNCTION) {
		printf("Not implemented.");
		write_debug(LuaAttackManager, "Attempted to call unimplemented lua function '%s'",
				Attack->AttackPluginKey);

		lua_pop(L, 3);
		return (AttackData_t){ 0 };
	}

	// self arg
	lua_pushvalue(L, -3);

	lua_pushvalue(L, -3);

	lua_call(L, 2, 1);

	type = lua_type(L, -1);
	write_debug(LuaAttackManager, "type=%s", lua_typename(L, type));
	
	AttackData_t Result = { 0 };
	if (type == LUA_TTABLE) {
		Result = ReadAttackDataTable(L);
	} else {
		Result.Attacker = Attacker;
		Result.Target = Target;

		lua_pop(L, 2);

		return Result;
	}

	lua_pop(L, 2);

	return Result;
}

Attack_t ConvertTableToAttack(lua_State *L, int idx, const char *Key, size_t PluginIdx) {
	idx = lua_absindex(L, idx);

	// read the table
	AttackID_t ID = ReadLuaTableNumber(L, "id", 0);
	lua_Number FirstRound = ReadLuaTableNumber(L, "first_round", 0);
	lua_Number MinimumEnergy = ReadLuaTableNumber(L, "minimum_energy", -1);

	const char *DisplayName = ReadLuaTableString(L, "disp_name", NULL);

	// validate the data
	if (DisplayName == NULL) {
		lua_pushstring(L, "invalid attack name.");
		lua_error(L);
	}

	if (MinimumEnergy < 0) {
		lua_pushstring(L, "Invalid value for minimum energy.");
		lua_error(L);
	}

	write_debug(ConvertTableToAttack, "Registering attack \"%s\" of { %d, %d, %d }",
			DisplayName, (int)FirstRound, (int)MinimumEnergy, ID);

	// define the attack's struct
	Attack_t LuaAttack = { 0 };

	LuaAttack.ID = ID;
	LuaAttack.FirstAvailableRound = FirstRound;
	LuaAttack.MinimumEnergy = MinimumEnergy;

	LuaAttack.Attack = LuaAttackManager;
	LuaAttack.Available = DefaultCanAttack;

	LuaAttack.AppliesToAllies = 0;
	LuaAttack.AppliesToEnemies = 1;
	
	size_t NameLength = strlen(DisplayName);

	LuaAttack.Identifier = NULL;
	LuaAttack.AttackName = malloc(NameLength + 1);

	strncpy((char*)LuaAttack.AttackName, DisplayName, NameLength + 1);

	LuaAttack_t AttackData = {
		.L=L,
		.AttackPluginIndex=PluginIdx,
		.AttackPluginKey=Key
	};

	LuaAttack.LuaAttackData = malloc(sizeof(LuaAttack_t));
	memcpy(LuaAttack.LuaAttackData, &AttackData, sizeof(LuaAttack_t));

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

	lua_newtable(L);
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

		lua_setfield(L, -3, Ident);
		RegistrationCount++;

		// clear the registration
		lua_pushnil(L);
		lua_rawseti(L, -2, i);
	}

	lua_pop(L, 1);

	write_debug(RegisterLuaAttacks, "Writing to index %d", lua_absindex(L, -2));
	PushLuaArray(L, -2);

	lua_pushnumber(L, RegistrationCount);
	return 1;
}
