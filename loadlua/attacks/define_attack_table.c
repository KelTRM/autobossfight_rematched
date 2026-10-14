#include <assert.h>
#include<lua.h>
#include<lauxlib.h>
#include"../types/plugin/lua_plugin.h"
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

void GetAttackUptable(lua_State *L);

void DefineAttackTable(lua_State *L) {
	luaL_Reg fns[] = {
		{ "RegisterPlugin", RegisterLuaAttacks },
		{ "AttackData", DefineAttackDataTable },
		{ NULL, NULL }
	};

	lua_newtable(L);
	luaL_setfuncs(L, fns, 0);

	GetAttackUptable(L);
	lua_pushcclosure(L, CreateLuaPlugin, 1);
	lua_setfield(L, -2, "NewPlugin");
}

void GetAttackUptable(lua_State *L) {
	lua_newtable(L);

	PluginField(L, "AttackName",    LUA_TSTRING,  lua_pushnil(L),	 0,"disp_name");
	PluginField(L, "RequiredEnergy",LUA_TNUMBER,  lua_pushinteger(L,0),1,"minimum_energy");
	PluginField(L, "EarliestRound", LUA_TNUMBER,  lua_pushinteger(L,0),1,"first_round");
	PluginField(L, "CanAttack",     LUA_TFUNCTION,lua_pushnil(L),	 1,"can_attack");
	PluginField(L, "AttackHandler", LUA_TFUNCTION,lua_pushnil(L),	 1,"attack_handler");
	PluginField(L, "id",	           LUA_TNUMBER,  lua_pushinteger(L,0),1,"id");
}
