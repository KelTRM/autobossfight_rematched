#include"entity_ldr.h"
#include"lua_entities.h"
#include<lua.h>

int RegisterLuaEntities(lua_State *L, DefMgr_t *Players, DefMgr_t *Bosses) {
	lua_getglobal(L, "bossfight");
	lua_getfield(L, -1, "entity");

	lua_getfield(L, -1, "Bosses");
	int Count = RegisterLuaBosses(L, -1, Bosses);
	lua_pop(L, 1); // pop(Bosses)

	lua_getfield(L, -1, "Players");
	Count += RegisterLuaPlayers(L, -1, Players);
	lua_pop(L, 3); // pop(Players); pop(bossfight["entity"]); pop(bossfight)

	return Count;
}

