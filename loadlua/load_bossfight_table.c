#include"entities/lua_entities.h"
#include"lua_bossfight.h"
#include<lua.h>

//enum LuaPerm {
//	PERM_ENTITY,
//	PERM_ATTACK
//};

// pushes bossfight table to top of stack
void LoadBossfightTable(lua_State *L, int Perms) {
	lua_newtable(L);

	printf("Perms = %d\n", Perms);
	if ((Perms & PERM_ENTITY) != 0) {
		DefineEntityTable(L);
		lua_setfield(L, -2, "entity");

		printf("tst\n");
	}

	lua_setglobal(L, "bossfight");
}
