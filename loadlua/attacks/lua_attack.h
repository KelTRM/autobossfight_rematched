#pragma once

#include<lua.h>
#include"../../attacks/attack.h"

typedef struct LuaAttack {
	lua_State *L;

	const char *AttackPluginKey;
	size_t AttackPluginIndex;
} LuaAttack_t;

typedef struct LuaAttackData {
	lua_State *L;
	size_t ArrayIdx;
} LuaAttackData_t;

int CreateAttackPlugin(lua_State *L);
int AddAttack(lua_State *L);
