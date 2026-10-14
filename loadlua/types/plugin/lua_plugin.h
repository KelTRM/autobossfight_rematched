#pragma once

#include<lua.h>

int CreateLuaPlugin(lua_State *L);

#define PluginField(L, key, type, pushvalue, has_default, lua_name) do{	\
	lua_newtable((L));						\
	lua_pushboolean((L), (has_default));				\
	lua_setfield((L), -2, "has_default");				\
	pushvalue;							\
	lua_setfield((L), -2, "value");					\
	lua_pushinteger((L), (type));					\
	lua_setfield((L), -2, "type");					\
	lua_pushstring((L), (lua_name));				\
	lua_setfield((L), -2, "name");					\
	lua_setfield((L), -2, key);					\
} while(0)


