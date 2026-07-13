#pragma once

#include"../entity.h"
#include"entity_ldr.h"

int CheckLuaEntities(void);
size_t LoadLuaEntities(DefMgr_t *Players, DefMgr_t *Bosses);
