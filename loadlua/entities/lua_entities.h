#pragma once

#include<lua.h>

int LuaAddBoss(lua_State *L);
int LuaAddPlayer(lua_State *L);
int InitializeEntityManagerTable(lua_State *L, int Index);

