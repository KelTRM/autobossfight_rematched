// lua shit
#include<lua.h>
#include<lauxlib.h>
#include<lualib.h>
#include"lua_entities.h"

void DefineEntityTable(lua_State *L) {
	luaL_Reg fns[] = {
		{ "AddBoss", LuaAddBoss },
		{ "AddPlayer", LuaAddPlayer },
		{ NULL, NULL }
	};

	lua_newtable(L);
	
	// bossfight:entity table
	lua_newtable(L);
	InitializeEntityManagerTable(L, -1);
	luaL_setfuncs(L, fns, 0);
	
	lua_setfield(L, -2, "entity");

	lua_setglobal(L, "bossfight");
}
