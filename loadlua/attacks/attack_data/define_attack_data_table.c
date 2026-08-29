#include<lua.h>
#include<lauxlib.h>
#include"../../../debug/debug.h"
//	Entity_t *Target;
//	Entity_t *Attacker;

//	AttackID_t Attack;
//	Health_t Damage;
	
//	Health_t PriorHealth;

//	AttackAnnouncer Announcer;

void AssertTop(lua_State *L, int top, const char *msg) {
	int CurrentTop = lua_gettop(L);

	if (top != CurrentTop) {
		lua_pushstring(L, msg);
		lua_error(L);
	}
}

void SetLuaField(lua_State *L, const char *FieldName, int idx, int RequiredType) {
	int PassedType = lua_type(L, -1);
	if (PassedType != RequiredType) {
		lua_pushfstring(L, "expected type %d. got %d instead",
				lua_typename(L, RequiredType),
				lua_typename(L, PassedType));
		lua_error(L);
	}

	lua_setfield(L, idx, FieldName);
}

int AttackData_SetTarget(lua_State *L) {
	AssertTop(L, 2, "expected AttackData:SetTarget(x)");
	SetLuaField(L, "target", 1, LUA_TTABLE);

	return 0;
}

int AttackData_SetAttacker(lua_State *L) {
	AssertTop(L, 2, "expected AttackData:SetAttacker(x)");
	SetLuaField(L, "attacker", 1, LUA_TTABLE);

	return 0;
}

int AttackData_SetDamage(lua_State *L) {
	AssertTop(L, 2, "expected AttackData:SetDamage(x)");
	SetLuaField(L, "damage", 1, LUA_TNUMBER);

	return 0;
}

int AttackData_SetAnnouncer(lua_State *L) {
	AssertTop(L, 2, "expected AttackData:SetAnnouncer(x)");
	SetLuaField(L, "announcer", 1, LUA_TFUNCTION);

	return 0;
}

int AttackData_SetID(lua_State *L) {
	AssertTop(L, 2, "expected AttackData:SetID(x)");
	SetLuaField(L, "id", 1, LUA_TNUMBER);

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

	luaL_setfuncs(L, fns, 0);

	write_debug(DefineAttackDataTable, "recieved top=%d", top);

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

		// target
		lua_getfield(L, 2, "target");
		lua_setfield(L, -2, "target");

		// attacker
		lua_getfield(L, 2, "attacker");
		lua_setfield(L, -2, "attacker");
	}

	lua_pushnumber(L, 0);
	lua_setfield(L, -2, "damage");

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

