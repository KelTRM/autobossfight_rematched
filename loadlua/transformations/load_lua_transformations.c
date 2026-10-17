#include<lua.h>
#include"../../transformations/transformation_manager.h"
#include"../../registration/registration_allocator.h"
#include"../lua_bossfight.h"

//	PluginField(L, "name",		LUA_TSTRING, lua_pushnil(L), 0, "name");
//	PluginField(L, "cost",		LUA_TNUMBER, lua_pushnil(L), 0, "cost");
//	PluginField(L, "upkeep",	LUA_TNUMBER, lua_pushnil(L), 0, "upkeep");
//	PluginField(L, "multiplier",	LUA_TNUMBER, lua_pushnil(L), 0, "multiplier");
//	PluginField(L, "on_damage",	LUA_TNUMBER, lua_pushnil(L), 1, "on_damage");
//	PluginField(L, "on_upkeep",	LUA_TNUMBER, lua_pushnil(L), 1, "on_upkeep");

Transformation_t GetTransformationFromTable(lua_State *L, int idx) {
	Transformation_t Transformation = { 0 };

	lua_getfield(L, idx, "name");
	Transformation.TransformationName = (char*)lua_tostring(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, idx, "int_name");
	Transformation.Identifier = (char*)lua_tostring(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, idx, "cost");
	Transformation.Cost = lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, idx, "upkeep");
	Transformation.Cost = lua_tonumber(L, -1);
	lua_pop(L, 1);
	
	lua_getfield(L, idx, "multipler");
	Transformation.TransformationMultiplier = lua_tonumber(L, -1);
	lua_pop(L, 1);

}

int LoadLuaTransformations(void *LuaState, RegistrationMgr_t *Manager) {
	struct BossfightLuaState *State = LuaState;

	
}
