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

int GetRegistryTable(lua_State *L, const char *Location);

int GetAttackPluginsTable(lua_State *L) {
	if (!GetRegistryTable(L, "bossfight.plugins.attack")) {
		lua_pushnil(L);
		return 0;
	}

	return 1;
}
