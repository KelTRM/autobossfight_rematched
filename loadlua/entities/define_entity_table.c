// lua shit
#include<lua.h>
#include<lauxlib.h>
#include<lualib.h>
#include"lua_entities.h"

// Reads bossfight table from top of stack, adds entity field
void DefineEntityTable(lua_State *L) {
	luaL_Reg fns[] = {
		{ "AddBoss", LuaAddBoss },
		{ "AddPlayer", LuaAddPlayer },
		{ NULL, NULL }
	};

	// bossfight:entity table
	lua_newtable(L);
	InitializeEntityManagerTable(L, -1);
	luaL_setfuncs(L, fns, 0);
}
