#include<lua.h>

//	Entity_t *Target;
//	Entity_t *Attacker;

//	AttackID_t Attack;
//	Health_t Damage;
	
//	Health_t PriorHealth;

//	AttackAnnouncer Announcer;

int DefineAttackDataTable(lua_State *L) {
	int top = lua_gettop(L);
	lua_newtable(L);

	if (top <= 1) {
		// effectively does nothing
		lua_pushnil(L);
		lua_setfield(L, -2, "target");

		lua_pushnil(L);
		lua_setfield(L, -2, "attacker");
	} else {
		int type = lua_type(L, 2);
		if (type != LUA_TTABLE) {
			lua_pushliteral(L, "error: expected argument Attack of table");
			lua_error(L);
		}
	}

	return 1;
}
