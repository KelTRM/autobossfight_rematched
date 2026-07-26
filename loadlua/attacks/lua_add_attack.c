#include<lua.h>
#include"../../debug/debug.h"
#include"../lua_registration.h"

void VerifyField(lua_State *L, const char *Name, int ExpectedType);
const char *ReadLuaTableString(lua_State *L, const char *Name, char *DefaultValue);
lua_Number ReadLuaTableNumber(lua_State *L, const char *Name, lua_Number DefaultValue);

int AddAttack(lua_State *L) {
//	return 0;
//	return 1;
	int n = lua_gettop(L);
	if (n != 3) {
		lua_pushliteral(L, "AddAttack: expected AttackPlugin:AddAttack(string, table)");
		lua_error(L);
	}

	int type = lua_type(L, 2);
	if (type != LUA_TSTRING) {
		lua_pushfstring(L, "expected type string (got %s instead)", lua_typename(L, type));
		lua_error(L);
	}

	type = lua_type(L, 3);
	if (type != LUA_TTABLE) {
		lua_pushfstring(L, "expected type table (got %s instead)", lua_typename(L, type));
		lua_error(L);
	}

//	return 0;

	const char *AttackName = lua_tostring(L, 2);

	const char *AttackDispName = ReadLuaTableString(L, "AttackName", "nil");
	lua_Number RequiredEnergy = ReadLuaTableNumber(L, "RequiredEnergy", 0);
	lua_Number EarliestRound = ReadLuaTableNumber(L, "EarliestRound", 0);
	lua_Number ID = ReadLuaTableNumber(L, "id", 0);

	write_debug(AddAttack, "Recieved attack of value { %s, %d, %d, %d }",
			AttackDispName, (int)RequiredEnergy, (int)EarliestRound, (int)ID);


	// push EntryTable
	lua_newtable(L);

	// push CanAttack
	type = lua_getfield(L, 3, "CanAttack");
	if (type != LUA_TFUNCTION) {
		lua_pop(L, 1);
		lua_pushnil(L);
	}

	// lua_pop(L, 1);
	lua_setfield(L, -2, "attack_handler");

	// push AttackHandler
	type = lua_getfield(L, 3, "AttackHandler");
	if (type != LUA_TFUNCTION) {
		lua_pop(L, 1);
		lua_pushnil(L);
	}

	lua_setfield(L, -2, "can_attack");

	lua_pushnumber(L, ID);
	lua_setfield(L, -2, "id");

	lua_pushnumber(L, EarliestRound);
	lua_setfield(L, -2, "first_round");

	lua_pushnumber(L, RequiredEnergy);
	lua_setfield(L, -2, "minimum_energy");

	lua_pushstring(L, AttackDispName);
	lua_setfield(L, -2, "disp_name");

	lua_pushstring(L, AttackName);
	lua_setfield(L, -2, "int_name");

	lua_getfield(L, 1, "current_entries");
	lua_pushvalue(L, -2);

	PushLuaArray(L, -2);

	lua_pushnumber(L, 1);
	return 1;
}

