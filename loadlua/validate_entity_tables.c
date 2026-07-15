#include"../entity/entity_manager.h"
#include<lua.h>

// Asserts that a t[Name] of type ExpectedType where t is the table at the top of the stack
void VerifyField(lua_State *L, const char *Name, int ExpectedType) {
	int Type = lua_getfield(L, -1, Name);
	if (Type != ExpectedType) {
		lua_pushfstring(L, "Expected [\"%s\"] of type %s. got %s instead.", Name,
					lua_typename(L, ExpectedType),
					lua_typename(L, Type));
		lua_error(L);
	}

	lua_pop(L, 1);
}

// Verifies a boss table at the top of the stack. Returns lua error if otherwise
void VerifyBossDefTable(lua_State *L) {
	VerifyField(L, "Name", LUA_TSTRING);
	VerifyField(L, "HP", LUA_TNUMBER);
}

// Verifies a player table at the top of the stack. Returns lua error if otherwise
void VerifyPlayerDefTable(lua_State *L) {
	// players are supersets of bosses
	VerifyBossDefTable(L);

	VerifyField(L, "HealMin", LUA_TNUMBER);
	VerifyField(L, "HealMax", LUA_TNUMBER);
	VerifyField(L, "Color", LUA_TNUMBER);
}


