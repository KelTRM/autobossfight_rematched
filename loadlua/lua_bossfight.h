#pragma once

#include<lua.h>

enum LuaPerm {
	PERM_ENTITY=1,
	PERM_ATTACK=2
};

void LoadBossfightTable(lua_State *L, int Perms);
