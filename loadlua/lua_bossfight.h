#pragma once

#include<lua.h>
#include"entities/lua_entities.h"
#include"attacks/lua_attack_manager.h"

struct BossfightLuaState {
	lua_State *L;
	struct {
		DefMgr_t Players;
		DefMgr_t Bosses;
	} Entities;

	// struct incase of future additions
	struct {
		AttackMgr_t mgr;
	} Attacks;
};

enum LuaPerm {
	PERM_ENTITY=1,
	PERM_ATTACK=2
};

void LoadBossfightTable(lua_State *L, int Perms);
