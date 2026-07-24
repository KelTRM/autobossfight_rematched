#include<lua.h>
#include<lauxlib.h>

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

int AddAttack(lua_State *L) {
	int n = lua_gettop(L);
	if (n != 3) {
		lua_pushliteral(L, "AddAttack: expected AttackPlugin:AddAttack(string, table)");
		lua_error(L);
	}

	return 0;
}

int CreateAttackPlugin(lua_State *L) {
	//int n = lua_gettop(L);

	luaL_Reg fns[] = {
		{ .name="AddAttack", .func=AddAttack },
		{ NULL, NULL }
	};
	
	lua_newtable(L);

	lua_newtable(L);
	lua_setfield(L, -2, "current_entries");

	luaL_setfuncs(L, fns, 0);

	return 1;
}
