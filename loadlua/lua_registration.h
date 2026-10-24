#pragma once

#include<lua.h>
#include"entity_ldr.h"

int PushLuaArray(lua_State *L, int ArrayIndex);

// Register bosses to a DefMgr from a lua array in ArrayIdx
int RegisterLuaBosses(lua_State *L, int ArrayIdx, DefMgr_t *Bosses);

// Register players to a DefMgr from a lua array in ArrayIdx
int RegisterLuaPlayers(lua_State *L, int ArrayIdx, DefMgr_t *Players);
