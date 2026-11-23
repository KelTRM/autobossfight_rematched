#pragma once

#include<lua.h>
#include"../debug/debug.h"
#include"entities/lua_entities.h"
#include"../registration/registration_allocator.h"

struct BossfightLuaState {
	lua_State *L;
	struct {
		DefMgr_t Players;
		DefMgr_t Bosses;
	} Entities;

	// struct incase of future additions
//	struct {
//		RegistrationMgr_t mgr;
//	} Attacks;
};

enum LuaPerm {
	PERM_ENTITY=1,
	PERM_ATTACK=2
};

void LoadBossfightTable(lua_State *L);

#define TopWarn(L, correct_top)	if((correct_top) != lua_gettop((L))) {		\
	write_verbose(Warn, "%s:%s TOP ERROR. INTENDED=%d,ACTUAL=%d",		\
			__FILE__,__LINE__,(correct_top),lua_gettop((L)));	\
}
