#pragma once

#include<stddef.h>

//enum LuaPermissions {
//	PERM_ENTITY,
//	PERM_ATTACK
//};

//struct {
//	size_t Count;
//	const char *Filename;
//} LoadedScripts;

struct LuaData {
	// null-terminated array of dependencies
	struct LuaData *DependencyScripts;
};

#include"../entity.h"
#include"entity_ldr.h"
#include"../registration/registration.h"

int CheckLuaEntities(void);
size_t LoadLuaEntities(void *LuaState, DefMgr_t *Players, DefMgr_t *Bosses);
size_t LoadLuaAttacks(void *LuaState, Registrar_t *Registrar);

