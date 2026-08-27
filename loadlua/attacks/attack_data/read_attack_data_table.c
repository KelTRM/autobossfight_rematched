#include<stdlib.h>
#include<lua.h>
#include <string.h>
#include"../../../attacks/attack.h"
#include"../../types/lua_types.h"

const char *PrevAnnouncerGlobal = "__last_announcer";

const char *ReadLuaTableString(lua_State *L, const char *Name, char *DefaultValue);
lua_Number ReadLuaTableNumber(lua_State *L, const char *Name, lua_Number DefaultValue);

AttackData_t ReadAttackDataTable(lua_State *L) {
	AttackData_t AttackData = { 0 };

	// - Target (target of the given attack) entity
	// - Attacker (attacker of the target) entity
	// - Damage (damage dealt) number
	// - Announcer (method to handle attack message) function
	// - ID (leave nil) number

	int type;

	AttackData.Attack = ReadLuaTableNumber(L, "id", 0);
	AttackData.Damage = ReadLuaTableNumber(L, "damage", 0);

	type = lua_getfield(L, -1, "target");

	if (type == LUA_TTABLE) {
		Entity_t Target = GetEntityFromTable(L);
		AttackData.Target = malloc(sizeof(Entity_t));
		memcpy(AttackData.Target,   &Target,   sizeof(Entity_t));
	}
	
	lua_pop(L, 1);

	type = lua_getfield(L, -1, "attacker");

	if (type == LUA_TTABLE) {
		Entity_t Attacker = GetEntityFromTable(L);
		AttackData.Attacker = malloc(sizeof(Entity_t));
		memcpy(AttackData.Attacker, &Attacker, sizeof(Entity_t));
	}

	lua_pop(L, 1);

	AttackData.Announcer = NULL;

	return AttackData;
}
