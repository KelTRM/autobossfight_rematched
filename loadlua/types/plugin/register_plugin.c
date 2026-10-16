#include<lua.h>
#include"../../../debug/debug.h"

#define LUA_PLUGINS_ADDR	"bossfight.plugins"

void AssertUpvalues(lua_State *L, size_t Count, ...);
void AssertParameters(lua_State *L, const char *Prototype, int ParamCount, ...);
int GetRegistryTable(lua_State *L, const char *Location);

[[noreturn]] void TypeError(lua_State *L, int Expected, int Required);

// Takes 1 upvalue denoting which plugin table to use
// Takes 1 parameter for the value to register
int RegisterPlugin(lua_State *L) {
	AssertUpvalues(L, 1, LUA_TSTRING);
	AssertParameters(L, "bossfight.module:RegisterPlugin", 2, LUA_TTABLE, LUA_TTABLE);

	// don't care about formatting. just assume it to be correct and let the user suffer if it's fucked up. It'd be their fault anyways
	
	int res = GetRegistryTable(L, LUA_PLUGINS_ADDR);
	if (!res) {
		write_log(Error, "Failed to load plugins table. Is the registry configured correctly?");
	}

	if (lua_type(L, -1) != LUA_TTABLE) {
		write_log(Error, "Expected table REGISTRY." LUA_PLUGINS_ADDR " of type table. "
				"Recieved type %s instead", lua_typename(L, lua_type(L, -1)));
		TypeError(L, LUA_TTABLE, lua_type(L, -1));
	}

	lua_pushvalue(L, lua_upvalueindex(1));
	int type = lua_gettable(L, -2);

	if (type != LUA_TTABLE) {
		write_log(Error, "Expected table REGISTRY." LUA_PLUGINS_ADDR ".%s "
				"of type table. Recieved type %s instead",
					lua_tostring(L, lua_upvalueindex(1)),
					lua_typename(L, type));
		TypeError(L, LUA_TTABLE, type);
	}


}
