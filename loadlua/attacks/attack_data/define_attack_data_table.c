#include<lua.h>
#include<lauxlib.h>
//	Entity_t *Target;
//	Entity_t *Attacker;

//	AttackID_t Attack;
//	Health_t Damage;
	
//	Health_t PriorHealth;

//	AttackAnnouncer Announcer;

int AttackData_SetTarget(lua_State *L) {
	int top = lua_gettop(L);
	return 0;
}

int AttackData_SetAttacker(lua_State *L) {
	int top = lua_gettop(L);
	return 0;
}

int AttackData_SetDamage(lua_State *L) {
	int top = lua_gettop(L);
	return 0;
}

int AttackData_SetAnnouncer(lua_State *L) {
	int top = lua_gettop(L);
	return 0;
}

int AttackData_SetID(lua_State *L) {
	int top = lua_gettop(L);
	return 0;
}

int DefineAttackDataTable(lua_State *L) {
	int top = lua_gettop(L);
	lua_newtable(L);

	luaL_Reg fns[] = {
		{ "SetTarget", AttackData_SetTarget },
		{ "SetAttacker", AttackData_SetAttacker },
		{ "SetDamage", AttackData_SetDamage },
		{ "SetAnnouncer", AttackData_SetAnnouncer },
		{ "SetID", AttackData_SetID },
		{ NULL, NULL },
	};

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

//The AttackData type has the following data points attached to it.

// - Target (target of the given attack) entity
// - Attacker (attacker of the target) entity
// - Damage (damage dealt) number
// - Announcer (method to handle attack message) function
// - ID (leave nil) number

//All of the following can be altered using `AttackData:SetX(X)`
//For example

//```lua
//local attack = bossfight.attack:AttackData()
//attack:SetDamage(100)
//```

