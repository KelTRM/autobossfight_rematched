#pragma once

#include<lua.h>

int CreateLuaPlugin(lua_State *L);

// sagn_noreturn (Standard-Agnostic No return)
#if __STDC_VERSION__ >= 202311L
#define sagn_noreturn [[noreturn]]
#elif defined(__GNUC__)
#define sagn_noreturn __attribute__((noreturn))
#elif defined(_MSC_VER)
#define sagn_noreturn __declspec(noreturn)
#else
#define sagn_noreturn
#endif

sagn_noreturn void TypeError(lua_State *L, int Expected, int Required);

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


