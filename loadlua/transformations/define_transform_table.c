#include<lua.h>
#include<lauxlib.h>

void DefineTransformationTable(lua_State *L) {
	luaL_Reg Funcs[] = {
		{ "NewPlugin", NULL },
		{ NULL, NULL }
	};

	lua_newtable(L);
	luaL_setfuncs(L, Funcs, 0);
}
