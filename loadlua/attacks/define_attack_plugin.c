#include <assert.h>
#include<lua.h>
#include<lauxlib.h>
#include"../../debug/debug.h"

int PushLuaArray(lua_State *L, int ArrayIndex);

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
	// lua_pop(L, 1);

	lua_pushnumber(L, ID);
	lua_setfield(L, -2, "id");
	// lua_pop(L, 1);

	lua_pushnumber(L, EarliestRound);
	lua_setfield(L, -2, "first_round");
	// lua_pop(L, 1);

	lua_pushnumber(L, RequiredEnergy);
	lua_setfield(L, -2, "minimum_energy");
	// lua_pop(L, 1);

	lua_pushstring(L, AttackDispName);
	lua_setfield(L, -2, "disp_name");
	// lua_pop(L, 1);

	lua_pushstring(L, AttackName);
	lua_setfield(L, -2, "int_name");
	/// lua_pop(L, 1);

	lua_getfield(L, 1, "current_entries");
//	lua_gettable(L, -2);
	lua_pushvalue(L, -2);

	PushLuaArray(L, -2);

//	int len = lua_rawlen(L, -1);	// #current_entries
//	lua_rawseti(L, -2, len+1);

//	lua_pop(L, 1);

//	lua_setfield(L, 1, "current_entries");

	lua_pushnumber(L, 1);
	return 1;
}

int CreateAttackPlugin(lua_State *L) {
	int n = lua_gettop(L);

//	lua_pushnumber(L, 0);
//	return 1;
	
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
