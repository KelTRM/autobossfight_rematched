#include<lua.h>
#include<lauxlib.h>
#include"../types/plugin/lua_plugin.h"

void GetTransformUptable(lua_State *L);

void DefineTransformationTable(lua_State *L) {
	luaL_Reg Funcs[] = {
//		{ "NewPlugin", CreateTransformationPlugin },
		{ NULL, NULL }
	};
	lua_newtable(L);
	luaL_setfuncs(L, Funcs, 0);
	
	GetTransformUptable(L);
	lua_pushcclosure(L, CreateLuaPlugin, 1);
	lua_setfield(L, -2, "NewPlugin");
}
//    ["name"]="example"          -- The name of the transformation
//    ["cost"]=10                 -- The cost to transform
//    ["upkeep"]=10               -- The cost to continue using the transformation
//    ["multiplier"]=1            -- A multiplier for attack damage. For use by the default handler
//    ["on_damage"]=function()end -- Called apon user's attack. Modifies damage. Use nil for default 
//    ["on_upkeep"]=function()end -- Called apon
void GetTransformUptable(lua_State *L) {
	lua_newtable(L);

	PluginField(L, "name",	LUA_TSTRING, lua_pushnil(L), 0, "name");
	PluginField(L, "cost",	LUA_TNUMBER, lua_pushnil(L), 0, "cost");
	PluginField(L, "upkeep",	LUA_TNUMBER, lua_pushnil(L), 0, "upkeep");
	PluginField(L, "multiplier",	LUA_TNUMBER, lua_pushnil(L), 0, "multiplier");
	PluginField(L, "on_damage",	LUA_TNUMBER, lua_pushnil(L), 1, "on_damage");
	PluginField(L, "on_upkeep",	LUA_TNUMBER, lua_pushnil(L), 1, "on_upkeep");
}
