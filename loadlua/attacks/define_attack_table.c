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

// local attacks=bossfight:CreateAttackPlugin();

void DefineAttackTable(lua_State *L) {
//	printf("Test\n");

	luaL_Reg fns[] = {
		{ "CreateAttackPlugin", CreateAttackPlugin },
		{ "RegisterAttackPlugin", RegisterLuaAttacks },
		{ "AttackData", DefineAttackDataTable },
		{ NULL, NULL }
	};

	lua_newtable(L);
	luaL_setfuncs(L, fns, 0);
}
