#include"entities/lua_entities.h"
#include"lua_bossfight.h"
#include<lua.h>

//enum LuaPerm {
//	PERM_ENTITY,
//	PERM_ATTACK
//};

void DefineAttackTable(lua_State *L);

// pushes bossfight table to top of stack
void LoadBossfightTable(lua_State *L, int Perms) {
	lua_newtable(L);

	DefineEntityTable(L);
	lua_setfield(L, -2, "entity");

//	printf("tst\n");

	DefineAttackTable(L);
	lua_setfield(L, -2, "attack");

	lua_setglobal(L, "bossfight");
}
