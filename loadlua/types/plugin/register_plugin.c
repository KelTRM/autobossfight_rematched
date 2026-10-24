#include<lua.h>
#include"lua_plugin.h"
#include"../../../debug/debug.h"

#define LUA_PLUGINS_ADDR	"bossfight.plugins"

void AssertUpvalues(lua_State *L, size_t Count, ...);
void AssertParameters(lua_State *L, const char *Prototype, int ParamCount, ...);
int GetRegistryTable(lua_State *L, const char *Location);

// Takes 1 upvalue denoting which plugin table to use
// Takes 1 parameter for the value to register
int RegisterPlugin(lua_State *L) {
	AssertUpvalues(L, 1, LUA_TSTRING);
	AssertParameters(L, "bossfight.module:RegisterPlugin", 2, LUA_TTABLE, LUA_TTABLE);

	// don't care about formatting. just assume it to be correct and let the user suffer if it's fucked up. It'd be their fault anyways
	

	int res = GetRegistryTable(L, LUA_PLUGINS_ADDR);
	if (!res) {
		write_log(Error, "Failed to load plugins table. "
				"Is the registry configured correctly?");
		TypeError(L, LUA_TTABLE, LUA_TNONE);
	}

	TypeAssert("REGISTRY." LUA_PLUGINS_ADDR, LUA_TTABLE, lua_type(L, -1));

	lua_pushvalue(L, lua_upvalueindex(1));
	int type = lua_gettable(L, -2);

	TypeAssert(lua_tostring(L, lua_upvalueindex(1)), LUA_TTABLE, type);

	lua_remove(L, -2);
	
	// plugin table
	lua_newtable(L);

	// add the input entries to the plugin table
	type = lua_getfield(L, 2, "entries");

	int Registrations = 0;

	// shouldn't need to worry about integer overflows (especially at 2^63), but
	// it's an int to be sure. Pretty sure it ain't a problem anyways since the
	// lua API uses 32-bit integers for the lua_rawlen result anyways but better be
	// safe than sorry I guess.
	int Entries = lua_rawlen(L, -1);
	for (ssize_t i = 1; i <= Entries; i++) {
		type = lua_rawgeti(L, -1, i);
		if (type != LUA_TTABLE) {
			write_verbose(Info, "Skipping non-table entry @ index %zi.", i);
			lua_pop(L, 1);
			continue;
		}

		type = lua_getfield(L, -1, "int_name");
		if (type != LUA_TSTRING) {
			write_verbose(Warning, "Malformed entry table @ idx=%d. Skipping", i);
			lua_pop(L, 2);
			continue;
		}

		Registrations++;

		lua_pushvalue(L, -2);
		lua_settable(L, -5);

		lua_pop(L, 1);
	}

	lua_pop(L, 1);

	int PluginIdx = lua_rawlen(L, -2) + 1;
	lua_rawseti(L, -2, PluginIdx);
	lua_pop(L, 1);

	lua_pushinteger(L, Registrations);
	return 1;
}
