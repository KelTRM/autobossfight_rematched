#include <lua.h>
#include<stdio.h>
#include<stdlib.h>
#include"lua_plugin.h"

int GetRegistryTable(lua_State *L, const char *Location);

#define BOSSFIGHT_PLUGINS_ADDR	"bossfight.plugins"

int GetPluginEntry(const char *RegistrarName, const struct LuaPluginLocation *Location) {
	if (Location == NULL) return 0;

	lua_State *L = Location->L;
	if (L == NULL) return 0;

	int r = GetRegistryTable(L, BOSSFIGHT_PLUGINS_ADDR);
	if (r == 0) {
		TypeError(L, LUA_TNONE, LUA_TTABLE);
	}

	r = lua_getfield(L, -1, RegistrarName);
	lua_remove(L, -2);

	TypeAssert(RegistrarName, LUA_TTABLE, r);

	lua_rawgeti(L, -1, Location->PluginID);
}
