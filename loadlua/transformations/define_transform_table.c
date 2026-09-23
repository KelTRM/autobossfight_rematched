#include<lua.h>
#include<lauxlib.h>

int CreateTransformationPlugin(lua_State *L) {
	int n = lua_gettop(L);
	if (n != 1) {
		lua_pushliteral(L, "expected bossfight.attack:CreateAttackPlugin()");
		lua_error(L);
	}

//	lua_pushnumber(L, 0);
//	return 1;
	
	luaL_Reg fns[] = {
		{ "AddTransformation", NULL },
		{ NULL, NULL }
	};
	
	lua_newtable(L);

	lua_newtable(L);
	lua_setfield(L, -2, "current_entries");
	luaL_setfuncs(L, fns, 0);

	return 1;
}

void DefineTransformationTable(lua_State *L) {
	luaL_Reg Funcs[] = {
		{ "NewPlugin", CreateTransformationPlugin },
		{ NULL, NULL }
	};

	lua_newtable(L);
	luaL_setfuncs(L, Funcs, 0);
}
