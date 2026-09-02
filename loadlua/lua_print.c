#include<stdio.h>
#include"../ui/console_manager/console.h"
#include<lua.h>

int LuaPrint(lua_State *L);

void LuaSetPrint(lua_State *L) {
	lua_pushcclosure(L, LuaPrint, 0);
	lua_setglobal(L, "print");
}

int LuaPrint(lua_State *L) {
	int top = lua_gettop(L);
	if (top < 1) {
		return 0;
	}

	const char *str = lua_tolstring(L, 1, NULL);
	if (str == NULL) {
		lua_pushfstring(L, "Cannot convert type %s to string",
				lua_typename(L, lua_type(L, 1)));
		lua_error(L);
	}

	printf("%s\n", str);

	return 0;
}
