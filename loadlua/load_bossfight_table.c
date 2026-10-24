#include"entities/lua_entities.h"
#include"lua_bossfight.h"
#include<lua.h>

//enum LuaPerm {
//	PERM_ENTITY,
//	PERM_ATTACK
//};

int Debug_GetBossfightTable(lua_State *L) {
	lua_getfield(L, LUA_REGISTRYINDEX, "bossfight");
	return 1;
}

void DefineAttackTable(lua_State *L);
void DefineTransformationTable(lua_State *L);

// pushes bossfight table to top of stack
void LoadBossfightTable(lua_State *L) {
	lua_newtable(L);

	DefineEntityTable(L);
	lua_setfield(L, -2, "entity");

//	printf("tst\n");

	DefineAttackTable(L);
	lua_setfield(L, -2, "attack");

	DefineTransformationTable(L);
	lua_setfield(L, -2, "transformation");

	lua_pushcfunction(L, Debug_GetBossfightTable);
	lua_setfield(L, -2, "get_bossfight_table");

	lua_setglobal(L, "bossfight");
}
