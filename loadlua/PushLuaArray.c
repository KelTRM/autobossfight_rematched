#include"../debug/debug.h"
#include<lua.h>

// pushes table into array at end of stack
int PushLuaArray(lua_State *L, int ArrayIndex) {
	size_t Length = lua_rawlen(L, ArrayIndex);

	write_debug(PushLuaArray, "Writing data to index %d.", Length+1);

	lua_rawseti(L, ArrayIndex, Length+1);
	return 1;
}
