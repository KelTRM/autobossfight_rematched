#include<lua.h>
#include<lauxlib.h>
#include"../types/plugin/lua_plugin.h"

void GetTransformUptable(lua_State *L);
int RegisterPlugin(lua_State *L);

#define NO_DESC			"No description"

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

	lua_pushstring(L, "transformation");
	lua_pushcclosure(L, RegisterPlugin, 1);
	lua_setfield(L, -2, "RegisterPlugin");
}
//    ["name"]="example"          -- The name of the transformation
//    ["cost"]=10                 -- The cost to transform
//    ["upkeep"]=10               -- The cost to continue using the transformation
//    ["multiplier"]=1            -- A multiplier for attack damage. For use by the default handler
//    ["on_damage"]=function()end -- Called apon user's attack. Modifies damage. Use nil for default 
//    ["on_upkeep"]=function()end -- Called apon
void GetTransformUptable(lua_State *L) {
	lua_newtable(L);

	PluginField(L, "name",		LUA_TSTRING,	lua_pushnil(L),	0, "name");
	PluginField(L, "description",	LUA_TSTRING,lua_pushliteral(L, NO_DESC),1, "description");
	PluginField(L, "cost",		LUA_TNUMBER,	lua_pushnil(L),	0, "cost");
	PluginField(L, "upkeep",		LUA_TNUMBER,	lua_pushnil(L),	0, "upkeep");
	PluginField(L, "multiplier",		LUA_TNUMBER,	lua_pushnil(L),	0, "multiplier");
	PluginField(L, "on_damage",		LUA_TFUNCTION,	lua_pushnil(L),	1, "on_damage");
	PluginField(L, "on_upkeep",		LUA_TFUNCTION,	lua_pushnil(L),	1, "on_upkeep");
	PluginField(L, "id",			LUA_TFUNCTION,	lua_pushnil(L),	1, "id");
}
