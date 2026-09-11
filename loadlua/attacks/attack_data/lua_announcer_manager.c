#include<lua.h>
#include"../../../attacks/attack.h"

void LuaAttackAnnouncer(AttackData_t *Attack) {
	lua_getfield(L, LUA_REGISTRYINDEX, "bossfight");
	lua_getfield(L, -1, "");
}
