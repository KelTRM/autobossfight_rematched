#include"../../transformations/transformation_manager.h"
#include"../../registration/registration_allocator.h"
#include"../lua_bossfight.h"

//	PluginField(L, "name",		LUA_TSTRING, lua_pushnil(L), 0, "name");
//	PluginField(L, "cost",		LUA_TNUMBER, lua_pushnil(L), 0, "cost");
//	PluginField(L, "upkeep",	LUA_TNUMBER, lua_pushnil(L), 0, "upkeep");
//	PluginField(L, "multiplier",	LUA_TNUMBER, lua_pushnil(L), 0, "multiplier");
//	PluginField(L, "on_damage",	LUA_TNUMBER, lua_pushnil(L), 1, "on_damage");
//	PluginField(L, "on_upkeep",	LUA_TNUMBER, lua_pushnil(L), 1, "on_upkeep");

int LoadLuaTransformations(void *LuaState, RegistrationMgr_t *Manager) {
	struct BossfightLuaState *State = LuaState;
}
