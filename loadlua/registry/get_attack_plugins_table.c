#include<lua.h>
#include"../../debug/debug.h"

int IndexTable(lua_State *L, int Index, const char *Field, int *FailChain, int ExpectedType) {
	if (FailChain == NULL) return 0;
	if (*FailChain != 0) return ++(*FailChain);

	int type = lua_getfield(L, Index, Field);
	if (type != ExpectedType) {
		*FailChain = 1;
	}

	return *FailChain;
}

int GetAttackPluginsTable(lua_State *L) {
	int fail = 0;

	IndexTable(L, LUA_REGISTRYINDEX, "bossfight", &fail, LUA_TTABLE);
	IndexTable(L, -1, "plugins", &fail, LUA_TTABLE);
	IndexTable(L, -1, "attack", &fail, LUA_TTABLE);

	if (fail != 0) {
		lua_pop(L, 3-fail);
		write_debug(Warning, "Failed to index bossfight.plugins.attack."
					"Is the registry initialized? Error code %d", fail);

		lua_pushnil(L);
		return 0;
	}

	lua_remove(L, -2);
	lua_remove(L, -2);

	return 1;
}
