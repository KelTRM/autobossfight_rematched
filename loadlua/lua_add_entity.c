#include"lua_registration.h"
#include"table_validation.h"

#define BOSS_TABLE_DEFINITION	"{ [\"Name\"]=string, [\"HP\"]=number }"

int LuaAddBoss(lua_State *L) {
	int n = lua_gettop(L);

	if (n != 2) {
		lua_pushliteral(L, "expected 1 argument of " BOSS_TABLE_DEFINITION);
		lua_error(L);
	}

	VerifyBossDefTable(L);

	int Type = lua_getfield(L, -2, "Bosses");
	if (Type != LUA_TTABLE) {
		lua_pushfstring(L, "Expected [\"Bosses\"] of type table");
		lua_error(L);
	}

	lua_pushvalue(L, -2);
	PushLuaArray(L, -2);
	lua_pop(L, 1);	// pop(Bosses)
	
	lua_pushnumber(L, 1);

	return 1;
}

int LuaAddPlayer(lua_State *L) {
	int n = lua_gettop(L);

//	write_debug(LuaAddBoss, "recieved %d args", n);

	if (n != 2) {
		lua_pushliteral(L, "expected 1 argument of " BOSS_TABLE_DEFINITION);
		lua_error(L);
	}

	VerifyPlayerDefTable(L);

	int Type = lua_getfield(L, -2, "Players");
	if (Type != LUA_TTABLE) {
		lua_pushfstring(L, "Expected [\"Players\"] of type table");
		lua_error(L);
	}

	lua_pushvalue(L, -2);
	PushLuaArray(L, -2);
	lua_pop(L, 1);	// pop(Players)

	lua_pushnumber(L, 1);

	return 1;
}

