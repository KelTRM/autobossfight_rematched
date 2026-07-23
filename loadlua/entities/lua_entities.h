#pragma once

#include<lua.h>
#include"entity_ldr.h"

int LuaAddBoss(lua_State *L);
int LuaAddPlayer(lua_State *L);
int InitializeEntityManagerTable(lua_State *L, int Index);
int RegisterLuaBosses(lua_State *L, int ArrayIdx, DefMgr_t *Bosses);
int RegisterLuaPlayers(lua_State *L, int ArrayIdx, DefMgr_t *Players);
void DefineEntityTable(lua_State *L);
int RegisterLuaEntities(lua_State *L, DefMgr_t *Players, DefMgr_t *Bosses);
