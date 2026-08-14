#include"../debug/debug.h"
#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include"../utils/sleep.h"
#include"lua_registration.h"
#include"lua_file_mgr.h"
#include"lua_entities.h"
#include"../lua_bossfight.h"

// lua shit
#include<lua.h>
#include<lauxlib.h>
#include<lualib.h>

size_t LoadLuaEntities(void *LuaState, DefMgr_t *Players, DefMgr_t *Bosses) {
	CreateDefMgr(Players);
	CreateDefMgr(Bosses);

	struct BossfightLuaState *State = LuaState;
	int Count = RegisterLuaEntities(State->L, Players, Bosses);

	return Count;
}

