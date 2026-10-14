#include <assert.h>
#include<lua.h>
#include<lauxlib.h>
#include"lua_attack.h"

int RegisterLuaAttacks(lua_State *L);
int DefineAttackDataTable(lua_State *L);

/*
 * -- ATTACK TABLE --
{
	AttackName="example",   -- The name of the attack. This is exclusively for the display name
	RequiredEnergy=0,       -- The amount of energy the attack consumes
	EarliestRound=0,        -- The first round in which the attack can be done (nil=0)
	CanAttack=function(proposed_attack) return true end, -- Tells you whether the attack can be used. use nil for default
	AttackHandler=function(attack) return { } end,       -- Run on attack. Determines the outcome of the attack
	id=nil                  -- The ID used to specify the attack. use nil to auto-select
}
*/

int CreateLuaPlugin(lua_State *L);
void GetAttackUptable(lua_State *L);

void DefineAttackTable(lua_State *L) {
	luaL_Reg fns[] = {
//		{ "NewPlugin", CreateAttackPlugin },
//		{ "NewPlugin", CreateLuaPlugin },
		{ "RegisterPlugin", RegisterLuaAttacks },
		{ "AttackData", DefineAttackDataTable },
		{ NULL, NULL }
	};

	lua_newtable(L);
	luaL_setfuncs(L, fns, 0);

//	lua_newtable(L);

//	lua_newtable(L);
//	lua_pushstring(L, "lmfaooooo");
//	lua_setfield(L, -2, "value");

//	lua_pushboolean(L, 2);
//	lua_setfield(L, -2, "has_default");

//	lua_setfield(L, -2, "CanAttack");

	GetAttackUptable(L);
	lua_pushcclosure(L, CreateLuaPlugin, 1);
	lua_setfield(L, -2, "NewPlugin");
}

#define Field(L, key, type, pushvalue, has_default, lua_name) do{	\
	lua_newtable((L));						\
	lua_pushboolean((L), (has_default));				\
	lua_setfield((L), -2, "default_type");				\
	pushvalue;							\
	lua_setfield((L), -2, "value");					\
	lua_pushinteger((L), (type));					\
	lua_setfield((L), -2, "type");					\
	lua_pushstring((L), (lua_name));				\
	lua_setfield((L), -2, "name");					\
	lua_setfield((L), -2, key);					\
} while(0)

void GetAttackUptable(lua_State *L) {
	lua_newtable(L);

	Field(L, "AttackName",	LUA_TSTRING,  lua_pushstring(L, "nil"),0, "disp_name");
	Field(L, "RequiredEnergy",	LUA_TNUMBER,  lua_pushinteger(L,0), 1,    "minimum_energy");
	Field(L, "EarliestRound",	LUA_TNUMBER,  lua_pushinteger(L,0), 1,    "first_round");
	Field(L, "CanAttack",	LUA_TFUNCTION,lua_pushnil(L), 1,	     "can_attack");
	Field(L, "AttackHandler",	LUA_TFUNCTION,lua_pushnil(L), 1,	     "attack_handler");
	Field(L, "id",		LUA_TNUMBER,  lua_pushinteger(L,0), 1,    "id");
}
