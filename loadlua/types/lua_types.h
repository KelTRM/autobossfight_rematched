#pragma once

#include<lua.h>
#include"../entity.h"

void CreateEntityTable(lua_State *L, Entity_t *Entity);
Entity_t GetEntityFromTable(lua_State *L);

