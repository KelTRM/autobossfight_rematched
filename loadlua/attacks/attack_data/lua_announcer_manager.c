#include<lua.h>
#include"../lua_attack.h"
#include"../../../attacks/attacks.h"
#include"../../../debug/debug.h"

AttackData_t ReadAttackDataTable(lua_State *L);
int GetRegistryTable(lua_State *L, const char *Location);

void LuaAttackAnnouncer(AttackData_t *Attack) {
	LuaAttackData_t *AttackData = Attack->LuaAttackData;
	if (AttackData == NULL)
		goto fallback;

	lua_State *L = AttackData->L;
	int top = lua_gettop(L);

//	ReadAttackDataTable(L);

//	lua_getfield(L, LUA_REGISTRYINDEX, "bossfight");
//	lua_getfield(L, -1, "attack_data");
	int Success = GetRegistryTable(L, "bossfight.attack_data");
	if (!Success)
		goto fallback;

//	lua_remove(L, -2);

	int type;
	type = lua_rawgeti(L, -1, AttackData->ArrayIdx);
	if (type != LUA_TTABLE) {
		write_debug(Error, "Recieved invalid AttackData table.");

		lua_pop(L, 2);
		goto fallback;
	}

	type = lua_getfield(L, -1, "announcer");
	if (type != LUA_TFUNCTION) {
		write_debug(Error, "Recieved announcer of type other than function.");

		lua_pop(L, 3);
		goto fallback;
	}


	lua_pushvalue(L, -2);
	lua_call(L, 1, 0);

	lua_pop(L, 2);

	return;

fallback:
	lua_settop(L, top);
	write_debug(Info, "There was an error trying to run the lua attack manager. "
			"Falling back to default.");
	DefaultAnnouncer(Attack);
}
