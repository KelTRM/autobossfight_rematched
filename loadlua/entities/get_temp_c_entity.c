#include<lua.h>
#include"../../entity.h"

static int pop0(lua_State *L) {
	lua_pop(L, 1);
	return 0;
}

int GetTempEntity(lua_State *L, int idx, Entity_t *Entity) {
	int AbsoluteIdx = lua_absindex(L, idx);

	int type;
	type = lua_getfield(L, AbsoluteIdx, "name");
	if (type != LUA_TSTRING) return pop0(L);

	Entity->Name = lua_tostring(L, -1);
	lua_pop(L, 1);

	type = lua_getfield(L, AbsoluteIdx, "color");
	if (type != LUA_TTABLE) return pop0(L);

	type = lua_getfield(L, -1, "r");
	if (type != LUA_TNUMBER) return pop0(L);
	Entity->EntityColor.r = lua_tonumber(L, -1);
	lua_pop(L, 1);

	type = lua_getfield(L, -1, "b");
	if (type != LUA_TNUMBER) return pop0(L);
	Entity->EntityColor.g = lua_tonumber(L, -1);
	lua_pop(L, 1);
	
	type = lua_getfield(L, -1, "g");
	if (type != LUA_TNUMBER) return pop0(L);
	Entity->EntityColor.b = lua_tonumber(L, -1);
	lua_pop(L, 1);

	return 1;
}
