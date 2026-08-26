#include<lua.h>
#include"../../../attacks/attack.h"

const char *PrevAnnouncerGlobal = "__last_announcer";

const char *ReadLuaTableString(lua_State *L, const char *Name, char *DefaultValue);
lua_Number ReadLuaTableNumber(lua_State *L, const char *Name, lua_Number DefaultValue);

AttackData_t ReadAttackDataTable(lua_State *L) {
	AttackData_t AttackData;

	// - Target (target of the given attack) entity
	// - Attacker (attacker of the target) entity
	// - Damage (damage dealt) number
	// - Announcer (method to handle attack message) function
	// - ID (leave nil) number

	AttackData.Attack = ReadLuaTableNumber(L, "id", 0);
	AttackData.Damage = ReadLuaTableNumber(L, "damage", 0);

}
