#pragma once

#include<lua.h>

// Verifies a boss table at the top of the stack. Returns lua error if otherwise
void VerifyBossDefTable(lua_State *L);
// Verifies a player table at the top of the stack. Returns lua error if otherwise
void VerifyPlayerDefTable(lua_State *L);

